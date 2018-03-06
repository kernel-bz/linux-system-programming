/*******************************************************************************
프로젝트 : gxLib
모듈내용 : BDF 폰트 출력
변경일자 : 2008-10-22
작성자   : 푸른수염(ssseo88@chol.com)
저작권   : 주석 내용을 변경하지 않는 한 무료 제공
홈페이지 : http://forum.falinux.com
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>                                                              // malloc srand
#include <string.h>                                                              // abs

#include <unistd.h>                                                              // open/close
#include <fcntl.h>                                                               // O_RDWR

#include <gx.h>
#include <gxbdf.h>
#include <fontinfo.h>
#include <ksc5601.h>

#define MAX_STRING_LENGTH     1024

bdfFontCache   gx_fontcache;
FILE*          gx_fontfile    = NULL;

static void  _text_out_( dc_t *dc, int x1 , int y1, unsigned short* string)
//-------------------------------------------------------------------------------
{
   int            k , i, j ,m;
   int            xOrg     = x1;
   int            yOrg     = y1;
   int            xPixel, yPixel;
   int            interval = 1;
   unsigned int   nCode;
   bdfFont  Font;
   pbdfFont pFont;

   //unsigned short unistring[512];
   //convertMultibyteToUnicodeString( (unsigned char*) str[strIndex] , 256 , unistring ,512 );

   for( k = 0 ; string[k] != 0x0000 ; k+=1 )
   {
      nCode = string[k];//(0xff00 & ( ((unsigned char ) str[strIndex][k]) << 8 )) | ( ( (unsigned char) str[strIndex][k+1]) & 0x00ff );

	  //printf ("%d:uniCode=%d\n", k, nCode);

      //nCode -= 32896;
      //if( nCode < 8481 || nCode > 32126 ) continue;
      if ( ReadFontOfCode( &gx_fontcache, gx_fontfile, nCode ,&Font ))
         pFont = &Font;
      else
         pFont = NULL;

      if ( pFont != NULL )
      {
         int xSave = xPixel = xOrg + (pFont->bbxoff)*interval;
         yPixel = yOrg - (pFont->bbyoff)*interval;

         for( i = pFont->bbh -1 ; i >=0   ; --i )
         {
            for ( j=0 ; j < (pFont->bbw+8)/8 ; ++ j)
            {
               for( m = 0 ; m < 8 ; ++m )
               {
                  if ( ( j*8 + m + 1) > pFont->bbw ) break;
                  if ((( pFont->data[(pFont->bbw+7)/8*i+j] & (1 << (8-m-1)) )>>(8-m-1)) == 1 )
                  {
                     gx_set_pixel( dc ,xPixel ,yPixel, dc->pen_color);
                  }
                  else
                  {
                     //HBRUSH hOld = (HBRUSH)::SelectObject( pDC->GetSafeHdc() ,::GetStockObject(WHITE_BRUSH) );
                     //pDC->Rectangle(CRect(x,y,x+interval , y+interval ));
                     //::SelectObject( pDC->GetSafeHdc() , hOld );
                  }
                  xPixel+= interval;
               }
            }
            yPixel-= interval;
            xPixel = xSave;
         }
         xOrg+= ( pFont->dwidth*interval);
      }
   }
}

void gx_text_out( dc_t *dc , int coor_x, int coor_y, char *text)
//-------------------------------------------------------------------------------
{
   static unsigned short   uniString[MAX_STRING_LENGTH+1];
   int                     szText;

   szText   = strlen( text);
   if ( MAX_STRING_LENGTH < szText)
   {
      text[MAX_STRING_LENGTH] = '\0';
      szText                  = MAX_STRING_LENGTH;
   }

   convertMultibyteToUnicodeString( (unsigned char*)text, szText, uniString, MAX_STRING_LENGTH);

   //printf ("%s\n", text);

   _text_out_( dc, coor_x ,coor_y, uniString);
}

int gx_set_font( char *font_filename)
//-------------------------------------------------------------------------------
{
   if ( NULL != gx_fontfile)     fclose( gx_fontfile);

   gx_fontfile = fopen( font_filename, "r");  // 폰트 파일 열기
   if ( NULL == gx_fontfile)
   {
      gx_error_code  = GXERR_NO_FONT_FILE;
      return GXERR_NO_FONT_FILE;              // 폰트 파일 열기에 실패
   }

   FontCacheInit (&gx_fontcache); //bdfFontCache 구조체 초기화
   FontCacheDeInit (&gx_fontcache); //pCache->nMaxIndex 수 만큼 pCache->pFontItem 구조체 메모리 해제
   FontChanged (gx_fontfile, &gx_fontcache); //폰트파일에서 인코딩 번호를 읽어서 bdfFontIndex에 저장

   gx_error_code  = GXERR_NONE;
   return GXERR_NONE;
}

/*******************************************************************************
프로젝트 : gxLib
모듈내용 : gxLib에서 JPEG 이미지 출력
변경일자 : 2008-11-23
작성자   : 푸른수염(ssseo88@chol.com)
저작권   : 주석 내용을 변경하지 않는 한 무료 제공
홈페이지 : http://forum.falinux.com
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include "gx.h"
#include "gxbmp.h"
#include "gxjpg.h"
#else
#include <gx.h>
#include <gxbmp.h>
#include <gxjpg.h>
#endif

#include "jpeglib.h"

#include <setjmp.h>

//#define BI_RGB          0
//#define BI_RLE4         1
//#define BI_RLE8         2
//#define BI_BITFIELD     3
//
//typedef unsigned char   uch;
//typedef unsigned short  ush;
//typedef unsigned int    uln;
//typedef unsigned long   ulg;

//typedef struct
//{
//	unsigned char red;
//	unsigned char green;
//   unsigned char blue;
//} rgb_color_struct;

struct ima_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct ima_error_mgr *ima_error_ptr;


struct Iterator__
{
  int Itx, Ity;				// Counters
  int Stepx, Stepy;
  unsigned char* IterImage;  		//  Image pointer
  jpg_t *ima;
};
typedef struct Iterator__ Iterator;

//static int IterItOK ( Iterator* iter )
//{
//  if ( iter->Itx <= iter->ima->width && iter->Ity <= iter->ima->height )
//	 return TRUE;
//  else
//	 return FALSE;
//}

//static void IterReset( Iterator* iter )
//{
//  iter->IterImage = iter->ima->data;
//  iter->Itx = iter->Ity = 0;
//}

//static int IterNextRow(Iterator* iter )
//{
//  if (++(iter->Ity) >= iter->ima->height) return 0;
//  iter->IterImage += iter->ima->bytes_per_line;
//  return 1;
//}

static int IterPrevRow(Iterator* iter )
{
  if (--(iter->Ity) < 0) return 0;
  iter->IterImage -= iter->ima->bytes_per_line;
  return 1;
}

static void IterUpset(Iterator* iter )
{
  iter->Itx = 0;
  iter->Ity = iter->ima->height-1;
  iter->IterImage = iter->ima->data + iter->ima->bytes_per_line*(iter->ima->height-1);
}

////////////////////////// AD - for interlace ///////////////////////////////
//static void IterSetY(Iterator* iter ,int y)
//{
//  if ((y < 0) || (y > iter->ima->height)) return;
//  iter->Ity = y;
//  iter->IterImage = iter->ima->data + iter->ima->bytes_per_line*y;
//}

/////////////////////////////////////////////////////////////////////////////

static void IterSetRow( Iterator* iter ,unsigned char *buf, int n )
{
// Here should be bcopy or memcpy
  //_fmemcpy(IterImage, (void far *)buf, n);
	int i;
  if (n<0 || n > iter->ima->width )
	 n = iter->ima->width;

  for (i=0; i<n; i++)
	  iter->IterImage[i] = buf[i];
}

//static void IterGetRow(Iterator* iter ,unsigned char *buf, int n)
//{
//	int i;
//  for (i=0; i<n; i++) buf[i] = iter->IterImage[i];
//}

//static unsigned char* IterGet(Iterator* iter )
//{
//  return iter->IterImage;
//}

//static int IterNextByte(Iterator* iter )
//{
//  if (++(iter->Itx )< iter->ima->width)
//	 return 1;
//  else
//	 if (++(iter->Ity) < iter->ima->height)
//	 {
//		iter->IterImage += iter->ima->bytes_per_line;
//		iter->Itx = 0;
//		return 1;
//	 } else
//		return 0;
//}
//
//static int IterPrevByte(Iterator* iter )
//{
//  if (--(iter->Itx) >= 0)
//	 return 1;
//  else
//	 if (--(iter->Ity) >= 0)
//	 {
//		iter->IterImage -= iter->ima->bytes_per_line;
//		iter->Itx = 0;
//		return 1;
//	 } else
//		return 0;
//}
//
//static int IterNextStep(Iterator* iter )
//{
//  iter->Itx += iter->Stepx;
//  if (iter->Itx < iter->ima->bytes_per_line)
//	 return 1;
//  else {
//	 iter->Ity += iter->Stepy;
//	 if (iter->Ity < iter->ima->height)
//	 {
//		iter->IterImage += iter->ima->bytes_per_line;
//		iter->Itx = 0;
//		return 1;
//	 } else
//		return 0;
//  }
//}
//
//static int IterPrevStep(Iterator* iter )
//{
//  iter->Itx -= iter->Stepx;
//  if (iter->Itx >= 0)
//	 return 1;
//  else {
//	 iter->Ity -= iter->Stepy;
//	 if (iter->Ity >= 0 && iter->Ity < iter->ima->height)
//	 {
//		iter->IterImage -= iter->ima->bytes_per_line;
//		iter->Itx = 0;
//		return 1;
//	 } else
//		return 0;
//  }
//}


/*
 * Here's the routine that will replace the standard error_exit method:
 */

void ima_jpeg_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  ima_error_ptr myerr = (ima_error_ptr) cinfo->err;

  char buffer[JMSG_LENGTH_MAX];

  /* Create the message */
  myerr->pub.format_message (cinfo, buffer);

  /* Send it to stderr, adding a newline */
//        AfxMessageBox(buffer);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}



static void CreateGrayColourMap( palette_t* palette , int n )
{
  int i;
  unsigned char g;

  for (i=0; i<n; i++)
  {
	g = (unsigned char)(256*i/n);
	palette[i].blue = palette[i].green = palette[i].red = g;
	palette[i].filter = 0;
  }
}

static int SetPalette( palette_t* palette, int n, unsigned char *r, unsigned char *g, unsigned char *b)
{
	int i;
	for( i=0; i <n ;++i )
	{
		palette[i].red = r[i];
		palette[i].green = g[i];
		palette[i].blue = b[i];
		palette[i].filter = 0;
	}
	return 1;
}

//static int SetPaletteStruct( palette_t* palette, int n, rgb_color_struct* p )
//{
//	int i;
//	for( i=0; i <n ;++i )
//	{
//		palette[i].red = p[i].red;
//		palette[i].green = p[i].green;
//		palette[i].blue = p[i].blue;
//		palette[i].filter = 0;
//	}
//	return 1;
//}

static void  release_dc( dc_t *dc)
//-------------------------------------------------------------------------------
{
   jpg_t  *jpg  = (jpg_t *)dc;

   if ( NULL == jpg)             return;
   if ( NULL != jpg->palette)    free( jpg->palette);
   if ( NULL != jpg->data   )    free( jpg->data   );
   free( jpg );
}


static void set_error_code( jpg_t *jpg, FILE *fp, int error_code)
//-------------------------------------------------------------------------------
{
   gx_error_code  = error_code;
   release_dc( ( dc_t *)jpg );
   if ( fp)    fclose(fp);
}

jpg_t *gx_jpg_open( char  *filename )
{

   jpg_t    *jpg;
   Iterator iter;
   struct jpeg_decompress_struct cinfo;
   struct ima_error_mgr jerr;
   FILE * fp = NULL;
   JSAMPARRAY buffer;
   int row_stride;

   if ( NULL == ( jpg = malloc( sizeof( jpg_t))) )
   {
      set_error_code( jpg, fp, GXERR_OUT_OF_MEMORY);
      return NULL;
   }
   memset( jpg, 0, sizeof( jpg_t));

  memset( &iter, 0x00, sizeof(Iterator));
  iter.ima = jpg;


  fp = fopen((const char *)filename, "rb");
  if ( NULL == ( fp) )                                                          // 파일이 없거나 열기에 실패했다면
  {
    set_error_code( jpg, fp, GXERR_NOFILE);
    return NULL;
  }
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = ima_jpeg_error_exit;

  if (setjmp(jerr.setjmp_buffer)) {
     set_error_code( jpg, fp, GXERR_PROCESSING);
	 jpeg_destroy_decompress(&cinfo);
	 return NULL;
  }
  jpeg_create_decompress(&cinfo);

  jpeg_stdio_src(&cinfo, fp);

  (void) jpeg_read_header(&cinfo, TRUE);

  if (cinfo.jpeg_color_space!=JCS_GRAYSCALE) {
	cinfo.quantize_colors = TRUE;
	cinfo.desired_number_of_colors = 128;
  }
  jpeg_start_decompress(&cinfo);

  if (cinfo.jpeg_color_space==JCS_GRAYSCALE)
  {
	gx_given_bmp_mastering((bmp_t*)jpg, cinfo.image_width, cinfo.image_height, 8*cinfo.output_components, cinfo.actual_number_of_colors);
	CreateGrayColourMap( jpg->palette ,256);
  }

  else
  {
	gx_given_bmp_mastering((bmp_t*)jpg, cinfo.image_width, cinfo.image_height, 8*cinfo.output_components, cinfo.actual_number_of_colors);
	SetPalette( jpg->palette, cinfo.actual_number_of_colors, cinfo.colormap[0], cinfo.colormap[1], cinfo.colormap[2]);
  }
  jpg->dc_type            = DCTYPE_JPG;                                         // DC 형태를 JPEG으로
  jpg->release_dc         = release_dc;                                         // 소멸자 가상 함수

  row_stride = cinfo.output_width * cinfo.output_components;

  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  IterUpset(&iter);
  while (cinfo.output_scanline < cinfo.output_height) {
	 (void) jpeg_read_scanlines(&cinfo, buffer, 1);
	 IterSetRow( &iter, buffer[0], row_stride);
	 IterPrevRow( &iter );
  }

  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  fclose(fp);
  return jpg;
}

void  gx_jpg_close( jpg_t *jpg)
//-------------------------------------------------------------------------------
// 설명: PNG 파일 사용 종료
{
   release_dc( (dc_t *)jpg);
}

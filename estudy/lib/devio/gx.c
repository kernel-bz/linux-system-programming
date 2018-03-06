/*******************************************************************************
프로젝트 : gxLib
모듈내용 : 프레임 버퍼와 스크린 DC 처리 및 gxLib 함수 사용의 최상위 함수
변경일자 : 2008-11-22
작성자   : 장길석(jwjwmx@gmail.com)
저작권   : 주석 내용을 변경하지 않는 한 무료 제공
홈페이지 : http://forum.falinux.com
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>                                                              // malloc srand
#include <string.h>                                                              // abs

#include <unistd.h>                                                              // open/close
#include <fcntl.h>                                                               // O_RDWR
#include <sys/ioctl.h>                                                           // ioctl
#include <sys/mman.h>                                                            // mmap PROT_
#include <linux/fb.h>
#include <gx.h>

frame_buffer_t    gx_fb;                                                         // 프레임 버퍼 정보
int               gx_error_code;                                                 // 에러코드

typedef unsigned short  ush;
typedef unsigned char   uch;

#define ALPHA_COMPOSITE(composite, fg, alpha, bg) {               \
    ush temp = ((ush)(fg)*(ush)(alpha) +                          \
                (ush)(bg)*(ush)(255 - (ush)(alpha)) + (ush)128);  \
    (composite) = (uch)((temp + (temp >> 8)) >> 8);               \
}


/*********************************************************************** 1 bpp */

static int b1_color( color_t color)
//-------------------------------------------------------------------------------
// 설명: 1 bit 칼라에 대한 정수 값을 구한다.
// 인수: color       red, green, blue 칼라 값
// 반환: 정수 칼라 값
{
   printf( "구현 해야 함\n");
   color = color;
   return  0;
}

static void b1_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// 설명: 스크린을 특정 칼라로 채움
{
   printf( "구현 해야 함\n");
   dc = dc;       color = color;
}

static void b1_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 점을 찍는다.
// 참고: 좌표 값을 확인하여 엉뚱한 좌표가 입력되도 실행 에러가 발생하지 않도록 한다.
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b1_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// 설명: 좌표에 대한 칼라 값을 구한다.
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b1_hline( dc_t *dc, int x1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수평선을 그린다.
{
   int      n_color;

   printf( "구현해야 함: gx.c -> b1_hline\n");
   n_color  = b1_color( color);
   dc = dc;    x1st = x1st;    x_2nd = x_2nd;    coor_y = coor_y;     color = color;
}

static void b1_vline( dc_t *dc, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수직선을 그린다.
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     y_1st = y_1st;    y_2nd = y_2nd;    color = color;
}

/*********************************************************************** 4 bpp */

static int b4_color( color_t color)
//-------------------------------------------------------------------------------
// 설명: R,G,B 값을 지정했을 때, DC에 해당하는 정수 칼라 값을 구한다.
// 인수: color    red, green, blue 값
// 반환: 정수 칼라 값
{
   printf( "구현 해야 함\n");
   color = color;
   return  0;
}

static void b4_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// 설명: 스크린을 특정 칼라로 채움
{
   printf( "구현 해야 함\n");
   dc = dc;       color = color;
}

static void b4_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 점을 찍는다.
// 참고: 좌표 값을 확인하여 엉뚱한 좌표가 입력되어도 실행 에러가 발생하지 않도록 한다.
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b4_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// 설명: 좌표에 대한 칼라 값을 구한다.
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b4_hline( dc_t *dc, int x1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수평선을 그린다.
{
   int      n_color;

   printf( "구현해야 함: gx.c -> b4_hline\n");
   n_color  = b4_color( color);
   dc       = dc;    x1st = x1st;    x_2nd = x_2nd;    coor_y = coor_y;     color = color;
}

static void b4_vline( dc_t *dc, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수직선을 그린다.
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     y_1st = y_1st;    y_2nd = y_2nd;    color = color;
}

/*********************************************************************** 8 bpp */

static int b8_color( color_t color)
//-------------------------------------------------------------------------------
// 설명: R,G,B 값을 지정했을 때, DC에 해당하는 정수 칼라 값을 구한다.
// 인수: red:0부터 255 사이의 red 값, green:green 값, blud:blue  값
// 반환: 정수 칼라 값
{
   printf( "구현 해야 함\n");
   color = color;
   return  0;
}

static void b8_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// 설명: 스크린을 특정 칼라로 채움
{
   printf( "구현 해야 함\n");
   dc = dc;       color = color;
}

static void b8_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 점을 찍는다.
// 참고: 좌표 값을 확인하여 엉뚱한 좌표가 입력되도 실행 에러가 발생하지 않도록 한다.
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b8_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// 설명: 좌표에 대한 칼라 값을 구한다.
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b8_hline( dc_t *dc, int x1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수평선을 그린다.
{
   int      n_color;

   printf( "구현해야 함: gx.c -> b8_hline\n");
   n_color  = b8_color( color);
   dc       = dc;    x1st = x1st;    x_2nd = x_2nd;    coor_y = coor_y;     color = color;
}

static void b8_vline( dc_t *dc, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수직선을 그린다.
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     y_1st = y_1st;    y_2nd = y_2nd;    color = color;
}

/********************************************************************** 16 bpp */

static int b16_color( color_t color)
//-------------------------------------------------------------------------------
// 설명: R,G,B 값을 지정했을 때, DC에 해당하는 정수 칼라 값을 구한다.
// 참고: 16 bit 환경에 따라 shift=0:5:6:5, shift=0:11:5:0
// 인수: red:0부터 255 사이의 red 값, green:green 값, blud:blue  값
// 반환: 정수 칼라 값
{
   color.red   >>= 3;                                                            // 8bit 인수 값을 칼라 bit 크기 만큼 축소
   color.green >>= 2;                                                            // 8bit 인수 값을 칼라 bit 크기 만큼 축소
   color.blue  >>= 3;                                                            // 8bit 인수 값을 칼라 bit 크기 만큼 축소

   return   ( color.red << 11) | ( color.green << 5) | color.blue;
}

static void  b16_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// 설명: 스크린을 특정 칼라로 채움
{
   int             ndx;
   unsigned short *ptr;
   int             n_color;

   n_color  = b16_color( color);
   ptr = (unsigned short *)dc->mapped;

   for ( ndx = 0; ndx < dc->dots; ndx++)
      *ptr++ = n_color;
}

static void  b16_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 점을 찍는다.
// 참고: 좌표 값을 확인하여 엉뚱한 좌표가 입력되도 실행 에러가 발생하지 않도록 한다.
{
   unsigned short *ptr;

   ptr   = (unsigned short *)dc->mapped +dc->width * coor_y + coor_x;
  *ptr   = b16_color( color);
}

static void b16_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// 설명: 좌표에 대한 칼라 값을 구한다.
// 참고: 좌표 값이 유효한지를 확인하지 않는다.
{
   unsigned short *ptr;
   unsigned short  clr_bit;

   ptr            = (unsigned short *)dc->mapped +dc->width * coor_y + coor_x;
   clr_bit        = *ptr;

   color->blue    =  (   clr_bit        & 0x1f) << 3;
   color->green   =  ( ( clr_bit >> 5 ) & 0x3f) << 2;
   color->red     =  ( ( clr_bit >> 11) & 0x1f) << 3;
}

static void  b16_hline( dc_t *dc, int x1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수평선을 그린다.
// 참고: 좌표 값을 확인하여 엉뚱한 좌표가 입력되도 실행 에러가 발생하지 않도록 한다.
{
   unsigned short *ptr;
   int            n_color;
   int            ndx;

   n_color  = b16_color( color);                                                 // DC에 해당하는 칼라 정수 값
   ptr   = (unsigned short *)dc->mapped + dc->width*coor_y +x1st;
   for ( ndx = x1st; ndx <= x_2nd; ndx++)
     *ptr++ = n_color;
}

static void  b16_vline( dc_t *dc, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수직선을 그린다.
{
   unsigned short *ptr;
   int            n_color;
   int            ndx;

   n_color  = b16_color( color);                                                 // DC에 해당하는 칼라 정수 값
   ptr   = (unsigned short *)dc->mapped + dc->width*y_1st +coor_x;
   for ( ndx = y_1st; ndx <= y_2nd; ndx++)
   {
     *ptr   = n_color;
      ptr  += dc->width;
   }
}

/********************************************************************** 24 bpp */

static int b24_color( color_t color)
//-------------------------------------------------------------------------------
// 설명: R,G,B 값을 지정했을 때, DC에 해당하는 정수 칼라 값을 구한다.
// 인수: red:0부터 255 사이의 red 값, green:green 값, blud:blue  값
// 반환: 정수 칼라 값
{
   return  ( color.red << 16) | ( color.green << 8) | color.blue;
}

static void b24_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// 설명: 스크린을 특정 칼라로 채움
// 인수: color       칼라
{
   printf( "구현 해야 함\n");
   dc = dc;    color = color;
}

static void b24_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 점을 찍는다.
// 참고: 좌표 값을 확인하여 엉뚱한 좌표가 입력되도 실행 에러가 발생하지 않도록 한다.
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b24_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// 설명: 좌표에 대한 칼라 값을 구한다.
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b24_hline( dc_t *dc, int x1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수평선을 그린다.
{
   int      n_color;

   printf( "구현해야 함: gx.c -> b24_hline\n");
   n_color  = b24_color( color);
   dc       = dc;    x1st = x1st;    x_2nd = x_2nd;    coor_y = coor_y;     color = color;
}

static void b24_vline( dc_t *dc, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수직선을 그린다.
// 인수: dc             출력 대상 DC
//       coor_x         출력 x 좌표
//       y_1st, y_2nd   수직선을 긋기 위한 y 좌표의 시작과 종료 좌표
{
   printf( "구현 해야 함\n");
   dc = dc;    coor_x = coor_x;     y_1st = y_1st;    y_2nd = y_2nd;    color = color;
}

/********************************************************************** 32 bpp */

static int b32_color( color_t color)
//-------------------------------------------------------------------------------
// 설명: R,G,B 값을 지정했을 때, DC에 해당하는 정수 칼라 값을 구한다.
// 인수: red:0부터 255 사이의 red 값, green:green 값, blud:blue  값
// 반환: 정수 칼라 값
{
   return   ( color.red << 16) | ( color.green << 8) | color.blue;
}

static void b32_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// 설명: 스크린을 특정 칼라로 채움
// 인수: color       칼라
{
   unsigned long  *ptr;
   int             n_color;
   int             ndx;

   n_color  = b32_color( color);
   ptr = (unsigned long *)dc->mapped;

   for ( ndx = 0; ndx < dc->dots; ndx++)
      *ptr++ = n_color;
}

static void b32_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 점을 찍는다.
{
   unsigned long *ptr;

   ptr   = (unsigned long *)dc->mapped +dc->width*coor_y +coor_x;
   *ptr  = b32_color( color);
}

static void b32_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// 설명: 좌표에 대한 칼라 값을 구한다.
{
   unsigned long  *ptr;
   unsigned long   clr_bit;

   ptr            = (unsigned long *)dc->mapped +dc->width*coor_y +coor_x;
   clr_bit        = *ptr;

   color->blue    =  clr_bit & 0xff;
   color->green   =  ( clr_bit >> 8 ) & 0xff;
   color->red     =  ( clr_bit >> 16) & 0xff;
}

static void b32_hline( dc_t *dc, int x1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수평선을 그린다.
{
   unsigned long *ptr;
   int            n_color;
   int            ndx;

   n_color  = b32_color( color);
   ptr      = (unsigned long *)dc->mapped + dc->width * coor_y +x1st;
   for ( ndx = x1st; ndx <= x_2nd; ndx++)
     *ptr++ = n_color;
}

static void b32_vline( dc_t *dc, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수직선을 그린다.
{
   unsigned long *ptr;
   int            n_color;
   int            ndx;

   n_color  = b32_color( color);
   ptr   = (unsigned long *)dc->mapped + dc->width * y_1st +coor_x;
   for ( ndx = y_1st; ndx <= y_2nd; ndx++)
   {
     *ptr   = n_color;
      ptr  += dc->width;
   }
}

static void draw_circle( dc_t *dc, int center_x, int center_y, int coor_x, int coor_y, color_t color, void (*fun)( dc_t *c, int, int, int, color_t))
//-------------------------------------------------------------------------------
// 설명: 원의 8 부분의 좌표를 처리
{
   int      y_dot;

   y_dot   = center_y +coor_y;
   fun( dc, center_x -coor_x, center_x +coor_x, y_dot, color);

   y_dot   = center_y -coor_y;
   fun( dc, center_x -coor_x, center_x +coor_x, y_dot, color);

   y_dot   = center_y +coor_x;
   fun( dc, center_x -coor_y, center_x +coor_y, y_dot, color);

   y_dot   = center_y -coor_x;
   fun( dc, center_x -coor_y, center_x +coor_y, y_dot, color);
}

static void circle_dot( dc_t *dc, int x1st , int x_2nd , int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 원의 테두리에 점을 찍음
{
   gx_set_pixel( dc, x1st , coor_y, color);
   gx_set_pixel( dc, x_2nd, coor_y, color);
}

static void circle( dc_t *dc, int center_x, int center_y, int radius, color_t color, void (*fun)( dc_t *dc, int, int, int, color_t) )
//-------------------------------------------------------------------------------
// 설명: 실제 원을 그린다.
// 참고: fun() 함수에 따라 내부를 채우거나 테두리만 그린다.
// 인수: dc          Device Context
//       center_x,_y 원의 중심 좌표
//       radius      원의 반지름
//       color       테두리 또는 내부 색상
//       (*fun)      테두리 또는 내부를 칠하는 함수

{
   int      coor_x;
   int      coor_y;
   int      p_value;

   if ( 0 == color.alpha)    return;

   coor_x      = 0;
   coor_y      = radius;
   p_value     = 3 - 2 * radius;
   while   ( coor_x < coor_y)
   {
      draw_circle( dc, center_x, center_y, coor_x, coor_y, color, fun);
      if ( p_value < 0)
      {
         p_value   += 4 * coor_x +6;
      }
      else
      {
         p_value   += 4 * ( coor_x -coor_y) +10;
         coor_y--;
      }
      coor_x++;
   }
   if ( coor_x == coor_y)
      draw_circle( dc, center_x, center_y, coor_x, coor_y, color, fun);
}
static void ellipse( dc_t *dc, int center_x, int center_y, int width, int height, color_t color, void (*fun)( dc_t *dc, int, int, int, color_t) )
//-------------------------------------------------------------------------------
// 설명: 타원을 그린다.
// 참고: fun() 함수에 따라 내부를 채우거나 테두리만 그린다.
// 인수: dc             Device Context
//       center_x, _y   타원의 중심 좌표
//       width          타원의 가로 폭
//       height         타원의 세로 높이
//       color          테두리 또는 내부 색상
//       (*fun)         테두리 또는 내부를 칠하는 함수
{
   int      coor_x, coor_y;
   long     onesqu_x, twosqu_x;
   long     onesqu_y, twosqu_y;
   long     delta, dx, dy;

   if ( 0 == color.alpha)    return;

   coor_x   = 0;
   coor_y   = height;
   onesqu_x = width * width;
   twosqu_x = onesqu_x << 1;
   onesqu_y = height * height;
   twosqu_y = onesqu_y << 1;

   delta    = onesqu_y - onesqu_x *height + (onesqu_x >> 2);
   dx       = 0;
   dy       = twosqu_x * height;

   while( dx < dy )
   {
      fun( dc, center_x-coor_x, center_x +coor_x, center_y +coor_y, color);
      fun( dc, center_x-coor_x, center_x +coor_x, center_y -coor_y, color);

      if( delta > 0 )
      {
         coor_y--;
         dy    -= twosqu_x;
         delta -= dy;
      }
      coor_x++;
      dx       += twosqu_y;
      delta    += onesqu_y + dx;
   }

   delta += ( 3*(onesqu_x - onesqu_y)/2 - (dx+dy)/2 );

   while( coor_y >= 0 )
   {
      fun( dc, center_x -coor_x, center_x +coor_x, center_y +coor_y, color);
      fun( dc, center_x -coor_x, center_x +coor_x, center_y -coor_y, color);

      if( delta < 0 )
      {
         coor_x++;
         dx      += twosqu_y;
         delta   += dx;
      }
      coor_y--;
      dy      -= twosqu_x;
      delta   += onesqu_x - dy;
   }
}

static void release_screen_dc( dc_t *dc)
//-------------------------------------------------------------------------------
// 설명: DC 자원을 반환
{
   if ( NULL != dc)     free( dc);
   dc = NULL;
}

static void release_buffer_dc( dc_t *dc)
//-------------------------------------------------------------------------------
// 설명: DC 자원을 반환
{
   if ( NULL != dc)
   {
      if ( NULL != dc->mapped)
      {
         free( dc->mapped);
      }
      free( dc);
   }
   dc = NULL;
}

static void set_virtual_func( dc_t *dc, int colors)
//-------------------------------------------------------------------------------
// 설명: 칼라 깊이에 맞추어 가상 함수를 지정
// 참고: 칼라 깊이에 해당하는 가상함수가 없다면 dc는 소멸된다.
// 인수: dc          출력 DC
//       colors      칼라 깊이
{
      switch( colors)                                                            // 칼라 깊이에 따라 포인터 함수를 지정
      {
      case 1   :
                     dc->clear      = b1_clear;
                     dc->get_pixel  = b1_get_pixel;
                     dc->set_pixel  = b1_set_pixel;
                     dc->hline      = b1_hline;
                     dc->vline      = b1_vline;
                     break;
      case 4   :
                     dc->clear      = b4_clear;
                     dc->get_pixel  = b4_get_pixel;
                     dc->set_pixel  = b4_set_pixel;
                     dc->hline      = b4_hline;
                     dc->vline      = b4_vline;
                     break;
      case 8   :
                     dc->clear      = b8_clear;
                     dc->get_pixel  = b8_get_pixel;
                     dc->set_pixel  = b8_set_pixel;
                     dc->hline      = b8_hline;
                     dc->vline      = b8_vline;
                     break;
      case 15  :                                                                 // 15bit
      case 16  :                                                                 // 16bit 칼라에 대해서는 같은 함수 사용
                     dc->clear      = b16_clear;
                     dc->get_pixel  = b16_get_pixel;
                     dc->set_pixel  = b16_set_pixel;
                     dc->hline      = b16_hline;
                     dc->vline      = b16_vline;
                     break;
      case 24  :
                     dc->clear      = b24_clear;
                     dc->get_pixel  = b24_get_pixel;
                     dc->set_pixel  = b24_set_pixel;
                     dc->hline      = b24_hline;
                     dc->vline      = b24_vline;
                     break;
      case 32  :
                     dc->clear      = b32_clear;
                     dc->get_pixel  = b32_get_pixel;
                     dc->set_pixel  = b32_set_pixel;
                     dc->hline      = b32_hline;
                     dc->vline      = b32_vline;
                     break;
      default  :
                     dc->release_dc( dc);
      }
}

static void  byte_bitblt( dc_t *dc_dest, int dest_x, int dest_y, dc_t *dc_sour, int sour_x, int sour_y, int sour_w, int sour_h, int bytes_per_pixel)
//-------------------------------------------------------------------------------
// 설명: DC영역에 다른 DC를 복사한다.
// 인수: dc_dest           : 출력 목적지 Device Context Handle
//       dest_x, dest_y    : 출력 x, y 좌표
//       dc_sour           : 출력할 대상 Device Context Handle
//       sour_x, sour_y    : dc_sour에서 복사 시작 좌표
//       sour_w, sour_h    : dc_sour에서 복사 종료 좌표
//       bytes_per_pixel    : pixel 당 바이트 개수
{
   int   bytes_per_line = sour_w *bytes_per_pixel;                                // 복사 영역의 라인당 바이트 수
   char *pdest          = (char*)dc_dest->mapped +dc_dest->bytes_per_line*dest_y +dest_x*bytes_per_pixel;
   char *psour          = (char*)dc_sour->mapped +dc_sour->bytes_per_line*sour_y +sour_x*bytes_per_pixel;

   while( 0 < sour_h)
   {
      memcpy( pdest, psour, bytes_per_line);
      pdest += dc_dest->bytes_per_line;
      psour += dc_sour->bytes_per_line;
      sour_h--;
   }
}

///////////////////////////////////////////////////////////////////////////////// 외부에 제공되는 함수

color_t  gx_color( int red, int green, int blue, int alpha)
//-------------------------------------------------------------------------------
// 설명: 칼라 값을 color_t로 구한다.
{
   color_t  color;

   color.red      = red;
   color.green    = green;
   color.blue     = blue;
   color.alpha    = alpha;
   return color;
}

void gx_set_alpha( color_t *color, int alpha)
//-------------------------------------------------------------------------------
// 설명: 칼라 값의 투명도를 지정
// 참고: 최소 0값부터 최대 255까지 확인하여 대입
{
   if       (   0 > alpha)       color->alpha   = 0;
   else if  ( 255 < alpha)       color->alpha   = 255;
   else                          color->alpha   = alpha;
}

void  gx_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// 설명: dc에 인수로 받은 색상으로 전체 칠함
// 인수: dc    Device Context
//       color 칠할 색상
{
   dc->clear( dc, color);
}

void  gx_pen_color( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// 설명: dc의 Pen 색상 변경
// 인수: dc       Device Context
//       color    Pen 색상
{
   dc->pen_color = color;
}

void  gx_brush_color( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// 설명: dc의 Brush 색상 변경
// 인수: dc       Device Context
//       color    브러쉬 색상
{
   dc->brush_color = color;
}

void  gx_get_pixel( dc_t  *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// 설명: 좌표에 대한 칼라 값을 구합니다.
// 참고: 좌표가 유효한지를 확인한다.
// 인수: dc                Device Context
//       coor_x, coor_y    색상을 구할 좌표
//       color             색상을 받을 레퍼런스 변수
{
   if ( 0 > coor_x || dc->width  <= coor_x)     return;
   if ( 0 > coor_y || dc->height <= coor_y)     return;

   dc->get_pixel( dc, coor_x, coor_y, color);
}

void  gx_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: dc에 점을 찍습니다.
// 참고: 좌표가 유효한지를 확인한다.
// 인수: dc                Device Context
//       coor_x, coor_y    색상을 변경할 좌표
//       color             색상 값
{
   if ( 0 > coor_x || dc->width  <= coor_x)     return;
   if ( 0 > coor_y || dc->height <= coor_y)     return;

   dc->set_pixel( dc, coor_x, coor_y, color);
}

void  gx_move_to( dc_t *dc, int coor_x, int coor_y)
//-------------------------------------------------------------------------------
// 설명: dc의 좌표를 이동
// 인수: dc                Device Context
//       coor_x, coor_y    좌표
{
   dc->coor_x  = coor_x;
   dc->coor_y  = coor_y;
}

void  gx_line_to( dc_t *dc, int coor_x, int coor_y)
//-------------------------------------------------------------------------------
// 설명: dc의 좌표와 인수로 받은 좌표까지 선을 그림
// 참고: 인수로 받은 좌표를 dc 좌표로 대입
{
   int      rx, ry;
   int      dx, dy;
   int      inc_x;
   int      inc_y;
   int      offset;
   color_t  color;

   rx = dc->coor_x;                                                              // dc 좌표 값
   ry = dc->coor_y;

   dc->coor_x  = coor_x;                                                         // dc 좌표를 인수로 받은 값으로 대입
   dc->coor_y  = coor_y;

   dx = coor_x -rx;                                                              // x 좌표에 대한 delta 값
   if ( 0 == dx)                                                                 // 수식선을 그린다면
   {
      gx_vline( dc, rx, ry, coor_y, dc->pen_color);
      return;
   }
   else if ( 0 < dx)    inc_x = 1;                                               // x 좌표가 우측으로 증가하는 직선이라면
   else                                                                          // x 좌표가 좌측으로 감소하는 직선이라면
   {
      dx    = -dx;
      inc_x = -1;
   }

   dy = coor_y -ry;                                                              // y 좌표에 대한 delta 값
   if ( 0 == dy)                                                                 // 수평선을 그린다면
   {
      gx_hline( dc, rx, coor_x, ry, dc->pen_color);
      return;
   }
   else if ( 0 < dy)    inc_y = 1;                                               // y 좌료가 밑으로 증가하는 직선이라면
   else                                                                          // y 좌료가 위로   감소하는 직선이라면
   {
      dy    = -dy;
      inc_y = -1;
   }

   color = dc->pen_color;                                                        // 선의 색깍을 구함
   gx_set_pixel( dc, rx, ry, color);                                             // 첫번째 좌표를 찍음
   if ( dy <= dx)                                                                // x 좌료 변도이 더 큼, x 좌표는 1씩 변화
   {
      offset   = dx / 2;

      for (; rx != coor_x; rx += inc_x)
      {
         offset   += dy;
         if ( dx <= offset)
         {
            offset   -= dx;
            ry       += inc_y;
         }
         gx_set_pixel( dc, rx, ry, color);
      }
   }
   else                                                                          // y 좌료 변동이 더 큼, y 좌표는 1씩 변화
   {
      offset   = dy /2;

      for (; ry != coor_y; ry += inc_y)
      {
         offset   += dx;
         if ( dy <= offset)
         {
            offset   -= dy;
            rx       += inc_x;
         }
         gx_set_pixel( dc, rx, ry, color);
      }
   }
}

void gx_hline( dc_t *dc, int x_1st  , int x_2nd , int coor_y, color_t color)
//-------------------------------------------------------------------------------
// 설명: 수평선 긋기
{
   int             ndx;

   if ( 0 > coor_y || dc->height <= coor_y)  return;                             // y 좌표가 스크린 영역을 벗어나면 복귀
   if ( x_2nd < x_1st)                                                           // x_1st 가 작은 좌료를 갖도록 swap
   {
      ndx      = x_1st;
      x_1st    = x_2nd;
      x_2nd    = ndx;
   }

   if       ( 0         >  x_1st)  x_1st   = 0;                                  // 좌측 화면 밖의 좌표이면 0부터 그린다.
   else if  ( dc->width <= x_1st)  x_1st   = dc->width-1;                        // 우측 화면 밖의 좌표이면 우측 화면까지만 그린다.

   if       ( 0         >  x_2nd)  x_2nd   = 0;
   else if  ( dc->width <= x_2nd)  x_2nd   = dc->width-1;

   dc->hline( dc, x_1st, x_2nd, coor_y, color);
}

void gx_vline( dc_t *dc, int coor_x, int y_1st , int y_2nd , color_t color)
//-------------------------------------------------------------------------------
// 설명: 수직선 긋기
{
   int             ndx;

   if ( 0 > coor_x || dc->width <= coor_x)   return;                             // x 좌료가 영역을 벗어나면 복귀

   if ( y_2nd < y_1st)                                                           // y_1st 좌표가 작은 값을 갖도록 swap
   {
      ndx     = y_1st;
      y_1st    = y_2nd;
      y_2nd    = ndx;
   }

   if       ( 0           >  y_1st)  y_1st   = 0;                                // 화면 상단 밖의 좌표이면 0부터 그린다.
   else if  ( dc->height  <= y_1st)  y_1st   = dc->height-1;                    // 화면 하단 밖의 좌표이면 우측 화면까지만 그린다.

   if       ( 0           >  y_2nd)  y_2nd   = 0;
   else if  ( dc->height  <= y_2nd)  y_2nd   = dc->height-1;

   dc->vline( dc, coor_x, y_1st, y_2nd, color);
}

void  gx_line( dc_t *dc, int x1, int y1, int x2, int y2)
//-------------------------------------------------------------------------------
// 설명: 두 개의 좌표를 잇는 선을 그린다.
{
   gx_move_to( dc, x1, y1);
   gx_line_to( dc, x2, y2);
}

void  gx_rectangle( dc_t *dc, int x1, int y1, int x2, int y2)
//-------------------------------------------------------------------------------
// 설명: 두 개의 좌표를 이용하여 테두리와 내부가 채워진 사각형을 그린다.
{
   int      tmp;

   if ( x2 < x1)
   {
      tmp   = x2;
      x2    = x1;
      x1    = tmp;
   }

   if ( y2 < y1)
   {
      tmp   = y2;
      y2    = y1;
      y1    = tmp;
   }

   if ( 0 != dc->brush_color.alpha)
   {
      tmp   = y1;
      for ( ; y1 <= y2; y1++)
         gx_hline( dc, x1, x2, y1, dc->brush_color);
      y1    = tmp;
   }

   if ( 0 != dc->pen_color.alpha)
   {
      gx_hline( dc, x1, x2, y1, dc->pen_color);
      gx_hline( dc, x1, x2, y2, dc->pen_color);
      gx_vline( dc, x1, y1, y2, dc->pen_color);
      gx_vline( dc, x2, y1, y2, dc->pen_color);
   }
}

//내부를 채우지 않고 사각형 그린다.
//-------------------------------------------------------------------------------
void gx_box (dc_t *dc, int x1, int y1, int x2, int y2)
{
	gx_hline( dc, x1, x2, y1, dc->pen_color);
    gx_hline( dc, x1, x2, y2, dc->pen_color);
    gx_vline( dc, x1, y1, y2, dc->pen_color);
    gx_vline( dc, x2, y1, y2, dc->pen_color);
}

void gx_circle( dc_t *dc, int center_x, int center_y, int radius)
//-------------------------------------------------------------------------------
// 설명: 원을 그린다.
// 참고: 테두리와 내부를 채운다.
// 인수: dc          Device Context
//       center_x,_y 원의 중심 좌표
//       radius      원의 반지름
{
   if ( 0 == radius) return;

   circle( dc, center_x, center_y, radius, dc->brush_color, gx_hline);
   circle( dc, center_x, center_y, radius, dc->pen_color  , circle_dot);
}

void  gx_ellipse(  dc_t *dc, int center_x, int center_y, int width, int height)
//-------------------------------------------------------------------------------
// 설명: 타원을 그린다.
// 참고: 테두리와 내부를 채운다.
// 인수: dc          Device Context
//       center_x,_y 타원의 중심 좌표
//       width       타원의 폭
//       height      타원의 높이
{
   if ( ( 0 == width) || ( 0 == height) ) return;

   ellipse( dc, center_x, center_y, width, height, dc->brush_color, gx_hline);
   ellipse( dc, center_x, center_y, width, height, dc->pen_color  , circle_dot);
}

void gx_to_screen_dc( dc_t *dc_screen, dc_t *dc_buffer)
//-------------------------------------------------------------------------------
// 설명: ScreenDC 로 BufferDC 전체를 복사한다.
// 인수  dc_screen      스크린 DC
//       dc_buffer      버퍼 DC
{
   memcpy( dc_screen->mapped, dc_buffer->mapped, dc_screen->bytes);
}

void  gx_bitblt(  dc_t *dc_dest, int dest_x, int dest_y, dc_t *dc_sour, int sour_x, int sour_y, int sour_w, int sour_h)
//-------------------------------------------------------------------------------
// 설명: DC영역에 다른 DC를 복사한다.
// 인수: dc_dest         : 출력 목적지 Device Context Handle
//       dest_x, dest_y  : 출력 x, y 좌표
//       dc_sour         : 출력할 대상 Device Context Handle
//       sour_x, sour_y  : dc_sour에서 복사 시작 좌표
//       sour_w, sour_h  : dc_sour에서 복사 종료 좌표
{
   int      coor_x;
   int      coor_y;
   color_t  color_sour;
   color_t  color_dest;
   int      ncheck;

   if ( 0 > dest_x)   dest_x = 0;
   else if ( dc_dest->width <= dest_x)   dest_x = dc_dest->width-1;

   if ( 0 > dest_y)   dest_y = 0;
   else if ( dc_dest->height <= dest_y)  dest_y = dc_dest->height-1;

   if ( 0 > sour_x)   sour_x = 0;
   else if ( dc_sour->width <= sour_x)   sour_x = dc_sour->width-1;

   if ( 0 > sour_y)   sour_y = 0;
   else if ( dc_sour->height <= sour_y)  sour_y = dc_sour->height-1;

   ncheck   = dest_x + sour_w;
   if ( dc_dest->width < ncheck)   sour_w = dc_dest->width -dest_x;

   ncheck   = sour_x + sour_w;
   if ( dc_sour->width < ncheck)   sour_w = dc_sour->width -sour_x;

   ncheck   = dest_y + sour_h;
   if ( dc_dest->height < ncheck)  sour_h = dc_dest->height -dest_y;

   ncheck   = sour_y + sour_h;
   if ( dc_sour->height < ncheck)  sour_h = dc_sour->height -sour_y;

   if ( ( DCTYPE_SCREEN == dc_sour->dc_type) && ( DCTYPE_SCREEN == dc_dest->dc_type) ) // 두 개의 DC가 모두 Screen Type 일 때, memcpy()를 사용한 빠른 복사 사용
   {
      switch( dc_dest->colors)
      {
      case  8  :
      case  15 :
      case  16 :
      case  24 :
      case  32 :  byte_bitblt( dc_dest, dest_x, dest_y, dc_sour, sour_x, sour_y, sour_w, sour_h, dc_sour->bits_per_pixel / 8);
                  break;
      default  :  printf( "8 bit 이하의 byte_bitblt()를 구현해야 합니다. \n");
      }
   }
   else if ( DCTYPE_PNG == dc_sour->dc_type)                                     // 원본이 PNG 파일이면 투명 영역을 처리한다.
   {
      if ( ( DCTYPE_PNG == dc_dest->dc_type) && ( dc_dest->bits_per_pixel == dc_sour->bits_per_pixel))
      {
         byte_bitblt( dc_dest, dest_x, dest_y, dc_sour, sour_x, sour_y, sour_w, sour_h, dc_sour->bits_per_pixel / 8);
      }
      else
      {
         for ( coor_y = 0; coor_y < sour_h; coor_y++)
         {
            for ( coor_x = 0; coor_x < sour_w; coor_x++)
            {
               dc_sour->get_pixel( dc_sour, coor_x+sour_x, coor_y+sour_y, &color_sour);
               if ( 0 < color_sour.alpha)
               {
                  if ( 255 > color_sour.alpha)
                  {
                     dc_dest->get_pixel( dc_dest, coor_x+dest_x, coor_y+dest_y, &color_dest);
                     ALPHA_COMPOSITE( color_sour.red  , color_sour.red  , color_sour.alpha, color_dest.red  );
                     ALPHA_COMPOSITE( color_sour.green, color_sour.green, color_sour.alpha, color_dest.green);
                     ALPHA_COMPOSITE( color_sour.blue , color_sour.blue , color_sour.alpha, color_dest.blue );
                  } // if
                  dc_dest->set_pixel( dc_dest, coor_x+dest_x, coor_y+dest_y, color_sour);
               } // if
            } // for
         } // for
      } // else
   }
   else                                                                          // 투명 영역이 없는 소스라면 바로 복사
   {
      for ( coor_y = 0; coor_y < sour_h; coor_y++)
      {
         for ( coor_x = 0; coor_x < sour_w; coor_x++)
         {
            dc_sour->get_pixel( dc_sour, coor_x+sour_x, coor_y+sour_y, &color_sour);
            dc_dest->set_pixel( dc_dest, coor_x+dest_x, coor_y+dest_y,  color_sour);
         }
      }
   }
}

void  gx_release_dc( dc_t *dc)
//-------------------------------------------------------------------------------
// 설명: Device Context 자원을 반환
{
   dc->release_dc( dc);
}

dc_t *gx_get_buffer_dc( int width, int height)
//-------------------------------------------------------------------------------
// 설명: 화면 출력을 빠르게 하기 위한 버퍼 DC를 구함.
// 참고: BitBlt를 빠르게 처리하기 위해 DC Type를 DCTYPE_SCREEN으로 지정한다.
// 인수: width       버퍼의 폭
//       height      버퍼의 높이
// 반환: 버퍼 Device Context 핸들
{
   int   szStruct;
   dc_t  *dc;

   szStruct = sizeof( dc_t);
   dc = malloc( szStruct);                                                       // dc_t 구조체 메모리 할당
   if ( NULL != dc)                                                              // 메모리 구하기에 성공함녀
   {
      dc->dc_type        = DCTYPE_SCREEN;                                        // DC 형태를 SCREEN으로, 이후 SCREEN과 같은 처리를 받음

      if ( 0 > width ) width  = gx_fb.width;                                     // 크기가 정해있지 않다면 프레임버퍼 크기로 지정
      if ( 0 > height) height = gx_fb.height;

      dc->width            = width;                                              // 폭
      dc->height           = height;                                             // 높이
      dc->colors           = gx_fb.colors;                                       // 칼라 깊이
      dc->dots             = width *height;                                      // 도트 갯수
      dc->mapped           = NULL;                                               // 메모리 포인터, set_virtual_func()에서 dc가 Release 될 수 있으므로 반드시 NULL로 초기화
      dc->coor_x           = 0;                                                  // LineTo를 위한 좌표를 0으로 초기화
      dc->coor_y           = 0;
      dc->pen_color        = gx_color( 255, 255, 255, 255);                      // 기본 펜 색상은 백색
      dc->brush_color      = gx_color(   0,   0,   0, 255);                      // 기본 브러쉬 색상은 검정
      dc->release_dc       = release_buffer_dc;

      dc->bits_per_pixel= gx_fb.bits_per_pixel;                                  // 비트당 픽셀 개수

      switch( gx_fb.colors)
      {
      case 1   :  dc->bytes_per_line  = width / 8;
                  if ( 0 != width % 8)    dc->bytes_per_line++;
                  break;
      case 4   :  dc->bytes_per_line  = width / 4;
                  if ( 0 != width % 4)    dc->bytes_per_line++;
                  break;
      case 15  :
      case 16  :  dc->bytes_per_line  = width * 2; break;
      case 24  :  dc->bytes_per_line  = width * 3; break;
      case 32  :  dc->bytes_per_line  = width * 4; break;
      default  :  dc->bytes_per_line  = width    ; break;                        // 8 bit 또는 정규 해상도가 아닐 경우 폭의 바이트 수 만큼 지정
      }
      dc->bytes   = dc->bytes_per_line *height;
      dc->mapped  = malloc( dc->bytes);
      set_virtual_func( dc, gx_fb.colors);                                       // 칼라 깊이에 맞추어 가상 함수를 지정, 칼라 깊이에 해당하는 가상함수가 없다면 dc는 소멸된다.
   }
   else
   {
      gx_error_code  = GXERR_OUT_OF_MEMORY;
   }
   return dc;
}

dc_t *gx_get_screen_dc( void)
//-------------------------------------------------------------------------------
// 설명: 화면에 그래픽을 출력하기 위한 Device Context를 구한다.
// 반환: 정상 -> dc_t *, 오류 -> NULL
// 참고: 프레임 버퍼에서 화면에 대한 모든 정보를 구한다.
{
   int   szStruct;
   dc_t  *dc;

   szStruct = sizeof( dc_t);
   dc = malloc( szStruct);                                                       // dc_t 구조체 메모리 할당
   if ( NULL != dc)                                                              // 메모리 구하기에 성공함녀
   {
      dc->dc_type             = DCTYPE_SCREEN;                                   // DC 형태를 SCREEN으로
      dc->width               = gx_fb.width;                                     // 화면 폭
      dc->height              = gx_fb.height;                                    // 화면 높이
      dc->dots                = gx_fb.dots;                                      // 화면 전체 도트 갯수
      dc->bytes               = gx_fb.bytes;                                     // 화면에 대한 메모리 전체 크기
      dc->colors              = gx_fb.colors;                                    // 칼라 깊이
      dc->bytes_per_line      = gx_fb.bytes_per_line;                            // 라인당 바이트 개수
      dc->bits_per_pixel      = gx_fb.bits_per_pixel;                            // 비트당 픽셀 개수

      dc->mapped              = gx_fb.mapped;                                    // 화면에 대한 메모리 시작 주소
      dc->coor_x              = 0;                                               // LineTo를 위한 좌표를 0으로 초기화
      dc->coor_y              = 0;
      dc->pen_color           = gx_color( 255, 255, 255, 255);                   // 기본 펜 색상은 백색
      dc->brush_color         = gx_color(   0,   0,   0, 255);                   // 기본 브러쉬 색상은 검정
      dc->release_dc          = release_screen_dc;

      set_virtual_func( dc, gx_fb.colors);                                       // 칼라 깊이에 맞추어 가상 함수를 지정, 칼라 깊이에 해당하는 가상함수가 없다면 dc는 소멸된다.
   }
   else
   {
      gx_error_code  = GXERR_OUT_OF_MEMORY;
   }
   return dc;
}

char  *gx_error_string( void)
//-------------------------------------------------------------------------------
// 설명: gx_error_code에 대한 에러 설명 문자열을 반환
{
   char *error_string[] ={ "에러 없음",                                          // GXERR_NONE
                           "장치가 없음",                                        // GXERR_NO_DEVICE
                           "접근 권한이 없음",                                   // GXERR_ACCESS_DEVICE
                           "FBIOGET_VSCREENINFO를 구하지 못함",                  // GXERR_VSCREEN_INFO
                           "FBIOGET_FSCREENINFO를 구하지 못함",                  // GXERR_FSCREEN_INFO
                           "프레임 버퍼 메모리 매핑에 실패",                     // GXERR_MEMORY_MAPPING
                           "이미지 파일이 없음",                                 // GXERR_NOFILE
                           "이미지 헤더 정보 이상",                              // GXERR_HEADER_INFO
                           "이미지 데이터 읽기 이상",                            // GXERR_READ_FILE
                           "팔레트 정보 이상",                                   // GXERR_PALETTE_INFO
                           "칼라 깊이가 표준이 아님",                            // GXERR_COLOR_DEPTH
                           "폰트파일이 없거나 사용할 수 없음",                   // GXERR_NO_FONT_FILE
                           "시그네쳐 에러",                                      // GXERR_SIGNATURE
                           "메모리 부족",                                        // GXERR_OUT_OF_MEMORY
                           "처리 중 에러 발생"                                   // GXERR_PROCESSING
                        };
   return( error_string[gx_error_code]);
}

int gx_print_error( char *remark)
//-------------------------------------------------------------------------------
// 설명: gx_error_code에 대한 에러 설명 문자열을 출력
// 인수: remark      에러 설명 문자열 끝에 첨부하여 출력할 문자열
{
   printf( "[gx error:%d]%s %s\n", gx_error_code, gx_error_string(), remark);
   return gx_error_code;
}

void  gx_close( void)
//-------------------------------------------------------------------------------
// 설명: 그래픽 라이브러리 종료
// 참고: 메모리 매핑 포인터와 프레임 버퍼 디스크립터를 소멸하고 변수를 초기화 한다.
{
   if ( 0 <= gx_fb.mapped)                                                       // 메모리가 메핑 되어 있다면
   {
      munmap( gx_fb.mapped, gx_fb.bytes);                                        // 메모리 메핑 자원 반환
      gx_fb.mapped = MAP_FAILED;
   }
   if ( 0 <= gx_fb.fd)                                                           // 파일 디스크립터가 만들어 졌다면
   {
      close( gx_fb.fd);                                                          // 파일 디스크립터 자원 반환
      gx_fb.fd = -1;
   }
}

int   gx_init( char *dev_name)                                                   // 장치 이름
//-------------------------------------------------------------------------------
// 설명: 그래픽 라이브러리를 초기화
// 반환: 0 = 에러 없음, 0 > 에러 발생
{
   struct   fb_var_screeninfo  fbvar;                                            // 화면 정보를 구하기 위해
   struct   fb_fix_screeninfo  fbfix;

   gx_fb.fd          = -1;                                                       // 초기값을 대입
   gx_fb.mapped      = MAP_FAILED;                                               // 초기값을 대입

   if ( access( dev_name, F_OK))
   {
      gx_error_code  = GXERR_NO_DEVICE;
      return GXERR_NO_DEVICE;
   }
   if ( 0 >  ( gx_fb.fd = open( dev_name, O_RDWR))   )
   {
      gx_error_code  = GXERR_ACCESS_DEVICE;
      return GXERR_ACCESS_DEVICE;
   }
   if ( ioctl( gx_fb.fd, FBIOGET_VSCREENINFO, &fbvar))
   {
      gx_close();
      gx_error_code  = GXERR_VSCREEN_INFO;
      return GXERR_VSCREEN_INFO;
   }
   if ( ioctl( gx_fb.fd, FBIOGET_FSCREENINFO, &fbfix))
   {
      gx_close();
      gx_error_code  = GXERR_FSCREEN_INFO;
      return GXERR_FSCREEN_INFO;
   }
   gx_fb.width        = fbvar.xres;                                              // 스크린의 픽셀 폭
   gx_fb.height       = fbvar.yres;                                              // 스크린의 픽셀 높이
   gx_fb.dots         = gx_fb.width * gx_fb.height;                              // 스크린 도트 개수
   gx_fb.bits_per_pixel = fbvar.bits_per_pixel;                                  // 스크린의 칼라 깊이
   gx_fb.bytes_per_line = fbfix.line_length;                                     // 한개 라인 당 바이트 개수

   gx_fb.bytes        = gx_fb.bytes_per_line *gx_fb.height;                      // 스크린의 총 메모리 바이트 수
   gx_fb.mapped       = ( void *)mmap( 0,
                                      gx_fb.bytes,                               // 메모리 전체 크기
                                      PROT_READ|PROT_WRITE,
                                      MAP_SHARED,
                                      gx_fb.fd,
                                      0);
   switch( gx_fb.bits_per_pixel)                                                 // 칼라 깊이 정보를 구함
   {
   case 16  :  gx_fb.colors   =   fbvar.red.length                               // 15bit 칼라일 경우를 위해 직접 칼라 bit 값을 구한다.
                                 +fbvar.green.length                             // 15bit일 경우 15값이 대입
                                 +fbvar.blue.length;                             // 16bit일 경우 16값이 대입
               break;
   default  :  gx_fb.colors   =  gx_fb.bits_per_pixel;
   }
   if ( 0 > gx_fb.mapped)
   {
      gx_close();
      gx_error_code  = GXERR_MEMORY_MAPPING;
      return   GXERR_MEMORY_MAPPING;
   }
   gx_error_code  = GXERR_NONE;
   return   GXERR_NONE;
}

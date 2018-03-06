/*******************************************************************************
������Ʈ : gxLib
��⳻�� : ������ ���ۿ� ��ũ�� DC ó�� �� gxLib �Լ� ����� �ֻ��� �Լ�
�������� : 2008-11-22
�ۼ���   : ��漮(jwjwmx@gmail.com)
���۱�   : �ּ� ������ �������� �ʴ� �� ���� ����
Ȩ������ : http://forum.falinux.com
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

frame_buffer_t    gx_fb;                                                         // ������ ���� ����
int               gx_error_code;                                                 // �����ڵ�

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
// ����: 1 bit Į�� ���� ���� ���� ���Ѵ�.
// �μ�: color       red, green, blue Į�� ��
// ��ȯ: ���� Į�� ��
{
   printf( "���� �ؾ� ��\n");
   color = color;
   return  0;
}

static void b1_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// ����: ��ũ���� Ư�� Į��� ä��
{
   printf( "���� �ؾ� ��\n");
   dc = dc;       color = color;
}

static void b1_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ���� ��´�.
// ����: ��ǥ ���� Ȯ���Ͽ� ������ ��ǥ�� �Էµǵ� ���� ������ �߻����� �ʵ��� �Ѵ�.
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b1_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// ����: ��ǥ�� ���� Į�� ���� ���Ѵ�.
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b1_hline( dc_t *dc, int x1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
{
   int      n_color;

   printf( "�����ؾ� ��: gx.c -> b1_hline\n");
   n_color  = b1_color( color);
   dc = dc;    x1st = x1st;    x_2nd = x_2nd;    coor_y = coor_y;     color = color;
}

static void b1_vline( dc_t *dc, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     y_1st = y_1st;    y_2nd = y_2nd;    color = color;
}

/*********************************************************************** 4 bpp */

static int b4_color( color_t color)
//-------------------------------------------------------------------------------
// ����: R,G,B ���� �������� ��, DC�� �ش��ϴ� ���� Į�� ���� ���Ѵ�.
// �μ�: color    red, green, blue ��
// ��ȯ: ���� Į�� ��
{
   printf( "���� �ؾ� ��\n");
   color = color;
   return  0;
}

static void b4_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// ����: ��ũ���� Ư�� Į��� ä��
{
   printf( "���� �ؾ� ��\n");
   dc = dc;       color = color;
}

static void b4_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ���� ��´�.
// ����: ��ǥ ���� Ȯ���Ͽ� ������ ��ǥ�� �ԷµǾ ���� ������ �߻����� �ʵ��� �Ѵ�.
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b4_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// ����: ��ǥ�� ���� Į�� ���� ���Ѵ�.
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b4_hline( dc_t *dc, int x1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
{
   int      n_color;

   printf( "�����ؾ� ��: gx.c -> b4_hline\n");
   n_color  = b4_color( color);
   dc       = dc;    x1st = x1st;    x_2nd = x_2nd;    coor_y = coor_y;     color = color;
}

static void b4_vline( dc_t *dc, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     y_1st = y_1st;    y_2nd = y_2nd;    color = color;
}

/*********************************************************************** 8 bpp */

static int b8_color( color_t color)
//-------------------------------------------------------------------------------
// ����: R,G,B ���� �������� ��, DC�� �ش��ϴ� ���� Į�� ���� ���Ѵ�.
// �μ�: red:0���� 255 ������ red ��, green:green ��, blud:blue  ��
// ��ȯ: ���� Į�� ��
{
   printf( "���� �ؾ� ��\n");
   color = color;
   return  0;
}

static void b8_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// ����: ��ũ���� Ư�� Į��� ä��
{
   printf( "���� �ؾ� ��\n");
   dc = dc;       color = color;
}

static void b8_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ���� ��´�.
// ����: ��ǥ ���� Ȯ���Ͽ� ������ ��ǥ�� �Էµǵ� ���� ������ �߻����� �ʵ��� �Ѵ�.
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b8_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// ����: ��ǥ�� ���� Į�� ���� ���Ѵ�.
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b8_hline( dc_t *dc, int x1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
{
   int      n_color;

   printf( "�����ؾ� ��: gx.c -> b8_hline\n");
   n_color  = b8_color( color);
   dc       = dc;    x1st = x1st;    x_2nd = x_2nd;    coor_y = coor_y;     color = color;
}

static void b8_vline( dc_t *dc, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     y_1st = y_1st;    y_2nd = y_2nd;    color = color;
}

/********************************************************************** 16 bpp */

static int b16_color( color_t color)
//-------------------------------------------------------------------------------
// ����: R,G,B ���� �������� ��, DC�� �ش��ϴ� ���� Į�� ���� ���Ѵ�.
// ����: 16 bit ȯ�濡 ���� shift=0:5:6:5, shift=0:11:5:0
// �μ�: red:0���� 255 ������ red ��, green:green ��, blud:blue  ��
// ��ȯ: ���� Į�� ��
{
   color.red   >>= 3;                                                            // 8bit �μ� ���� Į�� bit ũ�� ��ŭ ���
   color.green >>= 2;                                                            // 8bit �μ� ���� Į�� bit ũ�� ��ŭ ���
   color.blue  >>= 3;                                                            // 8bit �μ� ���� Į�� bit ũ�� ��ŭ ���

   return   ( color.red << 11) | ( color.green << 5) | color.blue;
}

static void  b16_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// ����: ��ũ���� Ư�� Į��� ä��
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
// ����: ���� ��´�.
// ����: ��ǥ ���� Ȯ���Ͽ� ������ ��ǥ�� �Էµǵ� ���� ������ �߻����� �ʵ��� �Ѵ�.
{
   unsigned short *ptr;

   ptr   = (unsigned short *)dc->mapped +dc->width * coor_y + coor_x;
  *ptr   = b16_color( color);
}

static void b16_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// ����: ��ǥ�� ���� Į�� ���� ���Ѵ�.
// ����: ��ǥ ���� ��ȿ������ Ȯ������ �ʴ´�.
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
// ����: ������ �׸���.
// ����: ��ǥ ���� Ȯ���Ͽ� ������ ��ǥ�� �Էµǵ� ���� ������ �߻����� �ʵ��� �Ѵ�.
{
   unsigned short *ptr;
   int            n_color;
   int            ndx;

   n_color  = b16_color( color);                                                 // DC�� �ش��ϴ� Į�� ���� ��
   ptr   = (unsigned short *)dc->mapped + dc->width*coor_y +x1st;
   for ( ndx = x1st; ndx <= x_2nd; ndx++)
     *ptr++ = n_color;
}

static void  b16_vline( dc_t *dc, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
{
   unsigned short *ptr;
   int            n_color;
   int            ndx;

   n_color  = b16_color( color);                                                 // DC�� �ش��ϴ� Į�� ���� ��
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
// ����: R,G,B ���� �������� ��, DC�� �ش��ϴ� ���� Į�� ���� ���Ѵ�.
// �μ�: red:0���� 255 ������ red ��, green:green ��, blud:blue  ��
// ��ȯ: ���� Į�� ��
{
   return  ( color.red << 16) | ( color.green << 8) | color.blue;
}

static void b24_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// ����: ��ũ���� Ư�� Į��� ä��
// �μ�: color       Į��
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    color = color;
}

static void b24_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ���� ��´�.
// ����: ��ǥ ���� Ȯ���Ͽ� ������ ��ǥ�� �Էµǵ� ���� ������ �߻����� �ʵ��� �Ѵ�.
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b24_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// ����: ��ǥ�� ���� Į�� ���� ���Ѵ�.
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     coor_y = coor_y;     color = color;
}

static void b24_hline( dc_t *dc, int x1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
{
   int      n_color;

   printf( "�����ؾ� ��: gx.c -> b24_hline\n");
   n_color  = b24_color( color);
   dc       = dc;    x1st = x1st;    x_2nd = x_2nd;    coor_y = coor_y;     color = color;
}

static void b24_vline( dc_t *dc, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
// �μ�: dc             ��� ��� DC
//       coor_x         ��� x ��ǥ
//       y_1st, y_2nd   �������� �߱� ���� y ��ǥ�� ���۰� ���� ��ǥ
{
   printf( "���� �ؾ� ��\n");
   dc = dc;    coor_x = coor_x;     y_1st = y_1st;    y_2nd = y_2nd;    color = color;
}

/********************************************************************** 32 bpp */

static int b32_color( color_t color)
//-------------------------------------------------------------------------------
// ����: R,G,B ���� �������� ��, DC�� �ش��ϴ� ���� Į�� ���� ���Ѵ�.
// �μ�: red:0���� 255 ������ red ��, green:green ��, blud:blue  ��
// ��ȯ: ���� Į�� ��
{
   return   ( color.red << 16) | ( color.green << 8) | color.blue;
}

static void b32_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// ����: ��ũ���� Ư�� Į��� ä��
// �μ�: color       Į��
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
// ����: ���� ��´�.
{
   unsigned long *ptr;

   ptr   = (unsigned long *)dc->mapped +dc->width*coor_y +coor_x;
   *ptr  = b32_color( color);
}

static void b32_get_pixel( dc_t *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// ����: ��ǥ�� ���� Į�� ���� ���Ѵ�.
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
// ����: ������ �׸���.
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
// ����: �������� �׸���.
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
// ����: ���� 8 �κ��� ��ǥ�� ó��
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
// ����: ���� �׵θ��� ���� ����
{
   gx_set_pixel( dc, x1st , coor_y, color);
   gx_set_pixel( dc, x_2nd, coor_y, color);
}

static void circle( dc_t *dc, int center_x, int center_y, int radius, color_t color, void (*fun)( dc_t *dc, int, int, int, color_t) )
//-------------------------------------------------------------------------------
// ����: ���� ���� �׸���.
// ����: fun() �Լ��� ���� ���θ� ä��ų� �׵θ��� �׸���.
// �μ�: dc          Device Context
//       center_x,_y ���� �߽� ��ǥ
//       radius      ���� ������
//       color       �׵θ� �Ǵ� ���� ����
//       (*fun)      �׵θ� �Ǵ� ���θ� ĥ�ϴ� �Լ�

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
// ����: Ÿ���� �׸���.
// ����: fun() �Լ��� ���� ���θ� ä��ų� �׵θ��� �׸���.
// �μ�: dc             Device Context
//       center_x, _y   Ÿ���� �߽� ��ǥ
//       width          Ÿ���� ���� ��
//       height         Ÿ���� ���� ����
//       color          �׵θ� �Ǵ� ���� ����
//       (*fun)         �׵θ� �Ǵ� ���θ� ĥ�ϴ� �Լ�
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
// ����: DC �ڿ��� ��ȯ
{
   if ( NULL != dc)     free( dc);
   dc = NULL;
}

static void release_buffer_dc( dc_t *dc)
//-------------------------------------------------------------------------------
// ����: DC �ڿ��� ��ȯ
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
// ����: Į�� ���̿� ���߾� ���� �Լ��� ����
// ����: Į�� ���̿� �ش��ϴ� �����Լ��� ���ٸ� dc�� �Ҹ�ȴ�.
// �μ�: dc          ��� DC
//       colors      Į�� ����
{
      switch( colors)                                                            // Į�� ���̿� ���� ������ �Լ��� ����
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
      case 16  :                                                                 // 16bit Į�� ���ؼ��� ���� �Լ� ���
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
// ����: DC������ �ٸ� DC�� �����Ѵ�.
// �μ�: dc_dest           : ��� ������ Device Context Handle
//       dest_x, dest_y    : ��� x, y ��ǥ
//       dc_sour           : ����� ��� Device Context Handle
//       sour_x, sour_y    : dc_sour���� ���� ���� ��ǥ
//       sour_w, sour_h    : dc_sour���� ���� ���� ��ǥ
//       bytes_per_pixel    : pixel �� ����Ʈ ����
{
   int   bytes_per_line = sour_w *bytes_per_pixel;                                // ���� ������ ���δ� ����Ʈ ��
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

///////////////////////////////////////////////////////////////////////////////// �ܺο� �����Ǵ� �Լ�

color_t  gx_color( int red, int green, int blue, int alpha)
//-------------------------------------------------------------------------------
// ����: Į�� ���� color_t�� ���Ѵ�.
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
// ����: Į�� ���� ������ ����
// ����: �ּ� 0������ �ִ� 255���� Ȯ���Ͽ� ����
{
   if       (   0 > alpha)       color->alpha   = 0;
   else if  ( 255 < alpha)       color->alpha   = 255;
   else                          color->alpha   = alpha;
}

void  gx_clear( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// ����: dc�� �μ��� ���� �������� ��ü ĥ��
// �μ�: dc    Device Context
//       color ĥ�� ����
{
   dc->clear( dc, color);
}

void  gx_pen_color( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// ����: dc�� Pen ���� ����
// �μ�: dc       Device Context
//       color    Pen ����
{
   dc->pen_color = color;
}

void  gx_brush_color( dc_t *dc, color_t color)
//-------------------------------------------------------------------------------
// ����: dc�� Brush ���� ����
// �μ�: dc       Device Context
//       color    �귯�� ����
{
   dc->brush_color = color;
}

void  gx_get_pixel( dc_t  *dc, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
// ����: ��ǥ�� ���� Į�� ���� ���մϴ�.
// ����: ��ǥ�� ��ȿ������ Ȯ���Ѵ�.
// �μ�: dc                Device Context
//       coor_x, coor_y    ������ ���� ��ǥ
//       color             ������ ���� ���۷��� ����
{
   if ( 0 > coor_x || dc->width  <= coor_x)     return;
   if ( 0 > coor_y || dc->height <= coor_y)     return;

   dc->get_pixel( dc, coor_x, coor_y, color);
}

void  gx_set_pixel( dc_t *dc, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: dc�� ���� ����ϴ�.
// ����: ��ǥ�� ��ȿ������ Ȯ���Ѵ�.
// �μ�: dc                Device Context
//       coor_x, coor_y    ������ ������ ��ǥ
//       color             ���� ��
{
   if ( 0 > coor_x || dc->width  <= coor_x)     return;
   if ( 0 > coor_y || dc->height <= coor_y)     return;

   dc->set_pixel( dc, coor_x, coor_y, color);
}

void  gx_move_to( dc_t *dc, int coor_x, int coor_y)
//-------------------------------------------------------------------------------
// ����: dc�� ��ǥ�� �̵�
// �μ�: dc                Device Context
//       coor_x, coor_y    ��ǥ
{
   dc->coor_x  = coor_x;
   dc->coor_y  = coor_y;
}

void  gx_line_to( dc_t *dc, int coor_x, int coor_y)
//-------------------------------------------------------------------------------
// ����: dc�� ��ǥ�� �μ��� ���� ��ǥ���� ���� �׸�
// ����: �μ��� ���� ��ǥ�� dc ��ǥ�� ����
{
   int      rx, ry;
   int      dx, dy;
   int      inc_x;
   int      inc_y;
   int      offset;
   color_t  color;

   rx = dc->coor_x;                                                              // dc ��ǥ ��
   ry = dc->coor_y;

   dc->coor_x  = coor_x;                                                         // dc ��ǥ�� �μ��� ���� ������ ����
   dc->coor_y  = coor_y;

   dx = coor_x -rx;                                                              // x ��ǥ�� ���� delta ��
   if ( 0 == dx)                                                                 // ���ļ��� �׸��ٸ�
   {
      gx_vline( dc, rx, ry, coor_y, dc->pen_color);
      return;
   }
   else if ( 0 < dx)    inc_x = 1;                                               // x ��ǥ�� �������� �����ϴ� �����̶��
   else                                                                          // x ��ǥ�� �������� �����ϴ� �����̶��
   {
      dx    = -dx;
      inc_x = -1;
   }

   dy = coor_y -ry;                                                              // y ��ǥ�� ���� delta ��
   if ( 0 == dy)                                                                 // ������ �׸��ٸ�
   {
      gx_hline( dc, rx, coor_x, ry, dc->pen_color);
      return;
   }
   else if ( 0 < dy)    inc_y = 1;                                               // y �·ᰡ ������ �����ϴ� �����̶��
   else                                                                          // y �·ᰡ ����   �����ϴ� �����̶��
   {
      dy    = -dy;
      inc_y = -1;
   }

   color = dc->pen_color;                                                        // ���� ������ ����
   gx_set_pixel( dc, rx, ry, color);                                             // ù��° ��ǥ�� ����
   if ( dy <= dx)                                                                // x �·� ������ �� ŭ, x ��ǥ�� 1�� ��ȭ
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
   else                                                                          // y �·� ������ �� ŭ, y ��ǥ�� 1�� ��ȭ
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
// ����: ���� �߱�
{
   int             ndx;

   if ( 0 > coor_y || dc->height <= coor_y)  return;                             // y ��ǥ�� ��ũ�� ������ ����� ����
   if ( x_2nd < x_1st)                                                           // x_1st �� ���� �·Ḧ ������ swap
   {
      ndx      = x_1st;
      x_1st    = x_2nd;
      x_2nd    = ndx;
   }

   if       ( 0         >  x_1st)  x_1st   = 0;                                  // ���� ȭ�� ���� ��ǥ�̸� 0���� �׸���.
   else if  ( dc->width <= x_1st)  x_1st   = dc->width-1;                        // ���� ȭ�� ���� ��ǥ�̸� ���� ȭ������� �׸���.

   if       ( 0         >  x_2nd)  x_2nd   = 0;
   else if  ( dc->width <= x_2nd)  x_2nd   = dc->width-1;

   dc->hline( dc, x_1st, x_2nd, coor_y, color);
}

void gx_vline( dc_t *dc, int coor_x, int y_1st , int y_2nd , color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �߱�
{
   int             ndx;

   if ( 0 > coor_x || dc->width <= coor_x)   return;                             // x �·ᰡ ������ ����� ����

   if ( y_2nd < y_1st)                                                           // y_1st ��ǥ�� ���� ���� ������ swap
   {
      ndx     = y_1st;
      y_1st    = y_2nd;
      y_2nd    = ndx;
   }

   if       ( 0           >  y_1st)  y_1st   = 0;                                // ȭ�� ��� ���� ��ǥ�̸� 0���� �׸���.
   else if  ( dc->height  <= y_1st)  y_1st   = dc->height-1;                    // ȭ�� �ϴ� ���� ��ǥ�̸� ���� ȭ������� �׸���.

   if       ( 0           >  y_2nd)  y_2nd   = 0;
   else if  ( dc->height  <= y_2nd)  y_2nd   = dc->height-1;

   dc->vline( dc, coor_x, y_1st, y_2nd, color);
}

void  gx_line( dc_t *dc, int x1, int y1, int x2, int y2)
//-------------------------------------------------------------------------------
// ����: �� ���� ��ǥ�� �մ� ���� �׸���.
{
   gx_move_to( dc, x1, y1);
   gx_line_to( dc, x2, y2);
}

void  gx_rectangle( dc_t *dc, int x1, int y1, int x2, int y2)
//-------------------------------------------------------------------------------
// ����: �� ���� ��ǥ�� �̿��Ͽ� �׵θ��� ���ΰ� ä���� �簢���� �׸���.
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

//���θ� ä���� �ʰ� �簢�� �׸���.
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
// ����: ���� �׸���.
// ����: �׵θ��� ���θ� ä���.
// �μ�: dc          Device Context
//       center_x,_y ���� �߽� ��ǥ
//       radius      ���� ������
{
   if ( 0 == radius) return;

   circle( dc, center_x, center_y, radius, dc->brush_color, gx_hline);
   circle( dc, center_x, center_y, radius, dc->pen_color  , circle_dot);
}

void  gx_ellipse(  dc_t *dc, int center_x, int center_y, int width, int height)
//-------------------------------------------------------------------------------
// ����: Ÿ���� �׸���.
// ����: �׵θ��� ���θ� ä���.
// �μ�: dc          Device Context
//       center_x,_y Ÿ���� �߽� ��ǥ
//       width       Ÿ���� ��
//       height      Ÿ���� ����
{
   if ( ( 0 == width) || ( 0 == height) ) return;

   ellipse( dc, center_x, center_y, width, height, dc->brush_color, gx_hline);
   ellipse( dc, center_x, center_y, width, height, dc->pen_color  , circle_dot);
}

void gx_to_screen_dc( dc_t *dc_screen, dc_t *dc_buffer)
//-------------------------------------------------------------------------------
// ����: ScreenDC �� BufferDC ��ü�� �����Ѵ�.
// �μ�  dc_screen      ��ũ�� DC
//       dc_buffer      ���� DC
{
   memcpy( dc_screen->mapped, dc_buffer->mapped, dc_screen->bytes);
}

void  gx_bitblt(  dc_t *dc_dest, int dest_x, int dest_y, dc_t *dc_sour, int sour_x, int sour_y, int sour_w, int sour_h)
//-------------------------------------------------------------------------------
// ����: DC������ �ٸ� DC�� �����Ѵ�.
// �μ�: dc_dest         : ��� ������ Device Context Handle
//       dest_x, dest_y  : ��� x, y ��ǥ
//       dc_sour         : ����� ��� Device Context Handle
//       sour_x, sour_y  : dc_sour���� ���� ���� ��ǥ
//       sour_w, sour_h  : dc_sour���� ���� ���� ��ǥ
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

   if ( ( DCTYPE_SCREEN == dc_sour->dc_type) && ( DCTYPE_SCREEN == dc_dest->dc_type) ) // �� ���� DC�� ��� Screen Type �� ��, memcpy()�� ����� ���� ���� ���
   {
      switch( dc_dest->colors)
      {
      case  8  :
      case  15 :
      case  16 :
      case  24 :
      case  32 :  byte_bitblt( dc_dest, dest_x, dest_y, dc_sour, sour_x, sour_y, sour_w, sour_h, dc_sour->bits_per_pixel / 8);
                  break;
      default  :  printf( "8 bit ������ byte_bitblt()�� �����ؾ� �մϴ�. \n");
      }
   }
   else if ( DCTYPE_PNG == dc_sour->dc_type)                                     // ������ PNG �����̸� ���� ������ ó���Ѵ�.
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
   else                                                                          // ���� ������ ���� �ҽ���� �ٷ� ����
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
// ����: Device Context �ڿ��� ��ȯ
{
   dc->release_dc( dc);
}

dc_t *gx_get_buffer_dc( int width, int height)
//-------------------------------------------------------------------------------
// ����: ȭ�� ����� ������ �ϱ� ���� ���� DC�� ����.
// ����: BitBlt�� ������ ó���ϱ� ���� DC Type�� DCTYPE_SCREEN���� �����Ѵ�.
// �μ�: width       ������ ��
//       height      ������ ����
// ��ȯ: ���� Device Context �ڵ�
{
   int   szStruct;
   dc_t  *dc;

   szStruct = sizeof( dc_t);
   dc = malloc( szStruct);                                                       // dc_t ����ü �޸� �Ҵ�
   if ( NULL != dc)                                                              // �޸� ���ϱ⿡ �����Գ�
   {
      dc->dc_type        = DCTYPE_SCREEN;                                        // DC ���¸� SCREEN����, ���� SCREEN�� ���� ó���� ����

      if ( 0 > width ) width  = gx_fb.width;                                     // ũ�Ⱑ �������� �ʴٸ� �����ӹ��� ũ��� ����
      if ( 0 > height) height = gx_fb.height;

      dc->width            = width;                                              // ��
      dc->height           = height;                                             // ����
      dc->colors           = gx_fb.colors;                                       // Į�� ����
      dc->dots             = width *height;                                      // ��Ʈ ����
      dc->mapped           = NULL;                                               // �޸� ������, set_virtual_func()���� dc�� Release �� �� �����Ƿ� �ݵ�� NULL�� �ʱ�ȭ
      dc->coor_x           = 0;                                                  // LineTo�� ���� ��ǥ�� 0���� �ʱ�ȭ
      dc->coor_y           = 0;
      dc->pen_color        = gx_color( 255, 255, 255, 255);                      // �⺻ �� ������ ���
      dc->brush_color      = gx_color(   0,   0,   0, 255);                      // �⺻ �귯�� ������ ����
      dc->release_dc       = release_buffer_dc;

      dc->bits_per_pixel= gx_fb.bits_per_pixel;                                  // ��Ʈ�� �ȼ� ����

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
      default  :  dc->bytes_per_line  = width    ; break;                        // 8 bit �Ǵ� ���� �ػ󵵰� �ƴ� ��� ���� ����Ʈ �� ��ŭ ����
      }
      dc->bytes   = dc->bytes_per_line *height;
      dc->mapped  = malloc( dc->bytes);
      set_virtual_func( dc, gx_fb.colors);                                       // Į�� ���̿� ���߾� ���� �Լ��� ����, Į�� ���̿� �ش��ϴ� �����Լ��� ���ٸ� dc�� �Ҹ�ȴ�.
   }
   else
   {
      gx_error_code  = GXERR_OUT_OF_MEMORY;
   }
   return dc;
}

dc_t *gx_get_screen_dc( void)
//-------------------------------------------------------------------------------
// ����: ȭ�鿡 �׷����� ����ϱ� ���� Device Context�� ���Ѵ�.
// ��ȯ: ���� -> dc_t *, ���� -> NULL
// ����: ������ ���ۿ��� ȭ�鿡 ���� ��� ������ ���Ѵ�.
{
   int   szStruct;
   dc_t  *dc;

   szStruct = sizeof( dc_t);
   dc = malloc( szStruct);                                                       // dc_t ����ü �޸� �Ҵ�
   if ( NULL != dc)                                                              // �޸� ���ϱ⿡ �����Գ�
   {
      dc->dc_type             = DCTYPE_SCREEN;                                   // DC ���¸� SCREEN����
      dc->width               = gx_fb.width;                                     // ȭ�� ��
      dc->height              = gx_fb.height;                                    // ȭ�� ����
      dc->dots                = gx_fb.dots;                                      // ȭ�� ��ü ��Ʈ ����
      dc->bytes               = gx_fb.bytes;                                     // ȭ�鿡 ���� �޸� ��ü ũ��
      dc->colors              = gx_fb.colors;                                    // Į�� ����
      dc->bytes_per_line      = gx_fb.bytes_per_line;                            // ���δ� ����Ʈ ����
      dc->bits_per_pixel      = gx_fb.bits_per_pixel;                            // ��Ʈ�� �ȼ� ����

      dc->mapped              = gx_fb.mapped;                                    // ȭ�鿡 ���� �޸� ���� �ּ�
      dc->coor_x              = 0;                                               // LineTo�� ���� ��ǥ�� 0���� �ʱ�ȭ
      dc->coor_y              = 0;
      dc->pen_color           = gx_color( 255, 255, 255, 255);                   // �⺻ �� ������ ���
      dc->brush_color         = gx_color(   0,   0,   0, 255);                   // �⺻ �귯�� ������ ����
      dc->release_dc          = release_screen_dc;

      set_virtual_func( dc, gx_fb.colors);                                       // Į�� ���̿� ���߾� ���� �Լ��� ����, Į�� ���̿� �ش��ϴ� �����Լ��� ���ٸ� dc�� �Ҹ�ȴ�.
   }
   else
   {
      gx_error_code  = GXERR_OUT_OF_MEMORY;
   }
   return dc;
}

char  *gx_error_string( void)
//-------------------------------------------------------------------------------
// ����: gx_error_code�� ���� ���� ���� ���ڿ��� ��ȯ
{
   char *error_string[] ={ "���� ����",                                          // GXERR_NONE
                           "��ġ�� ����",                                        // GXERR_NO_DEVICE
                           "���� ������ ����",                                   // GXERR_ACCESS_DEVICE
                           "FBIOGET_VSCREENINFO�� ������ ����",                  // GXERR_VSCREEN_INFO
                           "FBIOGET_FSCREENINFO�� ������ ����",                  // GXERR_FSCREEN_INFO
                           "������ ���� �޸� ���ο� ����",                     // GXERR_MEMORY_MAPPING
                           "�̹��� ������ ����",                                 // GXERR_NOFILE
                           "�̹��� ��� ���� �̻�",                              // GXERR_HEADER_INFO
                           "�̹��� ������ �б� �̻�",                            // GXERR_READ_FILE
                           "�ȷ�Ʈ ���� �̻�",                                   // GXERR_PALETTE_INFO
                           "Į�� ���̰� ǥ���� �ƴ�",                            // GXERR_COLOR_DEPTH
                           "��Ʈ������ ���ų� ����� �� ����",                   // GXERR_NO_FONT_FILE
                           "�ñ׳��� ����",                                      // GXERR_SIGNATURE
                           "�޸� ����",                                        // GXERR_OUT_OF_MEMORY
                           "ó�� �� ���� �߻�"                                   // GXERR_PROCESSING
                        };
   return( error_string[gx_error_code]);
}

int gx_print_error( char *remark)
//-------------------------------------------------------------------------------
// ����: gx_error_code�� ���� ���� ���� ���ڿ��� ���
// �μ�: remark      ���� ���� ���ڿ� ���� ÷���Ͽ� ����� ���ڿ�
{
   printf( "[gx error:%d]%s %s\n", gx_error_code, gx_error_string(), remark);
   return gx_error_code;
}

void  gx_close( void)
//-------------------------------------------------------------------------------
// ����: �׷��� ���̺귯�� ����
// ����: �޸� ���� �����Ϳ� ������ ���� ��ũ���͸� �Ҹ��ϰ� ������ �ʱ�ȭ �Ѵ�.
{
   if ( 0 <= gx_fb.mapped)                                                       // �޸𸮰� ���� �Ǿ� �ִٸ�
   {
      munmap( gx_fb.mapped, gx_fb.bytes);                                        // �޸� ���� �ڿ� ��ȯ
      gx_fb.mapped = MAP_FAILED;
   }
   if ( 0 <= gx_fb.fd)                                                           // ���� ��ũ���Ͱ� ����� ���ٸ�
   {
      close( gx_fb.fd);                                                          // ���� ��ũ���� �ڿ� ��ȯ
      gx_fb.fd = -1;
   }
}

int   gx_init( char *dev_name)                                                   // ��ġ �̸�
//-------------------------------------------------------------------------------
// ����: �׷��� ���̺귯���� �ʱ�ȭ
// ��ȯ: 0 = ���� ����, 0 > ���� �߻�
{
   struct   fb_var_screeninfo  fbvar;                                            // ȭ�� ������ ���ϱ� ����
   struct   fb_fix_screeninfo  fbfix;

   gx_fb.fd          = -1;                                                       // �ʱⰪ�� ����
   gx_fb.mapped      = MAP_FAILED;                                               // �ʱⰪ�� ����

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
   gx_fb.width        = fbvar.xres;                                              // ��ũ���� �ȼ� ��
   gx_fb.height       = fbvar.yres;                                              // ��ũ���� �ȼ� ����
   gx_fb.dots         = gx_fb.width * gx_fb.height;                              // ��ũ�� ��Ʈ ����
   gx_fb.bits_per_pixel = fbvar.bits_per_pixel;                                  // ��ũ���� Į�� ����
   gx_fb.bytes_per_line = fbfix.line_length;                                     // �Ѱ� ���� �� ����Ʈ ����

   gx_fb.bytes        = gx_fb.bytes_per_line *gx_fb.height;                      // ��ũ���� �� �޸� ����Ʈ ��
   gx_fb.mapped       = ( void *)mmap( 0,
                                      gx_fb.bytes,                               // �޸� ��ü ũ��
                                      PROT_READ|PROT_WRITE,
                                      MAP_SHARED,
                                      gx_fb.fd,
                                      0);
   switch( gx_fb.bits_per_pixel)                                                 // Į�� ���� ������ ����
   {
   case 16  :  gx_fb.colors   =   fbvar.red.length                               // 15bit Į���� ��츦 ���� ���� Į�� bit ���� ���Ѵ�.
                                 +fbvar.green.length                             // 15bit�� ��� 15���� ����
                                 +fbvar.blue.length;                             // 16bit�� ��� 16���� ����
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

/*******************************************************************************
������Ʈ : gxLib
��⳻�� : BITMAP ���
�������� : 2008-11-22
�ۼ���   : ��漮(jwjwmx@gmail.com)
���۱�   : �ּ� ������ �������� �ʴ� �� ���� ����
Ȩ������ : http://forum.falinux.com
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gx.h>
#include <gxbmp.h>

#define BI_RGB          0
#define BI_RLE4         1
#define BI_RLE8         2
#define BI_BITFIELD     3

/********************************************************************* 1 pixel */

static int   b1_color( color_t color)
//-------------------------------------------------------------------------------
// ����: Į�� ���� ���Ѵ�.
// �μ�: red               0���� 255 ������ red   ��
//       green             0���� 255 ������ green ��
//       blue              0���� 255 ������ blue  ��
{
   if ( 0 == color.red +color.green +color.blue)
      return   0;
   else
      return   1;
}

static void  b1_clear( bmp_t *bmp, color_t color)
//-------------------------------------------------------------------------------
{
   int      ncolor;

   printf( "�����ؾ� ��: gxbmp.c -> b1_clear\n");
   ncolor   = b1_color( color);
   bmp = bmp; color = color;
}

static void b1_get_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
{
   unsigned char *pdata;
   int            noffset;
   int            npixel;

   pdata    = bmp->data + (bmp->height -coor_y -1) * bmp->bytes_per_line + (coor_x /8);
   noffset  = coor_x %8;
   npixel   = (*pdata & (0x80U >> noffset)) >> (7 -noffset);

   /* palette lookup */
   color->blue    = bmp->palette[npixel].blue;
   color->green   = bmp->palette[npixel].green;
   color->red     = bmp->palette[npixel].red;
   color->alpha   = 255;
}

static void b1_set_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
{
   printf( "�����ؾ� ��: gxbmp.c -> b1_set_pixel\n");
   bmp = bmp;     coor_x = coor_x;    coor_y = coor_y;    color = color;
}

static void  b1_hline( bmp_t *bmp, int x_1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
{
   printf( "�����ؾ� ��: gxbmp.c -> b1_hline\n");
   bmp = bmp;  x_1st = x_1st;   x_2nd = x_2nd;   coor_y = coor_y;    color = color;
}

static void  b1_vline( bmp_t *bmp, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
{
   printf( "�����ؾ� ��: gxbmp.c -> b1_vline\n");
   bmp = bmp;  coor_x = coor_x; y_1st = y_1st;   y_2nd = y_2nd;    color = color;
}

/********************************************************************* 4 pixel */

static int   b4_color( color_t color)
//-------------------------------------------------------------------------------
// ����: Į�� ���� ���Ѵ�.
{
   printf( "�����ؾ� ��: gxbmp.c -> b4_color\n");
   color = color;
   return  0;
}

static void  b4_clear( bmp_t *bmp, color_t color)
//-------------------------------------------------------------------------------
{
   printf( "�����ؾ� ��: gxbmp.c -> b4_clear\n");
   bmp = bmp; color = color;
}
static void b4_get_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
{
   unsigned char *pdata;
   int            noffset;
   int            npixel;

   pdata    = bmp->data + ( bmp->height -coor_y -1) * bmp->bytes_per_line + ( coor_x>>1);
   noffset  = ( coor_x & 0x01) << 2;

   npixel   = (*pdata & (0xF0U >> noffset)) >> (4-noffset);

   /* palette lookup */
   color->blue    = bmp->palette[npixel].blue;
   color->green   = bmp->palette[npixel].green;
   color->red     = bmp->palette[npixel].red;
   color->alpha   = 255;
}

static void b4_set_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
{
   bmp = bmp;  coor_x = coor_x;  coor_y = coor_y;  color = color;
}

static void  b4_hline( bmp_t *bmp, int x_1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
{
   int      ncolor;

   printf( "�����ؾ� ��: gxbmp.c -> b4_hline\n");
   ncolor  = b4_color( color);
   bmp = bmp;  x_1st = x_1st;   x_2nd = x_2nd; coor_y = coor_y;  color = color;
}

static void  b4_vline( bmp_t *bmp, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
{
   printf( "�����ؾ� ��: gxbmp.c -> b4_vline\n");
   bmp = bmp;  coor_x = coor_x;  y_1st = y_1st; y_2nd = y_2nd; color = color;
}

/********************************************************************* 8 pixel */

static int   b8_color( color_t color)
//-------------------------------------------------------------------------------
// ����: Į�� ���� ���Ѵ�.
{
   printf( "�����ؾ� ��: gxbmp.c -> b8_color\n");
   color = color;
   return  0;
}

static void  b8_clear( bmp_t *bmp, color_t color)
//-------------------------------------------------------------------------------
{
   printf( "�����ؾ� ��: gxbmp.c -> b8_clear\n");
   bmp = bmp; color = color;
}
static void b8_get_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
{
   unsigned char *pdata;
   int            npixel;

   pdata          = bmp->data +(bmp->height -coor_y -1) *bmp->bytes_per_line +coor_x;
   npixel         = *pdata;

   /* palette lookup */

   color->blue    = bmp->palette[npixel].blue;
   color->green   = bmp->palette[npixel].green;
   color->red     = bmp->palette[npixel].red;
   color->alpha   = 255;
}

static void b8_set_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
{
   printf( "�����ؾ� ��: gxbmp.c -> b8_set_pixel\n");
   bmp = bmp;  coor_x = coor_x;  coor_y = coor_y;  color = color;
}

static void  b8_hline( bmp_t *bmp, int x_1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
{
   int      ncolor;

   printf( "�����ؾ� ��: gxbmp.c -> b8_hline\n");
   ncolor  = b8_color( color);
   bmp = bmp;  x_1st = x_1st;   x_2nd = x_2nd;   coor_y = coor_y;    color = color;
}

static void  b8_vline( bmp_t *bmp, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
{
   printf( "�����ؾ� ��: gxbmp.c -> b8_vline\n");
   bmp = bmp;  coor_x = coor_x; y_1st = y_1st;   y_2nd = y_2nd;    color = color;
}

/******************************************************************** 16 pixel */

static int   b16_color( color_t color)
//-------------------------------------------------------------------------------
// ����: R,G,B ���� �������� ��, DC�� �ش��ϴ� ���� Į�� ���� ���Ѵ�.
// ����: 16 bit ȯ�濡 ���� shift=0:5:6:5, shift=0:11:5:0
// ����: red, green, blue ���� �Ʒ��� ���� ���ؾ� �Ǵ�. �׷��� ���� �ӵ��� ���� �Ϲ� ����� ġȯ�ߴ�.
//   red   >>= 8-bmp->bsize_red;
//   green >>= 8-bmp->bsize_green;
//   blue  >>= 8-bmp->bsize_blue;
//   return   ( red << bmp->boffset_red) | ( green << bmp->boffset_green) | blue << bmp->boffset_blue;
// �μ�: red:0���� 255 ������ red ��, green:green ��, blud:blue  ��
// ��ȯ: ���� Į�� ��
{
   color.red   >>= 3;
   color.green >>= 3;
   color.blue  >>= 3;

   return   ( color.red << 10) | ( color.green << 5) | color.blue;
}

static void  b16_clear( bmp_t *bmp, color_t color)
//-------------------------------------------------------------------------------
// ����: Bitmap DC�� ������ ������ �������� ä���.
{
   unsigned short *ptr;
   int             ncolor;
   int             ndx;

   if ( 0 < color.alpha)
   {
      ncolor  = b16_color( color);
      ptr = (unsigned short *)bmp->data;

      for ( ndx = 0; ndx < bmp->dots; ndx++)
         *ptr++ = ncolor;
   }
}

static void b16_get_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
{
   unsigned short *pdata;
   unsigned       *mask    = (unsigned *)( bmp->palette);

   pdata = (unsigned short *)(bmp->data +(bmp->height -coor_y -1) *bmp->bytes_per_line +( coor_x << 1));

   color->blue    = ((*pdata & mask[2]) >> bmp->boffset_blue ) << (8-bmp->bsize_blue );
   color->green   = ((*pdata & mask[1]) >> bmp->boffset_green) << (8-bmp->bsize_green);
   color->red     = ((*pdata & mask[0]) >> bmp->boffset_red  ) << (8-bmp->bsize_red  );
   color->alpha   = 255;
}

static void b16_set_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
{
   unsigned short *pdata;
   int             offset = (bmp->height -coor_y -1) *bmp->bytes_per_line +( coor_x << 1);

   pdata = (unsigned short *)(bmp->data +offset);
  *pdata = b16_color( color);
}


static void  b16_hline( bmp_t *bmp, int x_1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
// �μ�: x_1st,_2nd     ������ �߱� ���� x ���۰� ���� ��ǥ
//       coor_y         ���� y ��ǥ
//       color
{
   unsigned short *ptr;
   int             ncolor;
   int             ndx;

   ncolor  = b16_color( color);
   ptr   = (unsigned short *)bmp->mapped + bmp->width *( bmp->height -coor_y) +x_1st;
   for ( ndx = x_1st; ndx <= x_2nd; ndx++)
     *ptr++ = ncolor;
}

static void  b16_vline( bmp_t *bmp, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
// �μ�: bmp            Bitmap DC
//       coor_x         �������� �߱� ���� x ��ǥ
//       y_1st,_2nd     �������� �߱� ���� y ��ǥ
//       color          �������� ����
{
   unsigned short *ptr;
   int             ncolor;
   int             ndx;

   ncolor  = b16_color( color);
   ptr   = (unsigned short *)bmp->mapped + bmp->width * ( bmp->height-y_1st) +coor_x;
   for ( ndx = y_1st; ndx <= y_2nd; ndx++)
   {
     *ptr   = ncolor;
      ptr  -= bmp->width;
   }
}

/******************************************************************** 24 pixel */

static int b24_color( color_t color)
//-------------------------------------------------------------------------------
// ����: R,G,B ���� �������� ��, DC�� �ش��ϴ� ���� Į�� ���� ���Ѵ�.
// �μ�: red:0���� 255 ������ red ��, green:green ��, blud:blue  ��
// ��ȯ: ���� Į�� ��
{
   return  ( color.red << 16) | ( color.green << 8) | color.blue;
}

static void b24_clear( bmp_t *bmp, color_t color)
//-------------------------------------------------------------------------------
// ����: Bitmap DC�� ������ ������ �������� ä���.
// �μ�: bmp
//       color
{
   printf( "�����ؾ� ��: gxbmp.c -> b24_clear\n");
   bmp = bmp; color = color;
}

static void b24_get_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
{
   unsigned char *pdata;

   pdata          = bmp->data + ( bmp->height -coor_y -1)*bmp->bytes_per_line +coor_x*3;
   color->blue    = *pdata;
   color->green   = *(pdata+1);
   color->red     = *(pdata+2);
   color->alpha   = 255;
}

static void b24_set_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
{
   unsigned char *pdata;
   int            offset = ( bmp->height -coor_y -1)*bmp->bytes_per_line +coor_x*3;

   pdata          = bmp->data + offset;
  *pdata          = b24_color( color);
}

static void b24_hline( bmp_t *bmp, int x_1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
{
   int      ncolor;

   printf( "�����ؾ� ��: gxbmp.c -> b24_hline\n");
   ncolor  = b24_color( color);
   bmp = bmp;  x_1st = x_1st;   x_2nd = x_2nd;   coor_y = coor_y;    color = color;
}

static void b24_vline( bmp_t *bmp, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
{
   printf( "�����ؾ� ��: gxbmp.c -> b24_vline\n");
   bmp = bmp;  coor_x = coor_x; y_1st = y_1st;   y_2nd = y_2nd;    color = color;
}

/******************************************************************** 32 pixel */

static int b32_color( color_t color)
//-------------------------------------------------------------------------------
// ����: R,G,B ���� �������� ��, DC�� �ش��ϴ� ���� Į�� ���� ���Ѵ�.
// �μ�: red:0���� 255 ������ red ��, green:green ��, blud:blue  ��
// ��ȯ: ���� Į�� ��
{
   return  ( color.red << 16) | ( color.green << 8) | color.blue;
}

static void  b32_clear( bmp_t *bmp, color_t color)
//-------------------------------------------------------------------------------
{
   printf( "�����ؾ� ��: gxbmp.c -> b32_clear\n");
   bmp = bmp; color = color;
}

static void b32_get_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
{
   unsigned *pdata;
   unsigned *mask    = ( unsigned *)( bmp->palette);

   pdata = (unsigned *)(bmp->data +( bmp->height -coor_y -1)*bmp->bytes_per_line +( coor_x << 2));

   color->blue    = ((*pdata & mask[2]) >> bmp->boffset_blue   ) << (8-bmp->bsize_blue );
   color->green   = ((*pdata & mask[1]) >> bmp->boffset_green  ) << (8-bmp->bsize_green);
   color->red     = ((*pdata & mask[0]) >> bmp->boffset_red    ) << (8-bmp->bsize_red  );
   color->alpha   = 255;
}

static void b32_set_pixel( bmp_t *bmp, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
{
   printf( "�����ؾ� ��: gxbmp.c -> b32_set_pixel\n");
   bmp = bmp;  coor_x   = coor_x;  coor_y = coor_y;    color = color;
}

static void  b32_hline( bmp_t *bmp, int x_1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
{
   int      ncolor;

   printf( "�����ؾ� ��: gxbmp.c -> b32_hline\n");
   ncolor  = b32_color( color);
   bmp = bmp;  x_1st = x_1st;   x_2nd = x_2nd;   coor_y = coor_y;    color = color;
}

static void  b32_vline( bmp_t *bmp, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
{
   printf( "�����ؾ� ��: gxbmp.c -> b32_vline\n");
   bmp = bmp;  coor_x = coor_x;  y_1st = y_1st; y_2nd = y_2nd; color = color;
}

/*******************************************************************************/

static void calculate_boffset(bmp_t *bmp)
//-------------------------------------------------------------------------------
{
   int i;
   unsigned *mask = (unsigned *)(bmp->palette);
   unsigned  temp;

   /* red */
   temp = mask[0];
   for(i = 0; i < 32; i++)
   {
      if(temp & 0x01)
         break;
      temp >>= 1;
   }
   bmp->boffset_red = i;
   for(i = 0; i < 32; i++)
   {
      if(temp & 0x800000UL)
         break;
      temp <<= 1;
   }
   bmp->bsize_red = 32-i;

   /* green */
   temp = mask[1];
   for(i = 0; i < 32; i++)
   {
      if(temp & 0x01)
         break;
      temp >>= 1;
   }
   bmp->boffset_green = i;
   for(i = 0; i < 32; i++)
   {
      if(temp & 0x800000UL)
         break;
      temp <<= 1;
   }
   bmp->bsize_green = 32-i;

   /* blue */
   temp = mask[2];
   for(i = 0; i < 32; i++)
   {
      if(temp & 0x01)
         break;
      temp >>= 1;
   }
   bmp->boffset_blue = i;
   for(i = 0; i < 32; i++)
   {
      if (temp & 0x800000UL)
         break;
      temp <<= 1;
   }
   bmp->bsize_blue = 32-i;
}

static void rle8_decoding( bmp_t *bmp)
//-------------------------------------------------------------------------------
{
   unsigned char *pdata    = bmp->data;
   unsigned char *pend     = pdata + bmp->width*bmp->height;
   unsigned char *penc_data = bmp->encoded_data;
   unsigned char  c_byte;
   int            ndx, jdx;

   while( 1 )
   {
      if(pdata >= pend)
         break;
      c_byte = *penc_data++;
      if( 0 == c_byte) /* escape */
      {
         c_byte = *penc_data++;
         if( 0 == c_byte) /* end of line */
         {
            ndx = pdata - bmp->data;
            ndx %= bmp->width;
            for( ; ndx < bmp->width; ndx++)
               pdata++;
         }
         else if( c_byte == 1) /* end of bitmap */
         {
            return;
         }
         else if( c_byte == 2) /* delta */
         {
            jdx = *penc_data++; /* right */
            ndx = *penc_data++; /* down */
            ndx = jdx +ndx *bmp->width;
            while(0 < ndx--)
               pdata++;
         }
         else /* absolute mode */
         {
            c_byte = *penc_data++;
            while( 0 < c_byte--)
               *pdata++ = *penc_data++;
            /* word boundary */
            while((unsigned)penc_data & 0x01)
               penc_data++;
         }
      }
      else
      {
         while( c_byte--)
            *pdata++ = *penc_data;
         penc_data++;
      }
   }
}

static int read_data(FILE *fp, bmp_t *bmp)
//-------------------------------------------------------------------------------
{
   fseek(fp, bmp->data_offset, SEEK_SET);

   if (bmp->compression == BI_RGB || bmp->compression == BI_BITFIELD)
   {
      bmp->data = (unsigned char *)malloc(bmp->bitmap_size);
      fread(bmp->data, 1, bmp->bitmap_size, fp);
   }
   else
   {
      bmp->encoded_data  = (unsigned char *)malloc(bmp->bitmap_size);
      bmp->data         = (unsigned char *)malloc(bmp->width*bmp->height*bmp->bpp/8);
      memset(bmp->encoded_data, 0, bmp->bitmap_size);
      fread( bmp->encoded_data, 1, bmp->bitmap_size, fp);
      if(bmp->compression == BI_RLE4)
         return -1;                                               /* rle4_decoding is not supported ! */
      else
         rle8_decoding( bmp);
   }
   return 0;
}

static int read_palette(FILE *fp, bmp_t *bmp)
//-------------------------------------------------------------------------------
{
   int size;

   size = fread( bmp->palette, sizeof( palette_t), bmp->cnt_palette, fp);
   if ( size != bmp->cnt_palette)
      return -1;

   return 0;
}

static int read_header(FILE *fp, bmp_t *bmp)
//-------------------------------------------------------------------------------
{
   int size;
   int remnant;
   unsigned char ID[2];

   ID[0] = fgetc( fp);                                                           // ID üũ
   ID[1] = fgetc( fp);
   if(ID[0] != 'B' || ID[1] != 'M')
      return -1;

   if ( 52 != ( size = fread( &bmp->file_size, 1, 52, fp)))                       // bmp ��� ������ ��� �о� ���δ�.
      return -1;

   bmp->cnt_palette  = ( bmp->data_offset-54) >> 2;
   bmp->width        = bmp->img_width;
   bmp->height       = bmp->img_height;

   size     = bmp->img_width * bmp->bpp /8;
   remnant  = size %4;
   if (remnant == 0)
      bmp->bytes_per_line = size;
   else
      bmp->bytes_per_line = size +(4 -remnant);

   if ( 0 == bmp->bitmap_size)                                                    // ���� ������ �ȵ� ���¶�� bitmap_size �� 0 �� ��
      bmp->bitmap_size = bmp->height*bmp->bytes_per_line;

   return 0;
}

static int set_header( bmp_t *bmp, int width, int height, int depth, int palette_size)
//-------------------------------------------------------------------------------
{
   int size;
   int remnant;

   bmp->width        = width;
   bmp->height       = height;
   bmp->img_width    = width;
   bmp->img_height   = height;
   bmp->dots         = width * height;
   bmp->data         = NULL;
   bmp->palette      = NULL;
   bmp->compression  = BI_RGB;                                                   // BITMAP ���� ����� RGB��

   if ( 0 < palette_size)                                                         // �ȷ�Ʈ ����� �����Ǿ� �ִٸ�
   {
      bmp->cnt_palette  = palette_size;
      bmp->palette      = ( palette_t *)malloc( sizeof( palette_t) * bmp->cnt_palette);
      memset( bmp->palette, 0, sizeof( palette_t) * bmp->cnt_palette);
   }

   bmp->bpp = depth;
   size     = bmp->img_width * bmp->bpp /8;
   remnant  = size %4;

   if ( 0 == remnant)
      bmp->bytes_per_line = size;
   else
      bmp->bytes_per_line = size + (4-remnant);

   bmp->bitmap_size  = bmp->height * bmp->bytes_per_line;
   bmp->bytes        = bmp->bitmap_size;
   return 0;
}

static void  release_dc( dc_t *dc)
//-------------------------------------------------------------------------------
{
   bmp_t  *bmp  = (bmp_t *)dc;

   if ( NULL == bmp)             return;
   if ( NULL != bmp->palette)    free( bmp->palette);
   if ( NULL != bmp->data   )    free( bmp->data   );
   free( bmp);
}

static void set_error_code( bmp_t *bmp, FILE *fp, int error_code)
//-------------------------------------------------------------------------------
{
   gx_error_code  = error_code;
   release_dc( ( dc_t *)bmp);
   if ( fp)    fclose(fp);
}

static bmp_t *setup_bitmap( bmp_t *bmp)
//-------------------------------------------------------------------------------
// ����: bmp ������ �ϼ�
{
   bmp->dc_type            = DCTYPE_BMP;                                         // DC ���¸� BITMAP��
   bmp->coor_x             = 0;                                                  // LineTo�� ���� ��ǥ�� 0���� �ʱ�ȭ
   bmp->coor_y             = 0;
   bmp->pen_color          = gx_color( 255, 255, 255, 255);                      // �⺻ �� ������ ���
   bmp->brush_color        = gx_color(   0,   0,   0, 255);                      // �⺻ �귯�� ������ ����
   bmp->release_dc         = release_dc;                                         // �Ҹ��� ���� �Լ�
   bmp->mapped             = bmp->data;                                          // �̹��� �κ��� �޸� ������
   bmp->colors             = bmp->bpp;                                           // Į�� ����

   switch(bmp->bpp)
   {
   case  1:
            bmp->clear     = b1_clear;
            bmp->get_pixel = b1_get_pixel;
            bmp->set_pixel = b1_set_pixel;
            bmp->vline     = b1_vline;
            bmp->hline     = b1_hline;
            break;
   case  4:
            bmp->clear     = b4_clear;
            bmp->get_pixel = b4_get_pixel;
            bmp->set_pixel = b4_set_pixel;
            bmp->vline     = b4_vline;
            bmp->hline     = b4_hline;
            break;
   case  8:
            bmp->clear     = b8_clear;
            bmp->get_pixel = b8_get_pixel;
            bmp->set_pixel = b8_set_pixel;
            bmp->vline     = b8_vline;
            bmp->hline     = b8_hline;
            break;
   case  16:
            bmp->clear     = b16_clear;
            bmp->get_pixel = b16_get_pixel;
            bmp->set_pixel = b16_set_pixel;
            bmp->vline     = b16_vline;
            bmp->hline     = b16_hline;

            if( bmp->compression == BI_RGB)                                      // ���� BI_RGB
            {
               unsigned *mask;

               if( NULL != bmp->palette) /* something wrong */
               {
                  set_error_code( bmp, NULL, GXERR_PALETTE_INFO);
                  return NULL;
               }
               mask = (unsigned *)malloc(sizeof(unsigned)*3);
               mask[2]              = 0x001F; /* blue mask */
               mask[1]              = 0x03E0; /* green mask */
               mask[0]              = 0x7C00; /* red mask */
               bmp->palette         = ( palette_t *)mask;
               bmp->boffset_blue    = 0;
               bmp->boffset_green   = 5;
               bmp->boffset_red     = 10;
               bmp->bsize_blue      = 5;
               bmp->bsize_green     = 5;
               bmp->bsize_red       = 5;
            }
            else  /* BI_BITFIELD */
            {
               if( bmp->palette == NULL) /* something wrong */
               {
                  set_error_code( bmp, NULL, GXERR_PALETTE_INFO);
                  return NULL;
               }
               calculate_boffset(bmp);
            }
            break;
   case  24:
            bmp->clear     = b24_clear;
            bmp->get_pixel = b24_get_pixel;
            bmp->set_pixel = b24_set_pixel;
            bmp->vline     = b24_vline;
            bmp->hline     = b24_hline;
            break;
   case  32:
            bmp->clear     = b32_clear;
            bmp->get_pixel = b32_get_pixel;
            bmp->set_pixel = b32_set_pixel;
            bmp->vline     = b32_vline;
            bmp->hline     = b32_hline;
            if(bmp->compression == BI_RGB)
            {
               unsigned *mask;

               if( bmp->palette != NULL) /* something wrong */
               {
                  set_error_code( bmp, NULL, GXERR_PALETTE_INFO);
                  return NULL;
               }

               mask                 = (unsigned *)malloc(sizeof(unsigned)*3);
               mask[2]              = 0x000000FF; /* blue mask */
               mask[1]              = 0x0000FF00; /* green mask */
               mask[0]              = 0x00FF0000; /* red mask */
               bmp->palette         = ( palette_t *)mask;
               bmp->boffset_blue    = 0;
               bmp->boffset_green   = 8;
               bmp->boffset_red     = 16;
               bmp->bsize_blue      = 8;
               bmp->bsize_green     = 8;
               bmp->bsize_red       = 8;
            }
            else /* BI_BITFILED */
            {
               if (bmp->palette == NULL) /* something wrong */
               {
                  set_error_code( bmp, NULL, GXERR_PALETTE_INFO);
                  return NULL;
               }
               calculate_boffset( bmp);
            }
            break;
   default:
            set_error_code( bmp, NULL, GXERR_COLOR_DEPTH);
            return NULL;
   }
   return bmp;
}

bmp_t *gx_bmp_create( int width, int height, int depth, int palette_size)
//-------------------------------------------------------------------------------
{
   bmp_t    *bmp;

   if ( NULL == ( bmp = malloc( sizeof( bmp_t))) )
   {
      set_error_code( bmp, NULL, GXERR_OUT_OF_MEMORY);
      return NULL;
   }
   memset( bmp, 0, sizeof( bmp_t));

   set_header( bmp, width, height, depth, palette_size);

   if ( NULL == ( bmp->data = malloc( bmp->bitmap_size)) )
   {
      set_error_code( bmp, NULL, GXERR_OUT_OF_MEMORY);
      return NULL;
   }

   return setup_bitmap( bmp);
}

bmp_t *gx_given_bmp_mastering( bmp_t *bmp , int width, int height, int depth, int palette_size)
//-------------------------------------------------------------------------------
{
   set_header( bmp, width, height, depth, palette_size);

   if ( NULL == ( bmp->data = malloc( bmp->bitmap_size)) )
   {
      set_error_code( bmp, NULL, GXERR_OUT_OF_MEMORY);
      return NULL;
   }

   return setup_bitmap( bmp);
}

void  gx_bmp_close( bmp_t *bmp)
//-------------------------------------------------------------------------------
// ����: bmp ����� ����
{
   release_dc( (dc_t *)bmp);
}

bmp_t *gx_bmp_open( char *filename)
//-------------------------------------------------------------------------------
// ����: bmp ������ �о� ����
// ����: bmp�� Į�� ���̿� ���߾� ���� �Լ� ����
// �μ�: fil
{
   FILE       *fp    = NULL;
   bmp_t   *bmp   = NULL;

   if ( NULL == ( bmp = malloc( sizeof( bmp_t))) )
   {
      set_error_code( bmp, NULL, GXERR_OUT_OF_MEMORY);
      return NULL;
   }

   memset( bmp, 0, sizeof( bmp_t));

   fp = fopen( filename, "r+b");
   if ( NULL == ( fp) )
   {
      set_error_code( bmp, fp, GXERR_NOFILE);
      return NULL;
   }

   if ( read_header( fp, bmp))
   {
      set_error_code( bmp, fp, GXERR_HEADER_INFO);
      return bmp;
   }

   if ( 0 != bmp->cnt_palette)
   {
      bmp->palette = ( palette_t *)malloc( sizeof( palette_t) * bmp->cnt_palette);
      memset( bmp->palette, 0, sizeof( palette_t) * bmp->cnt_palette);
      if ( read_palette(fp, bmp))
      {
         set_error_code( bmp, fp, GXERR_PALETTE_INFO);
         return NULL;
      }
   }

   if( read_data(fp, bmp))
   {
      set_error_code( bmp, fp, GXERR_READ_FILE);
      return NULL;
   }

   fclose(fp);
   return setup_bitmap( bmp);
}

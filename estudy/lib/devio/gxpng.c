/*******************************************************************************
������Ʈ : gxLib
��⳻�� : PNG ���
�������� : 2008-11-22
�ۼ���   : ��漮(jwjwmx@gmail.com)
���۱�   : �ּ� ������ �������� �ʴ� �� ���� ����
Ȩ������ : http://forum.falinux.com
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h>
#include <gx.h>
#include <gxpng.h>

typedef unsigned char   uch;
typedef unsigned short  ush;
typedef unsigned int    uln;
typedef unsigned long   ulg;

typedef struct rgb_t_ rgb_t;
struct rgb_t_
{
   unsigned char  red;
   unsigned char  green;
   unsigned char  blue;
};


static void  ch3_clear( png_t *png, color_t color)
//-------------------------------------------------------------------------------
{
    uch     *image_data;
    rgb_t   *ptr;                                                               // 3 ����Ʈ ũ���� �����͸� ����               
    rgb_t    color_rgb;                                                         // 3 ����Ʈ ũ���� Į���� �����. 
    int      ndx;
                          
    color_rgb.red   = color.red;
    color_rgb.green = color.green;
    color_rgb.blue  = color.blue;

    image_data  = png->mapped;
    ptr         = (rgb_t *)image_data;

    for ( ndx = 0; ndx < png->width*png->height; ndx++)                         
       *ptr++ = color_rgb;
}

static void ch3_get_pixel( png_t *png, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
{
   uch     *image_data;
   uch     *ptr;

   image_data  = png->mapped;
   ptr         = image_data + coor_y*png->bytes_per_line +coor_x*3;

   color->red     = *ptr++;
   color->green   = *ptr++;
   color->blue   = *ptr;
   color->alpha   = 255;
}

static void ch3_set_pixel( png_t *png, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
{
   uch     *image_data;
   uch     *ptr;

   image_data  = png->mapped;
   ptr         = image_data + coor_y*png->bytes_per_line +coor_x*3;

   *ptr++   = color.red  ;
   *ptr++   = color.green;
   *ptr     = color.blue;
}

static void  ch3_hline( png_t *png, int x_1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
{
   uch     *image_data;
   uch     *ptr;
   int      ndx;

   image_data  = png->mapped;
   ptr         = image_data + coor_y*png->bytes_per_line +x_1st*3;

   for ( ndx = x_1st; ndx <= x_2nd; ndx++)
   {
      *ptr++ = color.red  ;
      *ptr++ = color.green;
      *ptr++ = color.blue ;
   }
}

static void  ch3_vline( png_t *png, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
{
   uch     *image_data;
   uch     *ptr;
   uch     *ptr_bck;
   int      ndx;

   image_data  = png->mapped;
   ptr         = image_data + y_1st*png->bytes_per_line +coor_x*3;

   for ( ndx = y_1st; ndx <= y_2nd; ndx++)
   {
       ptr_bck = ptr;
      *ptr++ = color.red  ;
      *ptr++ = color.green;
      *ptr   = color.blue ;
       ptr   = ptr_bck +png->bytes_per_line;
   }
}


static void  ch4_clear( png_t *png, color_t color)
//-------------------------------------------------------------------------------
{                 
    uch        *image_data;
    color_t    *ptr;
    int         ndx;
    
    image_data  = png->mapped;
    ptr         = (color_t *)image_data;
    
    for ( ndx = 0; ndx < png->width*png->height; ndx++)
       *ptr++ = color;
}

static void ch4_get_pixel( png_t *png, int coor_x, int coor_y, color_t *color)
//-------------------------------------------------------------------------------
{
   uch     *image_data;
   uch     *ptr;

   image_data  = png->mapped;
   ptr         = image_data + coor_y*png->bytes_per_line +coor_x*4;

   color->red     = *ptr++;
   color->green   = *ptr++;
   color->blue    = *ptr++;
   color->alpha   = *ptr;
}

static void ch4_set_pixel( png_t *png, int coor_x, int coor_y, color_t color)
//-------------------------------------------------------------------------------
{
   uch     *image_data;
   uch     *ptr;

   image_data  = png->mapped;
   ptr         = image_data + coor_y*png->bytes_per_line +coor_x*4;

   *ptr++ = color.red  ;
   *ptr++ = color.green;
   *ptr++ = color.blue ;
   *ptr   = color.alpha;
}

static void  ch4_hline( png_t *png, int x_1st, int x_2nd, int coor_y, color_t color)
//-------------------------------------------------------------------------------
// ����: ������ �׸���.
{
   uch     *image_data;
   uch     *ptr;
   int      ndx;

   image_data  = png->mapped;
   ptr         = image_data + coor_y*png->bytes_per_line +x_1st*4;

   for ( ndx = x_1st; ndx <= x_2nd; ndx++)
   {
      *ptr++ = color.red  ;
      *ptr++ = color.green;
      *ptr++ = color.blue ;
      *ptr++ = color.alpha;
   }
}

static void  ch4_vline( png_t *png, int coor_x, int y_1st, int y_2nd, color_t color)
//-------------------------------------------------------------------------------
// ����: �������� �׸���.
{
   uch     *image_data;
   uch     *ptr;
   uch     *ptr_bck;
   int      ndx;

   image_data  = png->mapped;
   ptr         = image_data + y_1st*png->bytes_per_line +coor_x*4;

   for ( ndx = y_1st; ndx <= y_2nd; ndx++)
   {
       ptr_bck = ptr;
      *ptr++ = color.red  ;
      *ptr++ = color.green;
      *ptr++ = color.blue ;
      *ptr   = color.alpha;
       ptr   = ptr_bck +png->bytes_per_line;
   }
}

static void  release_dc( dc_t *dc)
//-------------------------------------------------------------------------------
{
   png_t  *png  = (png_t *)dc;

   if ( NULL == png)             return;
   if ( NULL != png->mapped)     free( png->mapped);
   free( png);
}

static void set_error_code( png_t *png, FILE *fp, int error_code)
//-------------------------------------------------------------------------------
// ����:
{
   gx_error_code  = error_code;
   release_dc( (dc_t *)png);
   if ( fp)    fclose(fp);
}

void  gx_png_close( png_t *png)
//-------------------------------------------------------------------------------
// ����: PNG ���� ��� ����
{
   release_dc( (dc_t *)png);
}

png_t  *gx_png_create( int width, int height)
// ����: ���Ͼ��� PNG ��ü�� ����
// ����: ä�� 3�� 4�� �и��Ͽ� ���� �Լ� ����
{                   
    png_t          *png      = NULL;
    uch            *image_data = NULL;
    int             image_rowbytes;
                    
    if ( NULL == ( png = malloc( sizeof( png_t))) )
    {
       set_error_code( png, 0, GXERR_OUT_OF_MEMORY);
       return NULL;
    }
    memset( png, 0, sizeof( png_t));
    
    png->dc_type        = DCTYPE_PNG;
    png->coor_x         = 0;                                                   // LineTo�� ���� ��ǥ�� 0���� �ʱ�ȭ
    png->coor_y         = 0;
    png->pen_color      = gx_color( 255, 255, 255, 255);                       // �⺻ �� ������ ���
    png->brush_color    = gx_color(   0,   0,   0, 255);                       // �⺻ �귯�� ������ ����
    png->release_dc     = release_dc;                                          // �Ҹ��� ���� �Լ�
    
    
    png->width          = width;
    png->height         = height;
    png->colors         = 8;                                                      // �켱 ����� ó��, bit_depth���� �־��
    png->color_type     = 6;                                                      // �켱 ����� ó��,
    png->bcolor.alpha   = 255;

    image_rowbytes      = 4 * width;                                            // r,g,b,a������ ���� �ȼ��� 4����Ʈ ����           
    
    png->bytes_per_line = image_rowbytes;                                       // ���δ� ����Ʈ ����
    png->bytes          = image_rowbytes*height;                                // ��ü ����Ʈ ����

    if ( NULL == (image_data = (uch *)malloc(png->bytes)))
    {
        set_error_code( png, 0, GXERR_OUT_OF_MEMORY);
        return NULL;
    }
    memset( image_data, 0, png->bytes);                                         // �̹��� ������ �ʱ�ȭ�Ѵ�.              

    //////////////////////////////////////////////////////////////////////////////
                               
    png->mapped         = image_data;
    png->clear          = ch4_clear;
    png->get_pixel      = ch4_get_pixel;
    png->set_pixel      = ch4_set_pixel;
    png->hline          = ch4_hline;
    png->vline          = ch4_vline;
    png->bits_per_pixel = 4 * 8;                                                // r,g,b,a������ ���� �ȼ��� 4����Ʈ ����           
    
    return png;
}

png_t *gx_png_open( char *filename)
// ����: PNG ������ �о� ����
// ����: ä�� 3�� 4�� �и��Ͽ� ���� �Լ� ����
{
   png_t         *png      = NULL;
   FILE          *fp       = NULL;

   uch            sig[8];
   png_structp    png_ptr  = NULL;
   png_infop      info_ptr = NULL;

   png_uint_32    ihdr_width, ihdr_height;
   int            bit_depth, color_type;
   png_color_16p  pBackground	= NULL;

   uch           *image_data = NULL;
   int            image_channels, image_rowbytes;
   double         display_exponent  = 1.0 * 2.2;

   double         gamma;
   png_bytepp     row_pointers = NULL;
   int            i;

   if ( NULL == ( png = malloc( sizeof( png_t))) )
   {
      set_error_code( png, fp, GXERR_OUT_OF_MEMORY);
      return NULL;
   }
   memset( png, 0, sizeof( png_t));

   png->dc_type         = DCTYPE_PNG;
   png->coor_x          = 0;                                                     // LineTo�� ���� ��ǥ�� 0���� �ʱ�ȭ
   png->coor_y          = 0;
   png->pen_color       = gx_color( 255, 255, 255, 255);                         // �⺻ �� ������ ���
   png->brush_color     = gx_color(   0,   0,   0, 255);                         // �⺻ �귯�� ������ ����
   png->release_dc      = release_dc;                                            // �Ҹ��� ���� �Լ�


   fp = fopen( filename, "rb");
   if ( NULL == ( fp) )                                                          // ������ ���ų� ���⿡ �����ߴٸ�
   {
      set_error_code( png, fp, GXERR_NOFILE);
      return NULL;
   }
   fread(sig, 1, 8, fp);
   if ( !png_check_sig( sig, 8))                                                 // PNG �ñ׳��� ����
   {
      set_error_code( png, fp, GXERR_SIGNATURE);
      return NULL;
   }
   png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);   // ����� ���� ���� �ڵ鷯�� ��� NULL�� ����
   if ( !png_ptr)
   {
      set_error_code( png, fp, GXERR_OUT_OF_MEMORY);
      return NULL;
   }

   info_ptr = png_create_info_struct( png_ptr);
   if ( !info_ptr)
   {
      set_error_code( png, fp, GXERR_OUT_OF_MEMORY);
      return NULL;
   }

   if ( setjmp( png_jmpbuf(png_ptr)))
   {
      png_destroy_read_struct( &png_ptr, &info_ptr, NULL);
      set_error_code( png, fp, GXERR_HEADER_INFO);
      return NULL;
   }

   png_init_io( png_ptr, fp);
   png_set_sig_bytes( png_ptr, 8);                                               // �̹� 8 ����Ʈ�� �ñ׳��� ���� �о����Ƿ� ������ �̵�
   png_read_info( png_ptr, info_ptr);                                            // �̹��� ������ ������ PNG ������ �о� ����

   /* alternatively, could make separate calls to png_get_image_width(),
    * etc., but want bit_depth and color_type for later [don't care about
    * compression_type and filter_type => NULLs] */

   png_get_IHDR(png_ptr, info_ptr, &ihdr_width, &ihdr_height, &bit_depth, &color_type, NULL, NULL, NULL);
   png->width        = ihdr_width;
   png->height       = ihdr_height;
   png->colors       = bit_depth;
   png->color_type   = color_type;
   
      //��׶�� Į�� ���Ѵ�. �Ǵ� ����ڰ� ���� ������ �� �� ������ ���⼭�� ����

   if ( setjmp( png_jmpbuf( png_ptr))) {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      set_error_code( png, fp, GXERR_PROCESSING);
      return NULL;
   }
// if ( !png_get_valid( png_ptr, info_ptr, PNG_INFO_bKGD))
// {
//    printf( "fails due to no bKGD chunk\n");
//    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
//    return;
// }
   png_get_bKGD(png_ptr, info_ptr, &pBackground);

   /* however, it always returns the raw bKGD data, regardless of any
    * bit-depth transformations, so check depth and adjust if necessary */

    if( pBackground != NULL )
    {
        if ( bit_depth == 16)
        {
           png->bcolor.red      = pBackground->red   >> 8;
           png->bcolor.green    = pBackground->green >> 8;
           png->bcolor.blue     = pBackground->blue  >> 8;
        }
        else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        {
           if      ( bit_depth == 1)  png->bcolor.red   = png->bcolor.green = png->bcolor.blue = pBackground->gray? 255 : 0;
           else if ( bit_depth == 2)  png->bcolor.red   = png->bcolor.green = png->bcolor.blue = (255/ 3) * pBackground->gray;
           else /* bit_depth == 4 */  png->bcolor.red   = png->bcolor.green = png->bcolor.blue = (255/15) * pBackground->gray;
        }
        else
        {
           png->bcolor.red   = (uch)pBackground->red;
           png->bcolor.green = (uch)pBackground->green;
           png->bcolor.blue  = (uch)pBackground->blue;
        }
    }
    png->bcolor.alpha    = 255;

   // ���� �̹��� �����͸� �о� ���δ�.

   /* setjmp() must be called in every function that calls a PNG-reading
    * libpng function */

   if  (setjmp( png_jmpbuf(png_ptr)))
   {
      png_destroy_read_struct( &png_ptr, &info_ptr, NULL);
      set_error_code( png, fp, GXERR_PROCESSING);
      return NULL;
   }

   /* expand palette images to RGB, low-bit-depth grayscale images to 8 bits,
    * transparency chunks to full alpha channel; strip 16-bit-per-sample
    * images to 8 bits per sample; and convert grayscale to RGB[A] */

   if (color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_expand(png_ptr);
   if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
      png_set_expand(png_ptr);
   if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
      png_set_expand(png_ptr);
   if (bit_depth == 16)
      png_set_strip_16(png_ptr);
   if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      png_set_gray_to_rgb(png_ptr);

   /* unlike the example in the libpng documentation, we have *no* idea where
    * this file may have come from--so if it doesn't have a file gamma, don't
    * do any correction ("do no harm") */

   if ( png_get_gAMA(png_ptr, info_ptr, &gamma))
      png_set_gamma(png_ptr, display_exponent, gamma);

   /* all transformations have been registered; now update info_ptr data,
    * get rowbytes and channels, and allocate image memory */

   png_read_update_info( png_ptr, info_ptr);

   image_rowbytes = png_get_rowbytes(png_ptr, info_ptr);
   image_channels = (int)png_get_channels(png_ptr, info_ptr);

   png->bytes_per_line  = image_rowbytes;                                        // ���δ� ����Ʈ ����
   png->bytes           = image_rowbytes*png->height;                            // ��ü ����Ʈ ����

   if ( NULL == (image_data = (uch *)malloc(png->bytes)))
   {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      set_error_code( png, fp, GXERR_OUT_OF_MEMORY);
      return NULL;
   }
   if ( NULL == (row_pointers = (png_bytepp)malloc(sizeof( png_bytep)*png->height)))
   {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      free(image_data);
      set_error_code( png, fp, GXERR_OUT_OF_MEMORY);
      return NULL;
   }

   /* set the individual row_pointers to point at the correct offsets */
   for (i = 0;  i < png->height; i++)
      row_pointers[i] = image_data + image_rowbytes*i;

   /* now we can go ahead and just read the whole image */

   png_read_image( png_ptr, row_pointers);

   /* and we're done!  (png_read_end() can be omitted if no processing of
    * post-IDAT text/time/etc. is desired) */

   free( row_pointers);
   row_pointers = NULL;
   png_read_end( png_ptr, NULL);

   //////////////////////////////////////////////////////////////////////////////

   png->mapped = image_data;
   if ( image_channels == 3)
   {
      png->clear           = ch3_clear;
      png->get_pixel       = ch3_get_pixel;
      png->set_pixel       = ch3_set_pixel;
      png->hline           = ch3_hline;
      png->vline           = ch3_vline;
      png->bits_per_pixel  = 3 * 8;
   }
   else /* if (image_channels == 4) */
   {
      png->clear           = ch4_clear;
      png->get_pixel       = ch4_get_pixel;
      png->set_pixel       = ch4_set_pixel;
      png->hline           = ch4_hline;
      png->vline           = ch4_vline;
      png->bits_per_pixel  = 4 * 8;
   }
   png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
   fclose(fp);

   return png;
}

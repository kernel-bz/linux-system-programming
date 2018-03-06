#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gx.h>
#include <gxpng.h>
#include <gximagelist.h>

image_list_t *gx_imagelist_create( char  *filename, int width, int height)
// ����: png ������ �̿��Ͽ� �̹��� ����Ʈ ����
{
    image_list_t   *list = NULL;
    png_t          *png_sour;
    int             coor_x  , coor_y;
    int             wid_sour, hgt_sour;
    int             cnt_x, cnt_y;
    int             ndx;

    if  ( !( png_sour = gx_png_open( filename)))
        return NULL;
    else
    {
        cnt_x   = png_sour->width  / width;
        cnt_y   = png_sour->height / height;
        list    = malloc( sizeof( image_list_t));
        list->count = cnt_x *cnt_y;
        list->array = malloc( sizeof( png_t *) *list->count);

        coor_y    = 0;
        wid_sour  = png_sour->width;
        hgt_sour  = png_sour->height;

        ndx = 0;
        while( coor_y < hgt_sour)
        {
            coor_x      = 0;
            while( coor_x < wid_sour)
            {
                list->array[ndx] = gx_png_create( width, height);
                gx_bitblt( ( dc_t *)list->array[ndx], 0, 0, (dc_t *)png_sour, coor_x, coor_y, width, height);
                coor_x += width;
                ndx++;
            }
            coor_y += height;
        }
        gx_png_close( png_sour);
    }
    return  list;
}

png_t *gx_imagelist_get( image_list_t *list, int index)
// ����: �ε��� ��ȣ�� �ش��ϴ� �̹��� ���� ��ȯ
{
    if ( index < list->count)
        return list->array[index];
    else
        return NULL;
}

void gx_imagelist_close( image_list_t *list)
// ����: �̹��� ����Ʈ ��ü �Ҹ�
{
    int     ndx;

    if ( NULL != list)
    {
        for ( ndx = 0; ndx < list->count; ndx++)
            gx_png_close( gx_imagelist_get( list, ndx));
        free( list);
    }
}

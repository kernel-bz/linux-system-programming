#ifndef _GX_IMAGE_LIST_H_
#define _GX_IMAGE_LIST_H_

#include <gx.h>
#include <gxpng.h>

typedef struct image_list_t_ image_list_t;
struct image_list_t_
{
   int   count;
   
   png_t **array;
};

extern image_list_t  *gx_imagelist_create( char  *filename, int width, int height); // png ������ �̿��Ͽ� �̹��� ����Ʈ ����
extern png_t *gx_imagelist_get( image_list_t *list, int index);                // �ε��� ��ȣ�� �ش��ϴ� �̹��� ���� ��ȯ
extern void gx_imagelist_close( image_list_t *list);                            // �̹��� ����Ʈ ��ü �Ҹ�

#endif

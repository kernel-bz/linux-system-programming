#ifndef _GX_PNG_H_
#define _GX_PNG_H_

#include <gx.h>

typedef struct png_t_ png_t;
struct png_t_
{
   char     dc_type;                                                             // DC�� ���·� Screen, Bitmap�� �����Ѵ�.
   int      width;                                                               // ��Ʈ ������ ��
   int      height;                                                              // ��Ʈ ������ ����
   int      dots;                                                                // ��ü ��Ʈ ���� width * height
   int      bytes;                                                               // �޸��� ��ü Byte ũ��
   int      colors;                                                              // Į�� ����
   int      bytes_per_line;                                                      // ���δ� ����Ʈ ����
   int      bits_per_pixel;                                                      // ��Ʈ�� �ȼ� ����
   int      coor_x;                                                              // ������ �׸��� �ߴ� ������ ��ǥ
   int      coor_y;                                                              // ������ �׸��� �ߴ� ������ ��ǥ
   color_t  pen_color;                                                           // ������ �� Į��
   color_t  brush_color;                                                         // ������ �귯�� Į��
   void    *mapped;                                                              // �޸� ���ε� ������

   void (*release_dc)( dc_t *dc);                                                // Device Context �Ҹ� �� ���� �޸𸮸� ����
   void (*clear    )( png_t *png, color_t color);                                // ������ ��ü ĥ�ϱ�
   void (*get_pixel)( png_t *png, int coor_x, int coor_y, color_t   *color );    // Į�� ���� �о� ����
   void (*set_pixel)( png_t *png, int coor_x, int coor_y, color_t    color );    // �� ���
   void (*hline    )( png_t *png, int x1st  , int x_2nd , int coor_y, color_t color);// ���� �߱�
   void (*vline    )( png_t *png, int coor_x, int y_1st , int y_2nd , color_t color);// ������ �߱�

   color_t  bcolor;                                                              // ��� Į�� �� ����
   int   color_type;
};

extern png_t  *gx_png_open  ( char  *filename);             	                  // png ��ü�� ������ �Ŀ�, ������ ����
extern void    gx_png_close ( png_t *ppng);                                      // ������ Ŭ���� �� png ��ü �Ҹ�

#endif

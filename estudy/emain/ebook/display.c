//
// ���ϸ�		: /ebook/display.c
// ���			: /����å/ȭ�����, �޴���ǥó��
// ��Ű����	: MIPS
// �ۼ���		: ������(rgbi3307@nate.com)
// ���۱�		: (c) Ŀ�ο���ȸ(http://www.kernel.bz/) ������
// �����ڵ�	: 2009-05-18
// �ֱټ���	: 2009-05-20
// ���ξ�		: dp_

#include <stdlib.h>
#include "display.h"


//ȭ�鿡 �۾����μ��� ��ǥ����
BOX_PROCESS* dp_process_create (int x, int y, int width, int height, int row, int colum)
{
	BOX_PROCESS* process;

	process = (BOX_PROCESS*) malloc (sizeof(BOX_PROCESS));
	if (process) {
		process->x = x;
		process->y = y;
		process->width = width;
		process->height = height;
		process->row = row;
		process->colum = colum;

		process->status = BP_CREATE;
	}
	return process;
}

//�۾������� ��ǥ����
BOX_LOCATION* dp_box_create (int x, int y, int width, int height)
{
	BOX_LOCATION* box;

	box = (BOX_LOCATION*) malloc (sizeof(BOX_LOCATION));
	if (box) {
		box->x = x;
		box->y = y;
		box->width = width;
		box->height = height;		
	}
	return box;
}

//�޴��� ��ġ �Ǿ����� �Ǵ�
int dp_touch_menu (int tx, int ty, BOX_PROCESS *prsMenu)
{
	int	x, y, irow;

	x    = prsMenu->x;
	y    = prsMenu->y;
	irow = prsMenu->row * 2;
	if ((ty > y) && (ty < y+irow)) {
		if ((tx > x) && (tx < x+100)) return 1;		//���θ޴�
		if ((tx > x+110) && (tx < x+180)) return 2; //�ڵ��÷���
		if ((tx > x+190) && (tx < x+260)) return 3; //�����ϱ�
	}
	return 0;
}

//�н� Chapter�� ��ġ �Ǿ����� �Ǵ�
int dp_touch_menu_chapter (int tx, int ty, int sy, int ipage, int row_max, int row_height)
{
	int	idx;

	if (tx < 1) return -1;
	idx = ty / row_height - sy;
	if ((idx < 0) || (idx > row_max-sy)) return -1;

	idx = (ipage-1) * row_max + idx;
	return idx;
}

//��ġ�Է� ��ġ �Ǵ�
//�Է� �Ű�����
//	ans: �����ȣ
//	tx,ty: ��ġ��ǥ
//	boxBookPic: �׸���ġ ����ü
//	��ġ�� �簢�� ��ǥ
//���: 1=����, 0=����
int dp_touch_position (int ans, int tx, int ty, BOX_LOCATION* boxBookPic, int *x1, int *y1, int *x2, int *y2)
{
	int	x, y, xc, yc, iw, ih;
	int ir;

	x  = boxBookPic->x;
	y  = boxBookPic->y;
	iw = boxBookPic->width / 2;	//������
	ih = boxBookPic->height / 2;	//��������
	xc = boxBookPic->x + iw;		//�߽�(X)
	yc = boxBookPic->y + ih;		//�߽�(Y)

	if ((tx > x) && (tx < xc+iw) && (ty > y) && (ty < yc+ih)) {
		if ((tx > x) && (tx < xc) && (ty > y) && (ty < yc)) {
			ir = 1;
			*x1=x; *x2=xc; *y1=y; *y2=yc;
		}
		else if ((tx > xc) && (tx < xc+iw) && (ty > y) && (ty < yc)) {
			ir = 2;
			*x1=xc; *x2=xc+iw; *y1=y; *y2=yc;
		}
		else if ((tx > x) && (tx < xc) && (ty > yc) && (ty < yc+ih)) {
			ir = 3;
			*x1=x; *x2=xc; *y1=yc; *y2=yc+ih;
		}
		else if ((tx > xc) && (tx < xc+iw) && (ty > yc) && (ty < yc+ih)) {
			ir = 4;
			*x1=xc; *x2=xc+iw; *y1=yc; *y2=yc+ih;
		}
		else ir = 0;

		if (ans == ir) return 1;	//����
		else return 0;				//����
	}
	return -1;
}

int dp_touch_position_auto (int ans, BOX_LOCATION* boxBookPic, int *x1, int *y1, int *x2, int *y2)
{
	int	x, y, xc, yc, iw, ih;
	int ir;

	x  = boxBookPic->x;
	y  = boxBookPic->y;
	iw = boxBookPic->width / 2;	//������
	ih = boxBookPic->height / 2;	//��������
	xc = boxBookPic->x + iw;		//�߽�(X)
	yc = boxBookPic->y + ih;		//�߽�(Y)

	ir = 1;
	if (ans == 1) {
		*x1=x; *x2=xc; *y1=y; *y2=yc;
	}
	else if (ans == 2) {
		*x1=xc; *x2=xc+iw; *y1=y; *y2=yc;
	}
	else if (ans == 3) {
		*x1=x; *x2=xc; *y1=yc; *y2=yc+ih;
	}
	else if (ans == 4) {
		*x1=xc; *x2=xc+iw; *y1=yc; *y2=yc+ih;
	}
	else ir = 0;

	return ir;
}


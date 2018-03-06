//
// ���ϸ�		: /include/ebook/display.h
// ���			: /����å/ȭ�����, �޴���ǥó��
// ��Ű����	: MIPS
// �ۼ���		: ������(rgbi3307@nate.com)
// ���۱�		: (c) Ŀ�ο���ȸ(http://www.kernel.bz/) ������
// �����ڵ�	: 2009-05-18
// �ֱټ���	: 2009-05-18
// 

#ifndef __DISPLAY_H
#define __DISPLAY_H

#define ROW 24	//�������
#define COL 15	//��ĭ����

#define PICW 200 //�׸�(����)ũ��
#define PICH 150

//�۾�(���μ���) ����
#define MN_CHAPTER	100		//�н����
#define MN_PREVIEW	101		//�̸�����
#define MN_RECORD	102		//�����ϱ�
#define MN_TOUCH	103		//�۾���ġ

#define BP_CREATE	200
#define BP_CLOSE	201
#define BP_VIEW		202
#define BP_RUNNING	203
#define BP_FINISH	204
#define BP_STOP		205
#define BP_PAUSE	206
#define BP_TOUCH	207


//���μ����� ȭ����ġ
typedef struct {
	int		x;		//left
	int		y;		//top
	int		width;	//��
	int		height;	//����
	int		row;	//���ٳ���
	int		colum;	//��ĭ��

	int		status;
} BOX_PROCESS;

//�������� ȭ����ġ
typedef struct {
	int		x;		//left
	int		y;		//top
	int		width;
	int		height;
} BOX_LOCATION;	


//ȭ�鿡 �۾����μ��� ��ǥ����
BOX_PROCESS* dp_process_create (int x, int y, int width, int height, int row, int colum);
//�۾������� ��ǥ����
BOX_LOCATION* dp_thread_create (int x, int y, int width, int height);
//�޴��� ��ġ �Ǿ����� �Ǵ�
int dp_touch_menu (int tx, int ty, BOX_PROCESS *prsMain);
//�н� Chapter�� ��ġ �Ǿ����� �Ǵ�
int dp_touch_menu_chapter (int tx, int ty, int sy, int ipage, int row_max, int row_height);
//��ġ�Է� ��ġ �Ǵ�
int dp_touch_position (int ans, int tx, int ty, BOX_LOCATION* boxBookPic, int *x1, int *y1, int *x2, int *y2);
int dp_touch_position_auto (int ans, BOX_LOCATION* boxBookPic, int *x1, int *y1, int *x2, int *y2);


#endif

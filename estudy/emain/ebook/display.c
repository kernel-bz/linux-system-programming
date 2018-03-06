//
// 파일명		: /ebook/display.c
// 기능			: /전자책/화면출력, 메뉴좌표처리
// 아키텍쳐	: MIPS
// 작성자		: 정재준(rgbi3307@nate.com)
// 저작권		: (c) 커널연구회(http://www.kernel.bz/) 정재준
// 최초코딩	: 2009-05-18
// 최근수정	: 2009-05-20
// 접두어		: dp_

#include <stdlib.h>
#include "display.h"


//화면에 작업프로세스 좌표생성
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

//작업쓰레드 좌표생성
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

//메뉴가 터치 되었는지 판단
int dp_touch_menu (int tx, int ty, BOX_PROCESS *prsMenu)
{
	int	x, y, irow;

	x    = prsMenu->x;
	y    = prsMenu->y;
	irow = prsMenu->row * 2;
	if ((ty > y) && (ty < y+irow)) {
		if ((tx > x) && (tx < x+100)) return 1;		//메인메뉴
		if ((tx > x+110) && (tx < x+180)) return 2; //자동플레이
		if ((tx > x+190) && (tx < x+260)) return 3; //발음하기
	}
	return 0;
}

//학습 Chapter가 터치 되었는지 판단
int dp_touch_menu_chapter (int tx, int ty, int sy, int ipage, int row_max, int row_height)
{
	int	idx;

	if (tx < 1) return -1;
	idx = ty / row_height - sy;
	if ((idx < 0) || (idx > row_max-sy)) return -1;

	idx = (ipage-1) * row_max + idx;
	return idx;
}

//터치입력 위치 판단
//입력 매개변수
//	ans: 정답번호
//	tx,ty: 터치좌표
//	boxBookPic: 그림위치 구조체
//	터치된 사각형 좌표
//출력: 1=정답, 0=오답
int dp_touch_position (int ans, int tx, int ty, BOX_LOCATION* boxBookPic, int *x1, int *y1, int *x2, int *y2)
{
	int	x, y, xc, yc, iw, ih;
	int ir;

	x  = boxBookPic->x;
	y  = boxBookPic->y;
	iw = boxBookPic->width / 2;	//사진폭
	ih = boxBookPic->height / 2;	//사진높이
	xc = boxBookPic->x + iw;		//중심(X)
	yc = boxBookPic->y + ih;		//중심(Y)

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

		if (ans == ir) return 1;	//정답
		else return 0;				//오답
	}
	return -1;
}

int dp_touch_position_auto (int ans, BOX_LOCATION* boxBookPic, int *x1, int *y1, int *x2, int *y2)
{
	int	x, y, xc, yc, iw, ih;
	int ir;

	x  = boxBookPic->x;
	y  = boxBookPic->y;
	iw = boxBookPic->width / 2;	//사진폭
	ih = boxBookPic->height / 2;	//사진높이
	xc = boxBookPic->x + iw;		//중심(X)
	yc = boxBookPic->y + ih;		//중심(Y)

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


//
// 파일명		: /include/ebook/display.h
// 기능			: /전자책/화면출력, 메뉴좌표처리
// 아키텍쳐	: MIPS
// 작성자		: 정재준(rgbi3307@nate.com)
// 저작권		: (c) 커널연구회(http://www.kernel.bz/) 정재준
// 최초코딩	: 2009-05-18
// 최근수정	: 2009-05-18
// 

#ifndef __DISPLAY_H
#define __DISPLAY_H

#define ROW 24	//한행높이
#define COL 15	//한칸길이

#define PICW 200 //그림(사진)크기
#define PICH 150

//작업(프로세스) 상태
#define MN_CHAPTER	100		//학습목록
#define MN_PREVIEW	101		//미리보기
#define MN_RECORD	102		//녹음하기
#define MN_TOUCH	103		//작업터치

#define BP_CREATE	200
#define BP_CLOSE	201
#define BP_VIEW		202
#define BP_RUNNING	203
#define BP_FINISH	204
#define BP_STOP		205
#define BP_PAUSE	206
#define BP_TOUCH	207


//프로세스의 화면위치
typedef struct {
	int		x;		//left
	int		y;		//top
	int		width;	//폭
	int		height;	//높이
	int		row;	//한줄높이
	int		colum;	//한칸폭

	int		status;
} BOX_PROCESS;

//쓰레드의 화면위치
typedef struct {
	int		x;		//left
	int		y;		//top
	int		width;
	int		height;
} BOX_LOCATION;	


//화면에 작업프로세스 좌표생성
BOX_PROCESS* dp_process_create (int x, int y, int width, int height, int row, int colum);
//작업쓰레드 좌표생성
BOX_LOCATION* dp_thread_create (int x, int y, int width, int height);
//메뉴가 터치 되었는지 판단
int dp_touch_menu (int tx, int ty, BOX_PROCESS *prsMain);
//학습 Chapter가 터치 되었는지 판단
int dp_touch_menu_chapter (int tx, int ty, int sy, int ipage, int row_max, int row_height);
//터치입력 위치 판단
int dp_touch_position (int ans, int tx, int ty, BOX_LOCATION* boxBookPic, int *x1, int *y1, int *x2, int *y2);
int dp_touch_position_auto (int ans, BOX_LOCATION* boxBookPic, int *x1, int *y1, int *x2, int *y2);


#endif

//
// 파일명		: /ebook/ebook.c
// 기능			: 전자책 메인
// 아키텍쳐	: MIPS
// 작성자		: 정재준(rgbi3307@nate.com)
// 저작권		: (c) 커널연구회(http://www.kernel.bz/) 정재준
// 최초코딩	: 2009-04-22
// 최근수정	: 2009-06-18
// 

#include <stdio.h>
#include <linux/input.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>

//devio
#include "gx.h"
#include "gxcolor.h"
#include "ex.h"
#include "ix.h"
#include "gxbdf.h"
#include "gxjpg.h"
#include "ux_lib.h"

//ebook
#include "queue.h"
#include "list.h"
#include "word_page.h"
#include "display.h"

//fileio
#include "sound.h"


// 전역 변수 
//-----------------------------------------------------------------------------
static dc_t*	dcScreen;
QUEUE*			queueWordPage;	//학습페이지
LIST*			listChapter;	//학습목록
//화면작업 좌표
BOX_PROCESS*	prsMenu;
BOX_PROCESS*	prsBook;
BOX_LOCATION*	boxBookTop;
BOX_LOCATION*	boxBookLeft;
BOX_LOCATION*	boxBookPic;

int				iCnt1, iCnt0;				//정답,오답수
int				idxPage, idxChapter;		//학습목록 페이지

pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;


//화면좌표 설정
void screen_coordinate (void)
{
	int	x, y;

	prsMenu = dp_process_create (0, dcScreen->height-ROW*2, dcScreen->width, ROW*2, ROW, COL);
	prsBook = dp_process_create (0, 0, dcScreen->width, dcScreen->height-ROW*2, ROW, COL);

	boxBookTop = dp_box_create (0, 0, dcScreen->width, ROW);
	boxBookLeft = dp_box_create (0, ROW+1, 150, prsBook->height-(ROW+1));

	//그림(사진) 위치 좌표
	x = boxBookLeft->width + prsBook->colum*3;
	y = boxBookLeft->y + prsBook->row*3;
	
	boxBookPic = dp_box_create (x, y, PICW*2, PICH*2);
}

void screen_coord_refresh (int x, int y)
{
	int	x2, y2;

	prsBook->x = x;
	prsBook->y = y;

	boxBookTop->x = prsBook->x;
	boxBookTop->y = prsBook->y;

	boxBookLeft->x = prsBook->x;
	boxBookLeft->y = prsBook->y + ROW+1;

	//그림(사진) 위치 좌표
	x2 = boxBookLeft->width + prsBook->colum*3;
	y2 = boxBookLeft->y + prsBook->row*3;

	boxBookPic->x = x2;
	boxBookPic->y = y2;
}

//JPEG 이미지 출력
//-----------------------------------------------------------------------------
int lib_jpg_display (dc_t *dc_screen, char *fname, int dx, int dy)
{
	int		iresult = 0;
	jpg_t	*jpg_img;

	if ((jpg_img = gx_jpg_open (fname))) {
		gx_bitblt (dc_screen, dx, dy, 
					(dc_t *)jpg_img, 0, 0, jpg_img->width, jpg_img->height);
	} else {
		gx_print_error ( "gx_jpg_open(): error!");
		iresult = -1;
	}

	//printf ("jpg filename: %s\n", fname); TRACE;
	gx_jpg_close (jpg_img);
	return iresult;
}

void menu_chapter_view (int ipage, int iselect)
{
	char		path[80];
	int			x, y, col, row;
	int			row_toal, row_limit, row_start, row_end, row_cnt, row_idx;
	CHAPTER*	Chapter;

	x = prsBook->x;
	y = prsBook->y;
	col = prsBook->colum;	//한칸
	row = prsBook->row;		//한행

	//백그라운드 메뉴
	sprintf (path, "%s%s", PATH_DATA, "bgd/menu/bgd_menu01.jpg");
	lib_jpg_display (dcScreen, path, x, y);

	dcScreen->pen_color = CL_BLACK;
	gx_text_out (dcScreen, x+col, row, "재밌는 영어학습 메뉴");

	//학습목록 출력
	row_cnt = 0;
	row_idx = 0;

	row_toal = listCount (listChapter);
	row_limit = dcScreen->height / row - 5;	//20-5
	row_end = ipage * row_limit;		//ipage * 15
	row_start = row_end - row_limit;

	printf ("menu_chapter_view(): %d,%d_%d\n", ipage, row_start, row_end);

	if (row_toal > 0) {
		traverse (listChapter, 0, (void**)&Chapter);
		do {
			row_cnt++;
			if ((row_cnt > row_start) && (row_cnt < row_end)) {
				row_idx++;
				gx_text_out (dcScreen, x+(Chapter->space+3)*col, row * (2+row_idx), Chapter->title);
			} else {
				break;
			}
		} while (traverse (listChapter, 1, (void**)&Chapter));
	}

	if (iselect > 0) { //선택된 목록표시
		row_idx = iselect - row_start;
		/*
		gx_text_out (dcScreen
					, x + col
					, row * (2+row_idx)
					, Chapter->ch[iselect]);
		*/
		gx_hline (dcScreen
					, x + col
					, prsBook->width - col * 2
					, row * (2+row_idx)
					, CL_BLUE);
	}
}

void word_page_left (char* sSection)
{
	int		x, y, col, row, itotal, iscore;
	char	sline[50];	

	x = boxBookLeft->x;
	y = boxBookLeft->y;
	col = prsBook->colum;	//한칸
	row = prsBook->row;		//한행

	dcScreen->pen_color = CL_SILVER;
	dcScreen->brush_color = CL_BLACK;
	gx_rectangle (dcScreen, x, y, x+boxBookLeft->width, y+boxBookLeft->height);

	//gx_text_out (dcScreen, x+col*10, row, Chapter->ch[idx]);
	gx_text_out (dcScreen, x+col, y + row, sSection);

	//점수계산
	itotal = queueCount (queueWordPage) + iCnt1 + iCnt0;
	iscore = (iCnt1 * 100) / itotal;

	sprintf (sline, "문제수:%d/%d", queueCount (queueWordPage), queueCount (queueWordPage) + iCnt1);
	gx_text_out (dcScreen, x+col, y + row*3, sline);
	sprintf (sline, "정답수:%d", iCnt1);
	gx_text_out (dcScreen, x+col, y + row*4, sline);
	sprintf (sline, "오답수:%d", iCnt0);
	gx_text_out (dcScreen, x+col, y + row*5, sline);	
	sprintf (sline, "* 점 수:%d", iscore);
	gx_text_out (dcScreen, x+col, y + row*6, sline);
}

//단어학습 페이지 표시
//리턴: 정답번호
//-----------------------------------------------------------------------------
int word_page_display (WORD1_PAGE* word_page)
{
	char			path_fname[256], section[20];
	int				x, y, x2, y2;
	SND_FILE		snd_file; 

	//왼쪽메뉴 출력
	sprintf (section, "ch_%d_%d", idxChapter, word_page->idx);
	word_page_left (section);

	//쿼리출력 좌표
	x = boxBookLeft->x + boxBookLeft->width + prsBook->colum;
	y = boxBookLeft->y + prsBook->row;
	dcScreen->pen_color = CL_SILVER;
	dcScreen->brush_color = CL_WHITE;
	//쿼리출력
	gx_rectangle (dcScreen, x-5, y-20, prsBook->width - 20, y + prsBook->colum*2);	
	dcScreen->pen_color = CL_BLACK;
	gx_text_out (dcScreen, x, y, word_page->query);	

	//그림(사진) 출력 좌표
	x = boxBookPic->x;
	y = boxBookPic->y;
	x2 = boxBookPic->x + boxBookPic->width / 2;
	y2 = boxBookPic->y + boxBookPic->height / 2;

	//pic1
	sprintf (path_fname, "%s%s", word_page->path, word_page->pic1);
	lib_jpg_display (dcScreen, path_fname, x, y);
	//pic2
	sprintf (path_fname, "%s%s", word_page->path, word_page->pic2);
	lib_jpg_display (dcScreen, path_fname, x2, y);
	//pic3
	sprintf (path_fname, "%s%s", word_page->path, word_page->pic3);
	lib_jpg_display (dcScreen, path_fname, x, y2);
	//pic4
	sprintf (path_fname, "%s%s", word_page->path, word_page->pic4);
	lib_jpg_display (dcScreen, path_fname, x2, y2);
	
	//그림 중심선
	gx_hline (dcScreen, x, x + boxBookPic->width, y2, CL_BLACK);
	gx_vline (dcScreen, x2, y, y + boxBookPic->height, CL_BLACK);
	//그림 테두리선(가로)
	gx_hline (dcScreen, x, x + boxBookPic->width, y, CL_BLACK);
	gx_hline (dcScreen, x, x + boxBookPic->width, y + boxBookPic->height, CL_BLACK);
	//그림 테두리선(세로)
	gx_vline (dcScreen, x, y, y + boxBookPic->height, CL_BLACK);
	gx_vline (dcScreen, x + boxBookPic->width, y, y + boxBookPic->height, CL_BLACK);	

	//사운드 출력
	//-------------------------------------------------------------------------
	strcpy (snd_file.fname, section);
	snd_file.length = strlen(word_page->query); //문자열 길이(사운드 길이)
	printf ("snd_file.fname = %s\n", snd_file.fname);
	//tid = pthread_create (&thr, NULL, snd_file_listening, (void*)&snd_file);
	snd_file_listening ((void*)&snd_file);

	sleep (word_page->timeout);

	return 1;
}

//페이지를 보고 말하기
//-----------------------------------------------------------------------------
int word_page_speak (WORD1_PAGE* word_page)
{
	char			section[20], path_fname[128];
	int				x, y, x2, y2, row;
	unsigned char*	buf_str;	
	unsigned int	length;
	SND_FILE		snd_file; 

	//왼쪽메뉴 출력
	sprintf (section, "ch_%d_%d", idxChapter, word_page->idx);
	word_page_left (section);

	//출력 좌표
	x = boxBookLeft->x + boxBookLeft->width + prsBook->colum;
	y = boxBookLeft->y;	
	row = prsBook->row;
	x2 = x + boxBookPic->width;
	y2 = y + boxBookPic->height + row*2;

	dcScreen->pen_color = CL_BLACK;
	//dcScreen->brush_color = CL_WHITE;
	gx_text_out (dcScreen, x, y + row, "Speaking...");
	gx_hline (dcScreen, x, x2, y + row*2, CL_GRAY);

	//그림(사진) 출력
	if (word_page->ans == 1)
		sprintf (path_fname, "%s%s", word_page->path, word_page->pic1);
	else if (word_page->ans == 2)
		sprintf (path_fname, "%s%s", word_page->path, word_page->pic2);
	else if (word_page->ans == 3)
		sprintf (path_fname, "%s%s", word_page->path, word_page->pic3);
	else if (word_page->ans == 4)
		sprintf (path_fname, "%s%s", word_page->path, word_page->pic4);
	lib_jpg_display (dcScreen, path_fname, x, y + row*3);

	//쿼리(speaking) 출력
	//gx_rectangle (dcScreen, x-5, y-20, prsBook->width - 20, y + prsBook->colum*2);

	dcScreen->pen_color = CL_BLACK;
	length = strlen(word_page->query); //문자열 길이
	buf_str = malloc (length + 12);
	sprintf (buf_str, "%s%s", "*Say: ", word_page->query);
	gx_text_out (dcScreen, x, y2, buf_str);

	//듣고 따라서 말하기
	//if (snd_file_listening_say (section, length) < 0) return -1;

	//사운드 녹음(저장)
	/*
	if (snd_file_recording (section, length) > 0) {
		dcScreen->pen_color = CL_BLUE;
		sprintf (buf_str, "%s%s", "*You said: ", word_page->query);
		gx_text_out (dcScreen, x, y2 + row*3, buf_str);		
	} else {
		//return -1;
	}
	*/
	//사운드 녹음(저장)
	//-------------------------------------------------------------------------
	strcpy (snd_file.fname, section);
	snd_file.length = length; //문자열 길이(사운드 길이)
	//tid = pthread_create (&thr, NULL, snd_file_recording, (void*)&snd_file);
	snd_file_recording ((void*)&snd_file);

	sleep (word_page->timeout);

	free (buf_str); //지역변수
	return 1;
}


//학습용 페이지 읽어오기 (전역변수 큐에 할당)
//-----------------------------------------------------------------------------
int word_page_init (int idx)
{
	CHAPTER		Chapter;
	CHAPTER*	ChapterOut;
	int			found;
	char		path_fname[128];

	Chapter.idx = idx;
	found = searchList (listChapter, &Chapter, (void**)&ChapterOut);
	if (found) {
		printf ("%d:%s,%s\n", ChapterOut->idx, ChapterOut->title, ChapterOut->file);
	} else {
		printf ("word_page_init(): list index fail!\n");
		return -1;
	}

	if (queueWordPage) destroyQueue (queueWordPage);

	sprintf (path_fname, "%s%s", ChapterOut->path, ChapterOut->file);
	idxChapter = idx;	//학습목록 번호

	queueWordPage = wp_page_create (path_fname);
	if (queueCount (queueWordPage) < 1) return -1;

	printf ("word_page_init(%s)\n", path_fname);

	//백그라운드 이미지출력
	sprintf (path_fname, "%s%s", PATH_DATA, "bgd/menu/bgd_menu02.jpg");
	lib_jpg_display (dcScreen, path_fname, 0, 0);	

	//점수
	iCnt1 = 0;
	iCnt0 = 0;	

	return 0;
}


//학습 페이지 자동 플레이
//-----------------------------------------------------------------------------
int word_page_auto_play (int idx)
{
	WORD1_PAGE*	word_page;
	int			ans;	//(1:정답, 0:오답)
	int			x1, y1, x2, y2;	//반환되는 그림좌표

	if (word_page_init (idx) < 0) return -1;

	//큐의 제일 앞부분 구조체 가져옴
	while (dequeue (queueWordPage, (void**)&word_page)) 
	{
		if (prsBook->status == BP_STOP) return 0; //메인메뉴 터치

		//정답번호로 그림좌표 자동설정
		ans = dp_touch_position_auto (word_page->ans, boxBookPic, &x1, &y1, &x2, &y2);
		if (ans == 1) {
			word_page_display (word_page);

			//정답 테두리 표시			
			dcScreen->pen_color = CL_BLUE;
			dcScreen->brush_color = CL_BLUE;
			gx_box (dcScreen, x1,   y1,   x2,   y2);
			gx_box (dcScreen, x1+1, y1+1, x2-1, y2-1);
			gx_box (dcScreen, x1+2, y1+2, x2-2, y2-2);
			//gx_circle (dcScreen, x1 + boxBookPic->width / 4, y1 + boxBookPic->height / 4, 20);
			iCnt1++;			
			sleep (1);			
		}
	}

	return 1;
}

//메뉴 페이지 번호 자동으로 가져오기
//-----------------------------------------------------------------------------
void word_page_auto_run (void* data)
{
	int i, rc, result;
	int chapter = *((int*)data);

	rc = pthread_mutex_lock(&a_mutex);

	result = 1;
	if (chapter > 0) { //지정된 것만 
		result = word_page_auto_play (chapter);

	} else {
		while (result > 0) //전체 반복
		{
			for (i = 0; i < listCount (listChapter); i++) {
				menu_chapter_view (i/(dcScreen->height/ROW-5)+1, i+1);
				sleep (1);
				result = word_page_auto_play (i+1);
				if (result < 1) break;				
			}
		}
	}

	rc = pthread_mutex_unlock(&a_mutex);
    //쓰레드 없애기(리턴시 자동종료됨?)
    pthread_exit(NULL);

	menu_chapter_view (idxPage, 0);
}

void word_page_auto_start (int chapter)
{
	//쓰레드 관련 변수
	int			tid;	//thread ID
    pthread_t	thr;	//thread's structure

	//쓰레드로 실행
	tid = pthread_create (&thr, NULL, word_page_auto_run, (void*)&chapter);
	//word_page_auto_run ((void*)&chapter);

}

//단어를 보고 말하기 루프
//-----------------------------------------------------------------------------
int snd_speak_play (int idx)
{
	WORD1_PAGE*	word_page;

	if (word_page_init (idx) < 0) return -1;

	//큐의 제일 앞부분 구조체 가져옴
	while (dequeue (queueWordPage, (void**)&word_page)) 
	{	
		if (prsBook->status == BP_STOP) return 0; //메인메뉴 터치

		if (word_page_speak (word_page) == -1) return -1;
		iCnt1++;		
	}
	return 1;
}

//단어를 보고 말하기
//-----------------------------------------------------------------------------
void snd_speaking_run (void* data)
{
	int i, rc, result;
	int chapter = *((int*)data);

	rc = pthread_mutex_lock(&a_mutex);

	result = 1;
	if (chapter > 0) { //지정된 것만 
		result = snd_speak_play (chapter);

	} else {
		while (result > 0) //전체 반복
		{
			for (i = 0; i < listCount (listChapter); i++) {
				menu_chapter_view (i/(dcScreen->height/ROW-5)+1, i+1);
				sleep (1);
				result = snd_speak_play (i+1);
				if (result < 1) break;				
			}
		}
	}

	rc = pthread_mutex_unlock(&a_mutex);
    //쓰레드 없애기(리턴시 자동종료됨?)
    pthread_exit(NULL);

	menu_chapter_view (idxPage, 0);    
}

void snd_speaking_start (int chapter)
{
	//쓰레드 관련 변수
	int			tid;	//thread ID
    pthread_t	thr;	//thread's structure

	tid = pthread_create (&thr, NULL, snd_speaking_run, (void*)&chapter);

}

//터치시 학습용 페이지 화면에서 정답확인
//입력: 터치좌표
//출력: 1=정답, 0=오답
//-----------------------------------------------------------------------------
int ev_word_page_answer (int tx, int ty)
{
	WORD1_PAGE*	word_page;
	int			ans, chapter;
	int			x1, y1, x2, y2;	//반환되는 그림좌표

	//메뉴가 터치 되었는지 판단
	ans = dp_touch_menu (tx, ty, prsMenu);
	if (ans == 1) {	//메인메뉴
		prsMenu->status = MN_CHAPTER;
		prsBook->status = BP_STOP;
		menu_chapter_view (idxPage, 0);
		return 0;

	} else if (ans == 2) {	//자동 플레이
		if (prsBook->status == BP_RUNNING) {
			prsBook->status = BP_STOP;
			//쓰레드가 종료되었는지 확인필요
			sleep(2);
		}
		if (prsMenu->status == MN_CHAPTER) word_page_auto_start (0);
		else word_page_auto_start (idxChapter);

		prsBook->status = BP_RUNNING;
		prsMenu->status = MN_PREVIEW;
		return 0;

	} else if (ans == 3) {	//발음하기
		if (prsBook->status == BP_RUNNING) {
			prsBook->status = BP_STOP;
			//쓰레드가 종료되었는지 확인필요
			sleep(2);
		}
		if (prsMenu->status == MN_CHAPTER) snd_speaking_start (0);
		else snd_speaking_start (idxChapter);

		prsBook->status = BP_RUNNING;
		prsMenu->status = MN_RECORD;
		return 0;
	}

	//학습목록 터치
	if (prsMenu->status == MN_CHAPTER) {
		idxChapter = dp_touch_menu_chapter (tx, ty, 1, idxPage, dcScreen->height/ROW-5, ROW);
		if (idxChapter > 0) {
			menu_chapter_view (idxPage, idxChapter);
			prsMenu->status = MN_TOUCH;

			if (word_page_init (idxChapter) < 0) {
				word_page_left ("오류:정보파일없음");
				menu_chapter_view (idxPage, 0);
				return -1;
			}

			//큐의 제일 앞부분 구조체 가져옴
			if (queueFront (queueWordPage, (void**)&word_page)) {
				word_page_display (word_page);
			} else {
				return -1;
			}			
		}
		return 0;

	//학습사진 터치
	} else {
		//큐의 제일 앞부분 구조체 가져옴
		if (queueFront (queueWordPage, (void**)&word_page) == 0) return -1;

		//그림(사진)이 터치 되었는지 판단
		ans = dp_touch_position (word_page->ans, tx, ty, boxBookPic, &x1, &y1, &x2, &y2);
		if (ans == 1) {
			//정답 테두리 표시
			//gx_circle (dcScreen, tx, ty, 20);
			dcScreen->pen_color = CL_BLUE;
			gx_box (dcScreen, x1,   y1,   x2,   y2);
			gx_box (dcScreen, x1+1, y1+1, x2-1, y2-1);
			gx_box (dcScreen, x1+2, y1+2, x2-2, y2-2);
			dequeue (queueWordPage, (void**)&word_page); //큐에서 삭제
			//sleep (1);
			//정답수 증가
			iCnt1++;
			//큐의 제일 앞부분 구조체 가져옴
			if (queueFront (queueWordPage, (void**)&word_page)) {
				word_page_display (word_page);
			} else {
				word_page_left ("The End.");
				sleep (1);
				prsMenu->status = MN_CHAPTER;
				menu_chapter_view (idxPage, 0);
				return -1;
			}			

		} else if (ans == 0) {
			//오답 테두리 표시
			dcScreen->pen_color = CL_YELLOW;
			gx_box (dcScreen, x1, y1, x2, y2);

			enqueue (queueWordPage, word_page); //큐의 제일뒤에 삽입
			//오답수 증가
			iCnt0++;
			word_page_left ("오답:다시하세요");
		}
		return 1;
	}	
}

//이벤트 처리함수
//-----------------------------------------------------------------------------
int event_handler (ie_event_t *event_data)
{   
	int tx, ty;

	tx = event_data->data.mouse.x;
	ty = event_data->data.mouse.y;

    switch (event_data->type)
    {
		case IE_MOUSE_DOWN:
				printf ("IE_MOUSE_DOWN: x=%d,y=%d\n", tx, ty); TRACE;
				ev_word_page_answer (tx, ty);
				break;

		case IE_MOUSE_UP:
				//printf ("IE_MOUSE_UP: x=%d,y=%d\n", tx, ty); TRACE;
				break;

		case IE_MOUSE_MOVE:
				//printf ("IE_MOUSE_MOVE: x=%d,y=%d\n", tx, ty); TRACE;
				//라인표시
				//gx_line_to(dcScreen, tx, ty);
				break;

		default:
				printf ("default\n"); TRACE;	
				break;
    }

    return 0;
} 

// 입출력 라이브러리 초기화
//-----------------------------------------------------------------------------
int devio_setting (void)
{
	//gxLib 초기화
    if (gx_init ("/dev/fb"))
    {
		gx_print_error("gx_init(): error!\n");
        return -1;

    } else {
		if ((dcScreen = gx_get_screen_dc()))
		{
			printf ("gx_set_font (gulim.bdf)\n"); TRACE;

			if  (gx_set_font ("gulim.bdf"))	//폰트 설정
                gx_print_error("gulim.bdf");
		} else {
			gx_print_error("gx_get_screen_dc(): fail!\n");
			return -1;
		}
    }

	//exLib를 초기화다.
    if (ex_init (EX_WAIT_TIME_DEFAULT))
    {
		ex_print_error("ex_init: error!\n");
        return -1;
    }

	//ixLib 초기화
	//매개변수:0=터치스크린 보정없는 좌표, 1=터치스크린 보정좌표
    if (ix_init(1))
    {
		ix_print_error("ix_init(): error!");
		return -1;
    }

	//입력 드라이버를 자동검색 및 open처리
    if (ix_auto_scan_open (IX_TMP_PATH_DEFAULT))               
    {
		ix_print_error("ix_auto_scan_open(): error!");
		return -1;
    }

	//이벤트 처리함수 등록(ix.c의 ix_event_func 포인터 할당)
    ix_add_event (event_handler);

    return 0;
}

//메인
//-----------------------------------------------------------------------------
int main (void)
{	
	//입출력 라이브러리 설정
	if (devio_setting () < 0) return -1;
	//printf ("devio_setting():dcScreen %d,%d\n", dcScreen->width, dcScreen->height);

	//화면좌표 설정
	screen_coordinate ();
	
	//파일에서 학습목록 읽기
	listChapter = wp_chapter_create ("eng_index.dat");
	if (!listChapter) return -1;
	
	idxPage = 1;
	idxChapter = 1;

	//학습페이지 메뉴
	menu_chapter_view (idxPage, 0);

	//전체반복(매개변수 0)
	//word_page_auto_start (0);

    while (1)
    {
		if (ex_loop ())
		{
			ex_print_error ("ex_loop(): error!\n");
			return -1;
		}
    }

	//할당해제
	if (queueWordPage) destroyQueue (queueWordPage);
	if (listChapter) destroyList (listChapter);
	
	gx_release_dc (dcScreen);
    gx_close ();
    return 0;
}

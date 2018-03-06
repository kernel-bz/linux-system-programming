//
// 파일명		: /include/ebook/word_page.h
// 기능			: /전자책/단어학습 페이지
// 아키텍쳐	: MIPS
// 작성자		: 정재준(http://www.kernel.bz/),(rgbi3307@nate.com)
// 최초코딩	: 2009-05-14
// 최근수정	: 2009-05-19
// 

#ifndef __WORD_PAGE_H
#define __WORD_PAGE_H

#define PATH_DATA "/mnt/usb2/ebook/data/"	//데이터경로

//학습목록 구조체
typedef struct
{
	int	 idx;		
	int	 space;			//왼쪽여백
	char title[60];		//학습제목
	char path [40];		//경로
	char file [20];		//데이터파일명
} CHAPTER;


//페이지정보 구조체
typedef struct
{
	int  idx;
	char query[80];	//질문
	char path [40];	//그림경로(절대경로)
	char pic1 [20];	//그림파일명
	char pic2 [20];
	char pic3 [20];
	char pic4 [20];
	int	 ans;		//정답번호
	int  timeout;	//제한시간(초)
} WORD1_PAGE;

QUEUE* wp_page_create (char* fname);
void wp_page_print (QUEUE* pQueue);


#endif

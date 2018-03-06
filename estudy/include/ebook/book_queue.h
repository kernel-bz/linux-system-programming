//
// 파일명		: /include/ebook/book_queue.h
// 기능			: /전자책/책페이지 큐
// 아키텍쳐	: MIPS
// 작성자		: 정재준(http://www.kernel.bz/),(rgbi3307@nate.com)
// 최초코딩	: 2009-05-13
// 최근수정	: 2009-05-13
// 

#ifndef __BOOK_QUEUE_H
#define __BOOK_QUEUE_H

#define PATH_BOOK_PAGE "/mnt/usb2/07Project/data/bgd/"

//QUEUE* qbook_page;

typedef struct
{
	char path[128];
	char file[80];
} BOOK_PAGE;

QUEUE* book_page_create (void);
void book_page_print (QUEUE* pQueue);

#endif

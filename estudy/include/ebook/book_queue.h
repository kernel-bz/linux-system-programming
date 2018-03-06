//
// ���ϸ�		: /include/ebook/book_queue.h
// ���			: /����å/å������ ť
// ��Ű����	: MIPS
// �ۼ���		: ������(http://www.kernel.bz/),(rgbi3307@nate.com)
// �����ڵ�	: 2009-05-13
// �ֱټ���	: 2009-05-13
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

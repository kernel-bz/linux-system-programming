//
// 파일명		: /ebook/book_queue.c
// 기능			: /전자책/책페이지 큐
// 아키텍쳐	: MIPS
// 작성자		: 정재준(http://www.kernel.bz/),(rgbi3307@nate.com)
// 최초코딩	: 2009-05-13
// 최근수정	: 2009-05-13
// 

#include <stdio.h>
#include "queue.h"
#include "book_queue.h"


QUEUE* book_page_create (void)
{
	QUEUE* qbook_page;
	BOOK_PAGE* pPage;
	int i;
	char file[10][20] = {"bk_letter.jpg", "bk_rain.jpg", "bk_sea.jpg"
					   , "bk_singer.jpg", "bk_dream.jpg", "bk_spring.jpg", "bk_memo.jpg", "bk_dancer.jpg"};

	qbook_page = createQueue ();

	for (i = 0; i < 8; i++) {
		pPage = (BOOK_PAGE*) malloc(sizeof(BOOK_PAGE));
		if (!(pPage)) {
			printf("book_page_create(): \aOut of Memory\n");
			return NULL;
		}
		sprintf (pPage->path, "%s", PATH_BOOK_PAGE);
		sprintf (pPage->file, "%s", file[i]);
		enqueue (qbook_page, pPage);
	}

	return qbook_page;
}

void book_page_print (QUEUE* pQueue)
{
	BOOK_PAGE* pPage;

	while (!emptyQueue (pQueue))
	{
		dequeue (pQueue, (void**)&pPage);
		printf("%s%s\n", pPage->path, pPage->file);
	}
	printf("\n");

	return;
}


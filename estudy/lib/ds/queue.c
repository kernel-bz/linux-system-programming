//
// 파일명		: /lib/ds/queue.c
// 기능			: 데이터처리 큐
// 아키텍쳐	: MIPS
// 작성자		: 정재준(http://www.kernel.bz/),(rgbi3307@nate.com)
// 최초코딩	: 2009-05-13
// 최근수정	: 2009-05-13
// 

#include <stdlib.h>
#include "queue.h"


QUEUE* createQueue (void)
{
	QUEUE* queue;

	queue = (QUEUE*) malloc(sizeof(QUEUE));
	if (queue) {
		queue->front = NULL;
		queue->rear  = NULL;
		queue->count = 0;
	}
	return queue;
}

bool enqueue (QUEUE* queue, void* itemPtr)
{
	QUEUE_NODE* newPtr;

	if (!(newPtr = (QUEUE_NODE*) malloc (sizeof(QUEUE_NODE)) ) ) 
		return false;

	newPtr->dataPtr = itemPtr;
	newPtr->next = NULL;

	if (queue->count == 0) queue->front = newPtr;
	else queue->rear ->next = newPtr;

	(queue->count )++;
	queue->rear = newPtr;
	return true;
}

bool dequeue (QUEUE* queue, void** itemPtr)
{
	QUEUE_NODE* deleteLoc;

	if (!queue->count) return false;

	*itemPtr = queue->front ->dataPtr ;
	deleteLoc = queue->front ;
	if (queue->count == 1) queue->rear = queue->front = NULL;
	else queue->front = queue->front->next ;

	(queue->count)--;
	free (deleteLoc);

	return true;
}

bool queueFront (QUEUE* queue, void** itemPtr)
{
	if (!queue->count) return false;
	else {
		*itemPtr = queue->front ->dataPtr ;
		return true;
	}
}

bool queueRear (QUEUE* queue, void** itemPtr)
{
	if (!queue->count) return false;
	else {
		*itemPtr = queue->rear ->dataPtr ;
		return true;
	}
}

bool emptyQueue (QUEUE* queue)
{
	return (queue->count == 0);
}

bool fullQueue (QUEUE* queue)
{
	QUEUE_NODE* temp;

	temp = (QUEUE_NODE*) malloc (sizeof(*(queue->rear )));
	if (temp) {
		free (temp);
		return false;
	}
	return true;
}

int queueCount (QUEUE* queue)
{
	return queue->count ;
}

QUEUE* destroyQueue (QUEUE* queue)
{
	QUEUE_NODE* deletePtr;

	if (queue) {
		while (queue->front != NULL) {
			free (queue->front->dataPtr );
			deletePtr = queue->front ;
			queue->front = queue->front ->next ;
			free (deletePtr);
		} //while
		free (queue);
	} //if
	return NULL;
}

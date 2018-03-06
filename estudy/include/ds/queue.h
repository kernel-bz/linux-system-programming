//
// ���ϸ�		: /include/ds/queue.h
// ���			: ������ó�� ť
// ��Ű����	: MIPS
// �ۼ���		: ������(http://www.kernel.bz/),(rgbi3307@nate.com)
// �����ڵ�	: 2009-05-13
// �ֱټ���	: 2009-05-13
// 

#ifndef __QUEUE_H
#define __QUEUE_H

#define bool	int
#define true	1
#define false	0


typedef struct node_queue
{
	void* dataPtr;
	struct node_queue* next;
} QUEUE_NODE;

typedef struct
{
	QUEUE_NODE* front;
	QUEUE_NODE* rear;
	int count;
} QUEUE;

QUEUE* createQueue (void);
QUEUE* destroyQueue (QUEUE*	queue);

bool dequeue	(QUEUE* queue, void** itemPtr);
bool enqueue	(QUEUE* queue, void*  itemPtr);
bool queueFront	(QUEUE* queue, void** itemPtr);
bool queueRear  (QUEUE* queue, void** itemPtr);
int  queueCount (QUEUE* queue);

bool emptyQueue (QUEUE* queue);
bool fullQueue  (QUEUE* queue);

#endif

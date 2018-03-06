//
// 파일명		: /include/ds/stack.h
// 기능			: 데이터처리 스택
// 아키텍쳐	: MIPS
// 작성자		: 정재준(http://www.kernel.bz/),(rgbi3307@nate.com)
// 최초코딩	: 2009-05-13
// 최근수정	: 2009-05-13
// 

#ifndef __STACK_H
#define __STACK_H

#define bool	int
#define true	1
#define false	0

typedef struct node_stack
{
	void* dataPtr;
	struct node_stack* link;
} STACK_NODE;


typedef struct
{
	int count;
	STACK_NODE* top;
} STACK;


STACK* createStack (void);
STACK* destroyStack (STACK* stack);

bool pushStack (STACK* stack, void* dataInPtr);
void* popStack (STACK* stack);
void* stackTop (STACK* stack);
bool emptyStack (STACK* stack);
bool fullStack (STACK* stack);
int stackCount (STACK* stack);

#endif

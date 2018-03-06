//
// 파일명		: /lib/ds/stack.c
// 기능			: 데이터처리 스택
// 아키텍쳐	: MIPS
// 작성자		: 정재준(http://www.kernel.bz/),(rgbi3307@nate.com)
// 최초코딩	: 2009-05-13
// 최근수정	: 2009-05-13
// 

#include <stdlib.h>
#include "stack.h"


STACK* createStack (void)
{
	STACK* stack;

	stack = (STACK*)malloc (sizeof(STACK));
	if (stack)
	{
		stack->count = 0;
		stack->top = NULL;
	}

	return stack;
}

bool pushStack (STACK* stack, void* dataInPtr)
{
	STACK_NODE* newPtr;

	newPtr = (STACK_NODE*)malloc (sizeof(STACK_NODE));
	if (!newPtr) return false;

	newPtr->dataPtr = dataInPtr;
	newPtr->link = stack->top;
	stack->top = newPtr;
	(stack->count)++;

	return true;
}

void* popStack (STACK* stack)
{
	void* dataOutPtr;

	STACK_NODE* temp;

	if (stack->count == 0) dataOutPtr = NULL;
	else {
		temp = stack->top;
		dataOutPtr = stack->top->dataPtr;
		stack->top = stack->top->link;
		free (temp);
		(stack->count)--;
	}

	return dataOutPtr;
}

void* stackTop (STACK* stack)
{
	if (stack->count == 0) return NULL;
	else return stack->top->dataPtr;
}

bool emptyStack (STACK* stack)
{
	return (stack->count == 0);
}

bool fullStack (STACK* stack)
{
	STACK_NODE* temp;

	if ((temp = (STACK_NODE*)malloc (sizeof(*(stack->top))) ) ) //malloc succeed
	{
		free(temp);
		return false;
	}
	//malloc failed
	return true;
}

int stackCount (STACK* stack)
{
	return stack->count;
}

STACK* destroyStack (STACK* stack)
{
	STACK_NODE* temp;

	if (stack)
	{
		while (stack->top != NULL)
		{
			free (stack->top->dataPtr);

			temp = stack->top;
			stack->top = stack->top->link;
			free (temp);
		}
		free (stack);
	}
	return NULL;
}

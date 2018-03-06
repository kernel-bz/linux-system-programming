//
// 파일명		: /lib/ds/list.c
// 기능			: 데이터처리 리스트
// 아키텍쳐	: MIPS
// 작성자		: 정재준(rgbi3307@nate.com)
// 저작권		: (c) 커널연구회(http://www.kernel.bz/) 정재준
// 최초코딩	: 2009-05-21
// 최근수정	: 2009-05-21
// 

#include <stdlib.h>
#include "list.h"


LIST* createList (int (*compare) (void* argu1, void* argu2) )
{
	LIST* list;

	list = (LIST*) malloc (sizeof (LIST));
	if (list) {
		list->head	= NULL;
		list->pos	= NULL;
		list->rear	= NULL;
		list->count	= 0;
		list->compare	= compare;
	}
	return list;
}

int addNode (LIST* pList, void* dataInPtr)
{
	bool found;
	bool success;

	NODE* pPre;
	NODE* pLoc;

	found = _search (pList, &pPre, &pLoc, dataInPtr);
	if (found) return (+1);

	success = _insert (pList, pPre, dataInPtr);
	if (!success) return (-1); //Overflow
	
	return (0);
}

bool _insert (LIST* pList, NODE* pPre, void* dataInPtr)
{
	NODE* pNew;

	if (!(pNew = (NODE*) malloc(sizeof(NODE)) )) return false;

	pNew->dataPtr = dataInPtr;
	pNew->link = NULL;

	if (pPre == NULL) {
		pNew->link = pList->head;
		pList->head = pNew;
		if (pList->count == 0) pList->rear = pNew;
	} else {
		pNew->link = pPre->link;
		pPre->link = pNew;

		if (pNew->link == NULL) pList->rear = pNew;
	}
	(pList->count)++;
	return true;
}

bool removeNode (LIST* pList, void* keyPtr, void** dataOutPtr)
{
	bool found;
	NODE* pPre;
	NODE* pLoc;

	found = _search (pList, &pPre, &pLoc, keyPtr);
	if (found) _delete (pList, pPre, pLoc, dataOutPtr);

	return found;
}

void _delete (LIST* pList, NODE* pPre, NODE* pLoc, void** dataOutPtr)
{
	*dataOutPtr = pLoc->dataPtr;
	if (pPre == NULL)
		pList->head = pLoc->link;
	else
		pPre->link = pLoc->link;

	if (pLoc->link == NULL) pList->rear = pPre;
	
	(pList->count)--;
	free (pLoc);

	return;
}

bool searchList (LIST* pList, void* pArgu, void** pDataOut)
{
	bool found;
	NODE* pPre;
	NODE* pLoc;

	found = _search (pList, &pPre, &pLoc, pArgu);
	if (found) 
		*pDataOut = pLoc->dataPtr;
	else 
		*pDataOut = NULL;

	return found;
}

bool _search (LIST* pList, NODE** pPre, NODE** pLoc, void* pArgu)
{
	#define COMPARE ( ((* pList->compare) (pArgu, (*pLoc)->dataPtr)) )
	#define COMPARE_LAST ((* pList->compare) (pArgu, pList->rear->dataPtr))

	int result;

	*pPre = NULL;
	*pLoc = pList->head;
	if (pList->count == 0) return false;

	if ( COMPARE_LAST > 0) {
		*pPre = pList->rear;
		*pLoc = NULL;
		return false;
	}

	while ( (result = COMPARE) > 0 )
	{
		*pPre = *pLoc;
		*pLoc = (*pLoc)->link;
	}

	if (result == 0) return true;
	else return false;
}

static bool retrieveNode (LIST* pList, void* pArgu, void** dataOutPtr)
{
	bool found;
	NODE* pPre;
	NODE* pLoc;

	found = _search (pList, &pPre, &pLoc, pArgu);
	if (found) {
		*dataOutPtr = pLoc->dataPtr;
		return true;
	}
	*dataOutPtr = NULL;
	return false;
}

bool emptyList (LIST* pList)
{
	return (pList->count == 0);
}

bool fullList (LIST* pList)
{
	NODE* temp;

	if ((temp = (NODE*)malloc(sizeof(*(pList->head))))) {
		free (temp);
		return false;
	}
	return true;
}

int listCount (LIST* pList)
{
	return pList->count;
}

bool traverse (LIST* pList, int fromWhere, void** dataPtrOut)
{
	if (pList->count == 0) return false;

	if (fromWhere == 0) {
		pList->pos = pList->head;
		*dataPtrOut = pList->pos->dataPtr;
		return true;
	} else {
		if (pList->pos->link == NULL)
			return false;
		else {
			pList->pos = pList->pos->link;
			*dataPtrOut = pList->pos->dataPtr;
			return true;
		}
	}
}

LIST* destroyList (LIST* pList)
{
	NODE* deletePtr;

	if (pList) {
		while (pList->count > 0) {
			free (pList->head->dataPtr);

			deletePtr = pList->head;
			pList->head = pList->head->link;
			pList->count--;
			free (deletePtr);
		}
		free (pList);
	}
	return NULL;
}

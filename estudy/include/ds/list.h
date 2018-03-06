//
// ���ϸ�		: /include/ds/list.h
// ���			: ������ó�� ����Ʈ
// ��Ű����	: MIPS
// �ۼ���		: ������(rgbi3307@nate.com)
// ���۱�		: (c) Ŀ�ο���ȸ(http://www.kernel.bz/) ������
// �����ڵ�	: 2009-05-21
// �ֱټ���	: 2009-05-21
// 
#ifndef __LIST_H
#define __LIST_H

#define bool	int
#define true	1
#define false	0

typedef struct node_list
{
	void* dataPtr;
	struct node_list* link;
} NODE;

typedef struct
{
	int count;
	NODE* pos;
	NODE* head;
	NODE* rear;
	int (*compare) (void* argu1, void* argu2);
} LIST;

LIST* createList (int (*compare) (void* argu1, void* argu2) );
LIST* destroyList (LIST* list);
int addNode (LIST* pList, void* dataInPtr);
bool removeNode (LIST* pList, void* keyPtr, void** dataOutPtr);
bool searchList (LIST* pList, void* pArgu, void** pDataOut);
bool retrieveNode (LIST* pList, void* pArgu, void** dataOutPtr);
bool traverse (LIST* pList, int fromWhere, void** dataOutPtr);

int listCount (LIST* pList);
bool emptyList (LIST* pList);
bool fullList (LIST* pList);

bool _insert (LIST* pList, NODE* pPre, void* dataInPtr);
void _delete (LIST* pList, NODE* pPre, NODE* pLoc, void** dataOutPtr);
bool _search (LIST* pList, NODE** pPre, NODE** pLoc, void* pArgu);

#endif

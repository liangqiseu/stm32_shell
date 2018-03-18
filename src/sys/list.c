/************************************************************************/
/* name:list.c                                                          */
/* create data:2017/11/24                                               */
/************************************************************************/

#include<stdio.h>
#include<stdlib.h>

#include "list_api.h"
#include "config.h"


void OS_ListInit()
{
	return;
}

/*
*/
LIST_HEAD* OS_ListCreate(void)
{
	LIST_HEAD* pList;
	pList = (LIST_HEAD*)malloc(sizeof(LIST_HEAD));
	pList->tail = NULL;
	pList->head = NULL;
	pList->listLen = 0;
	return pList;
}

u32 OS_ListEmpty(LIST_HEAD* v_pListHead)
{
	if (0 == v_pListHead->listLen)
	{
		return TRUE;
	}
	return FALSE;
}


/*
*/
void OS_ListAddNodeToTail(LIST_HEAD* v_pListHead,LIST_NODE_S* v_pNode)
{
	v_pNode->pNextNode = (struct LIST_NODE_S*)v_pNode;
	v_pNode->pPreNode = (struct LIST_NODE_S*)v_pListHead->tail;

	if (TRUE == OS_ListEmpty(v_pListHead))
	{
		v_pListHead->head = v_pNode;
		v_pListHead->tail = v_pNode;
	}
	else
	{
		v_pListHead->tail->pNextNode = (struct LIST_NODE_S*)v_pNode;
		v_pListHead->tail = v_pNode;
	}
	
	v_pListHead->listLen++;
	return;
}


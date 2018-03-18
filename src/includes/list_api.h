/************************************************************************/
/* name:list_api.h                                                      */
/* create data:2017/8/15                                                */
/************************************************************************/
#ifndef _LIST_API_H_
#define _LIST_API_H_

#include "config.h"

typedef struct tagLIST_NODE_S
{
    struct LIST_NODE_S *pPreNode;
    struct LIST_NODE_S *pNextNode;
    void *data;
}LIST_NODE_S;

typedef struct tagLIST_INFO_S
{
    u32 listId;
    u32 listLen;
    LIST_NODE_S *head;
    LIST_NODE_S *tail;
}LIST_INFO_S;

typedef LIST_INFO_S LIST_HEAD;

LIST_HEAD* OS_ListCreate(void);
void OS_ListAddNodeToTail(LIST_HEAD* v_pListHead, LIST_NODE_S* v_pNode);
u32 OS_ListEmpty(LIST_HEAD* v_pListHead);
#endif  

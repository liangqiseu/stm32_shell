/************************************************************************/
/* name:os_core.c                                                       */
/* create data:2017/8/15                                                */
/************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#include "os_api.h"
#include "config.h"
#include "list_api.h"

OS_SCHEDULE_CTRL_S g_osScheduleCtrl;

static u32 g_osTaskIdIndex = 0;

u32 g_osIdleTaskId = 0;
u32 g_osMsgHandleTaskId = 0;

LIST_HEAD* g_pOsMsgSendList = NULL;

u32 g_osTaskReadyGroup = 0;

u32 g_osTaskBitmap[8] = { 0 };

u32 g_osTaskReadMap[] = { 0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};

void OS_Schedule(void)
{
	u8 index = 0;
	u8 i = 0;
	while (i < 10000)
	{
		for (index = 0; index < OS_TASK_MAX_NUM; index++)
		{
			if (OS_TASK_ACTIVE == g_osScheduleCtrl.taskinfo[index].enableBitmap)
			{
				g_osScheduleCtrl.taskinfo[index].taskFunction();
				g_osScheduleCtrl.taskinfo[index].runtimes++;
			}
		}
		i++;
	}
	return;
}

OS_TASK_RETURN_E OS_IdleTask(void)
{
	return 	OS_TASK_DO_SOMETHING;
}

OS_TASK_RETURN_E OS_MsgHandleTask(void)
{
	LIST_NODE_S *lastNode;
	OS_MSG_RETRY_S *msg;
	if (TRUE == OS_ListEmpty(g_pOsMsgSendList))
	{
		return OS_TASK_DO_NOTHING;
	}
	else
	{
		lastNode = g_pOsMsgSendList->tail;
		msg = (OS_MSG_RETRY_S*)lastNode->data;
		msg->fun(msg->pArgs);
	}
	return OS_TASK_DO_NOTHING;
}


/*
*/
u32 OS_CreateTask(OS_TASK_FUNCTION v_fun, char* v_name)
{
	g_osScheduleCtrl.taskinfo[g_osTaskIdIndex].enableBitmap = OS_TASK_ACTIVE;
	g_osScheduleCtrl.taskinfo[g_osTaskIdIndex].name = v_name;
	g_osScheduleCtrl.taskinfo[g_osTaskIdIndex].taskFunction = v_fun;
	g_osScheduleCtrl.taskinfo[g_osTaskIdIndex].weight = 1;  //默认权重是1
	g_osScheduleCtrl.taskinfo[g_osTaskIdIndex].regFlag = TRUE;

	/*配置全局调度结构的bitmap*/
	/*
	g_osTaskReadyGroup |= g_osTaskReadMap[v_taskId >> 3];
	g_osTaskBitmap[v_taskId >> 3] |= g_osTaskReadMap[v_taskId & 0x7];
	printf("g_osTaskReadyGroup=0x%x\r\n", g_osTaskReadyGroup);
	printf("Taskbitmap[%d]=0x%x\r\n", v_taskId >> 3, g_osTaskBitmap[v_taskId >> 3]);
	*/
	g_osTaskIdIndex++;
	return (g_osTaskIdIndex-1);
}


/*
*/
void OS_Init(void)
{	
	(void)memset(&g_osScheduleCtrl, 0, sizeof(g_osScheduleCtrl));
	g_osIdleTaskId = OS_CreateTask(OS_IdleTask, "OS_IdleTask");
	g_osMsgHandleTaskId = OS_CreateTask(OS_MsgHandleTask, "OS_IdleTask");

	g_pOsMsgSendList = OS_ListCreate();
	return;
}

/*
*/
void OS_ModifyTaskWeight(u32 v_taskId,u32 v_value)
{
	if (TRUE == g_osScheduleCtrl.taskinfo[v_taskId].regFlag)
	{
		g_osScheduleCtrl.taskinfo[v_taskId].weight = v_value;
	}
	return;
}

/*
*/
OS_MSG_RETRY_E OS_MsgSend(OS_MSG_FUNCTION v_fun, void* v_pArgs)
{
	OS_MSG_RETRY_S* msgMem;
	LIST_NODE_S* nodeMem;
	if (OS_MSG_NEED_RETRY == v_fun(v_pArgs))
	{
		msgMem = (OS_MSG_RETRY_S*)malloc(sizeof(OS_MSG_RETRY_S));
		msgMem->fun = v_fun;
		msgMem->pArgs = v_pArgs;

		nodeMem = (LIST_NODE_S*)malloc(sizeof(LIST_NODE_S));
		nodeMem->data = (void*)msgMem;
		OS_ListAddNodeToTail(g_pOsMsgSendList, nodeMem);
		return OS_MSG_NEED_RETRY;
	}
	return OS_MSG_NEED_NO_RETRY;
}

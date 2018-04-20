/* std lib*/
#include <string.h>

#include "print.h"
#include "config.h"
#include "shell.h"
#include "usart.h"
#include "symbol_api.h"
#include "os_api.h"

#define SHELL_CMD_CHAR_MAX 50
#define SHELL_PROMPT "XiaoOS# "

extern SYMBOL_TABAL_S symTbl[];
extern unsigned g_symTblLen;

void SHELL_ExecOneCmd(void *v_pCmdPara)
{
	return;
} 


/*
	find function or variable's addres by binary search
	return value:
		-1 : cmd not find
		>0 : cmd array's index
*/
s32 SHELL_GetAddrByName(const char *v_pName)
{
	u32 left = 0;
	u32 right = g_symTblLen - 1;
	u32 mid = 0;	
	s8 cmpResult = 0;

	while (left <= right)
	{
		mid = left + (right - left) / 2;
		cmpResult = strcmp(v_pName,symTbl[mid].name);
		if (0 > cmpResult)
		{
			right = mid - 1;
		}
		else if (0 < cmpResult)
		{
			left = mid + 1;
		}
		else
		{
			return mid;
		}		
	}

	return SHELL_CMD_NOT_FIND;
}



void SHELL_GetOneCmd(void)
{
	static u8 cnt = 0;
	static u8 revDoneFlag = FALSE;
	static char oneCmd[SHELL_CMD_CHAR_MAX] = { 0 };

	while (g_usartRevBufWrIdx != g_usartRevBufRdIdx)
	{
		if (0x0d == g_usartRevBuf[g_usartRevBufRdIdx])
		{
			if (0x0a == (g_usartRevBuf[(g_usartRevBufRdIdx+1)%USART_REV_LEN]))
			{
				g_usartRevBuf[g_usartRevBufRdIdx] = '\0';
				g_usartRevBuf[(g_usartRevBufRdIdx+1)%USART_REV_LEN] = '\0';
				g_usartRevBufRdIdx = (g_usartRevBufRdIdx+1) % USART_REV_LEN;
				g_usartRevBufRdIdx = (g_usartRevBufRdIdx+1) % USART_REV_LEN;
				oneCmd[cnt] = '\0';
				revDoneFlag = TRUE;
				break;
			}
			else
			{
				g_usartRevBuf[g_usartRevBufRdIdx] = '\0';
				g_usartRevBufRdIdx = (g_usartRevBufRdIdx + 1) % USART_REV_LEN;
				oneCmd[cnt] = '\0';
				revDoneFlag = TRUE;
				break;
			}
		}


		oneCmd[cnt] = g_usartRevBuf[g_usartRevBufRdIdx];

		if (cnt == (SHELL_CMD_CHAR_MAX-3))
		{
			oneCmd[cnt] = '\0';
			revDoneFlag = TRUE;
			break;
		}
		cnt++;
		g_usartRevBufRdIdx = (g_usartRevBufRdIdx + 1) % USART_REV_LEN;
	}
		
	if (TRUE == revDoneFlag)
	{
		if ('\0' == oneCmd[0])
		{
			USART_PrintfFunc("%s\r\n",oneCmd);
		}
		else
		{
			USART_PrintfFunc("\r\n%s\r\n",oneCmd);
		}
		USART_PrintfFunc(SHELL_PROMPT);
		cnt = 0;
		revDoneFlag = FALSE;
	}
	
	return;
}

/*
	OS_ShellTask: called by OS , build-in task, created by OS init.
*/
OS_TASK_RETURN_E OS_ShellTask(void)
{
	SHELL_GetOneCmd();
	return OS_TASK_DO_SOMETHING;
}


/* std lib*/
#include <string.h>

#include "print.h"
#include "config.h"
#include "shell.h"
#include "usart.h"
#include "symbol_api.h"
#include "os_api.h"

#define SHELL_CMD_CHAR_MAX 50
#define SHELL_PROMPT "XiaoOS#"

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



OS_TASK_RETURN_E OS_ShellTask(void)
{
	u32 cnt = 0;
	char oneCmd[SHELL_CMD_CHAR_MAX] = {0};
	if (g_usartRevBufWrIdx != g_usartRevBufRdIdx)
	{
		while (TRUE)
		{
			if ((g_usartRevBuf[g_usartRevBufRdIdx]==0x0d) && (g_usartRevBuf[(g_usartRevBufRdIdx+1)%USART_REV_LEN]==0x0a))
			{
				g_usartRevBufRdIdx = (g_usartRevBufRdIdx+1) % USART_REV_LEN;
				g_usartRevBufRdIdx = (g_usartRevBufRdIdx+1) % USART_REV_LEN;
				oneCmd[cnt] = 0x0d;
				oneCmd[cnt+1] = 0x0a;
				oneCmd[cnt+2] = '\0';
				break;
			}

			oneCmd[cnt] = g_usartRevBuf[g_usartRevBufRdIdx];
	
			if (cnt == (SHELL_CMD_CHAR_MAX-3))
			{
				oneCmd[cnt] = 0x0d;
				oneCmd[cnt+1] = 0x0a;
				oneCmd[cnt+2] = '\0';
				break;
			}
			cnt++;
			g_usartRevBufRdIdx = (g_usartRevBufRdIdx+1)%USART_REV_LEN;
		}

		USART_PrintfFunc("%s",oneCmd);
		USART_PrintfFunc(SHELL_PROMPT);
	
		return OS_TASK_DO_SOMETHING;
	}

}



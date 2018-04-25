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



u32 SHELL_cmdIsFunction(SYMBOL_TABAL_S *v_pSymbolInfo)
{
	if ((SYMBOL_GLOBAL | SYMBOL_TEXT) == v_pSymbolInfo->type)
	{
		return TRUE;
	}

	return FALSE;
}


u32 SHELL_cmdIsGlobalValue(SYMBOL_TABAL_S *v_pSymbolInfo)
{
	if ((SYMBOL_GLOBAL | SYMBOL_BSS) == v_pSymbolInfo->type)
	{
		return TRUE;
	}

	if ((SYMBOL_GLOBAL | SYMBOL_DATA) == v_pSymbolInfo->type)
	{
		return TRUE;
	}

	return FALSE;
}


/*
	retValue: 
		0 -- receive no cmd
		n -- receive one cmd with length n
*/
u8 SHELL_GetOneCmd(char *v_pCmd)
{
	u8 retLength = 0;
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
				cnt++;
				revDoneFlag = TRUE;
				break;
			}
			else
			{
				g_usartRevBuf[g_usartRevBufRdIdx] = '\0';
				g_usartRevBufRdIdx = (g_usartRevBufRdIdx + 1) % USART_REV_LEN;
				oneCmd[cnt] = '\0';
				cnt++;
				revDoneFlag = TRUE;
				break;
			}
		}


		oneCmd[cnt] = g_usartRevBuf[g_usartRevBufRdIdx];

		if (cnt == (SHELL_CMD_CHAR_MAX-3))
		{
			oneCmd[cnt] = '\0';
			cnt++;
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
			USART_PrintfFunc(SHELL_PROMPT);
		}
		else
		{
			//USART_PrintfFunc("\r\n%s\r\n",oneCmd);
			(void)strcpy(v_pCmd,oneCmd);
		}

		retLength = cnt - 1;
		cnt = 0;
		revDoneFlag = FALSE;
		
	}

	return retLength;
}


void SHELL_ExecOneCmd(char *v_pCmdName)
{
	s32 cmdIndex = 0;
	
	USART_PrintfFunc("%s\r\n",v_pCmdName);
	cmdIndex = SHELL_GetAddrByName(v_pCmdName);
	if (0 <= cmdIndex)
	{
		
		//USART_PrintfFunc("%d %d\r\n",cmdIndex,*(u32*)(symTbl[cmdIndex].addr));
		if (TRUE == SHELL_cmdIsGlobalValue(&symTbl[cmdIndex]))
		{
			USART_PrintfFunc("%s: addr=0x%x value=0x%x\r\n",
							v_pCmdName,
							(void*)symTbl[cmdIndex].addr,
							*(u32*)(symTbl[cmdIndex].addr));
		}
		else if (TRUE == SHELL_cmdIsFunction(&symTbl[cmdIndex]))
		{
			USART_PrintfFunc("%s: addr=0x%x \r\n",
							v_pCmdName,
							(void*)symTbl[cmdIndex].addr);
		}
		else
		{
			USART_PrintfFunc("%s\r\n",v_pCmdName);
		}
	
	}
	else
	{
		USART_PrintfFunc("%s is not a valid cmd!\r\n",v_pCmdName);
	}

	USART_PrintfFunc(SHELL_PROMPT); 

	return;
} 



/*
	SHELL_CmdHandle: called by shell task
*/
void SHELL_CmdHandle(void)
{
	char oneCmd[SHELL_CMD_CHAR_MAX] = { 0 };

	if (0 == SHELL_GetOneCmd(oneCmd))
	{
		return;
	}
	else
	{
		//USART_PrintfFunc("%s\r\n",oneCmd);
		SHELL_ExecOneCmd(oneCmd);
	}
	return;
}


/* std lib*/
#include <string.h>

#include "print.h"
#include "config.h"
#include "shell.h"
#include "shell_api.h"
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
	int cmpResult = 0;

	while (left <= right)
	{
		mid = left + (right - left) / 2;
		cmpResult = strcmp(v_pName,symTbl[mid].name);

//		USART_PrintfFunc("%d %d %d %d\r\n",mid,left,right,cmpResult);
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
		(void)strcpy(v_pCmd,oneCmd);

		retLength = cnt;
		cnt = 0;
		revDoneFlag = FALSE;
		
	}

	return retLength;
}


void SHELL_CmdParse(u32 cmdIdx, SHELL_CMD_S *v_cmdInfo)
{
	SYMBOL_TABAL_S *symbolInfo = &symTbl[cmdIdx];
	if (TRUE == SHELL_cmdIsGlobalValue(symbolInfo))
	{
		v_cmdInfo->cmdType = SHELL_CMD_GLOBAL_VALUE;
		v_cmdInfo->cmdAddr = symbolInfo->addr;

	}
	else if (TRUE == SHELL_cmdIsFunction(symbolInfo))
	{
		v_cmdInfo->cmdType = SHELL_CMD_FUNCTION;
		v_cmdInfo->cmdAddr = symbolInfo->addr;
	}
	else
	{
		USART_PrintfFunc("\r\ncmdIdx=0x%x %s is what?\r\n",cmdIdx,symbolInfo->name);
	}
}


void SHELL_CmdExec(char *v_pCmdName,SHELL_CMD_S *v_cmdInfo)
{
	s32 fnRet = 0; 
	SHELL_FUN fn = NULL;
	
	if (SHELL_CMD_GLOBAL_VALUE == v_cmdInfo->cmdType)
	{
		USART_PrintfFunc("\r\n%s: addr=0x%x value=0x%x\r\n",
						v_pCmdName,
						(void*)v_cmdInfo->cmdAddr,
						*(u32*)v_cmdInfo->cmdAddr);

	}

	if (SHELL_CMD_FUNCTION == v_cmdInfo->cmdType)
	{
		fn = (SHELL_FUN)v_cmdInfo->cmdAddr;
		USART_PrintfFunc("\r\n%s: addr=0x%x \r\n",
						v_pCmdName,
						(void*)v_cmdInfo->cmdAddr);

		fnRet = fn(0,0,0,0,0);
		USART_PrintfFunc("ret=0x%x=%d\r\n",fnRet,fnRet);

	}
	
	return;
}



void SHELL_ExecOneCmd(char *v_pCmdName, u8 v_cmdLen)
{
	s32 cmdIndex = 0;
	SHELL_CMD_S cmdInfo;
	

	//len==1: maybe an enter  
	if ((1 == v_cmdLen))
	{
		USART_PrintfFunc("%s\r\n",v_pCmdName);
		return;
	}

	
	//USART_PrintfFunc("%s\r\n",v_pCmdName);
	cmdIndex = SHELL_GetAddrByName(v_pCmdName);
	if (0 <= cmdIndex)
	{
		SHELL_CmdParse((u32)cmdIndex,&cmdInfo);
		SHELL_CmdExec(v_pCmdName,&cmdInfo);
	
	}
	else
	{
		USART_PrintfFunc("\r\n%s is not a valid cmd!\r\n",v_pCmdName);
	}



	return;
} 



/*
	SHELL_CmdHandle: called by shell task
*/
void SHELL_CmdHandle(void)
{
	u8 len = 0;
	char oneCmd[SHELL_CMD_CHAR_MAX] = { 0 };

	len = SHELL_GetOneCmd(oneCmd);
	if (0 == len)
	{
		return;
	}

	SHELL_ExecOneCmd(oneCmd,len);

	USART_PrintfFunc(SHELL_PROMPT); 
	return;
}


s32 SHELL_Test(void)
{
	USART_PrintfFunc("SHELL_Test OK!\r\n");
	return (-19);
}


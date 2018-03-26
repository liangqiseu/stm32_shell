#include "shell.h"
#include "usart.h"
#include "print.h"
#include "os_api.h"

#define SHELL_CMD_CHAR_MAX 50
#define SHELL_PROMPT "XiaoOS#"

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



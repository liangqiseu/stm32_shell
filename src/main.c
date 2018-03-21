#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "print.h"
#include "os_api.h"
#include "config.h"
#include "shell.h"

/*
OS_TASK_RETURN_E OS_PrintTask(void)
{
    USART_Printf("os test!\r\n");
    return OS_TASK_DO_SOMETHING;
}
*/

OS_TASK_RETURN_E OS_LedTask(void)
{
    LED0 = ~LED0;
    LED1 = ~LED1;
    SYS_DelayMs(1000);
    return OS_TASK_DO_SOMETHING;
}

int main(void)
{	
    int a = 0;	 
    u32 ledTaskId = 0;
    u32 printTaskId = 0;

    Stm32_Clock_Init(9); 	
    SYS_DelayInit(72);	     	
    LED_Init();		  	 	
    USART_Init(72,9600);
    OS_Init();


    printTaskId = OS_CreateTask(OS_ShellTask,"OS_ShellTask");
    ledTaskId = OS_CreateTask(OS_LedTask,"OS_LedTask");

    OS_Schedule();
    return OS_OK;
}


























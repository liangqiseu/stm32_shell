#include "sys.h"
#include "delay_api.h"	
#include "prph_api.h" 
#include "print_api.h"
#include "os_api.h"
#include "config_api.h"
#include "prph_api.h"


OS_TASK_RETURN_E OS_LedTask(void)
{
    LED0 = ~LED0;
    LED1 = ~LED1;
    SYS_DelayMs(3000);
    return OS_TASK_DO_SOMETHING;
}

int main(void)
{	
    u32 ledTaskId = 0;

    Stm32_Clock_Init(9); 	
    SYS_DelayInit(72);	     	
    LED_Init();		  	 	
    USART_Init(72,9600);
    OS_Init();

    ledTaskId = OS_CreateTask(OS_LedTask,"OS_LedTask");

    OS_Schedule();
    return OS_OK;
}


























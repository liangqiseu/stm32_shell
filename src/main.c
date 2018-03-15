#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "print.h"

int main(void)
{		 
    Stm32_Clock_Init(9); 	
    SYS_DelayInit(72);	     	
    LED_Init();		  	 	
    USART_Init(72,9600);
    while(1)
    {
        LED0=1;
        LED1=0;
        SYS_DelayMs(1000);
        USART_Printf("hello world! %s %d %c\r\n","ssssqqadsc",10,'o');
        LED0=0;
        LED1=1;
        SYS_DelayMs(1000);
    }
	
    return 0;	 
}


























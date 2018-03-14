#include <stdio.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 

char* str = "hello world\r\n";

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
	//	printf("hello world!\r\n");
        //USART_SingleCharSend('a');
        USART_StringSend(str);
        LED0=0;
        LED1=1;
        SYS_DelayMs(1000);
    }
	
    return 0;	 
}


























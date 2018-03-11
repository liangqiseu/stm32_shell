#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 

int main(void)
{		 
	Stm32_Clock_Init(9); 	
	delay_init(72);	     	
	LED_Init();		  	 	
//	uart_init(72,9600);
	while(1)
	{
		LED0=1;
		LED1=0;
		delay_ms(1000);
//		USART_SingleCharSend('a');
		LED0=0;
		LED1=1;
		delay_ms(1000);
	}	 
}


























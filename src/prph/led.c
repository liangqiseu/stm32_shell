#include "sys.h"   
#include "prph_api.h"



/*
 *  copy form miniSTM devolpment board code
 * */
void LED_Init(void)
{
    RCC->APB2ENR|=1<<2;    	   	 
    RCC->APB2ENR|=1<<5;    
    GPIOA->CRH&=0XFFFFFFF0; 
    GPIOA->CRH|=0X00000003;
    GPIOA->ODR|=1<<8;     
											  
    GPIOD->CRL&=0XFFFFF0FF;
    GPIOD->CRL|=0X00000300;
    GPIOD->ODR|=1<<2;    
    return;
}







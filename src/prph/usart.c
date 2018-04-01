/*   std lib     */

/*  stm32 lib*/
#include "usart.h"	  

void USART_SingleCharSend(const char v_ch)
{
        while (0 == (USART1->SR&0x40));
        USART1->DR = (u8)v_ch;
        return;
}


void USART_StringSend(char* v_pDataBuf)
{
	while('\0' != *v_pDataBuf)
	{
		USART_SingleCharSend(*v_pDataBuf);
		v_pDataBuf++;
	}

    return;	
}

//data receive buffer ,max len is 200
u8 g_usartRevBuf[USART_REV_LEN];    
u16 g_usartRevBufRdIdx = 0;
u16 g_usartRevBufWrIdx = 0;

/*
	a simple customized usart protocol:
	1) string received by stm32  must end with enter key whose assic code is 0x0d0a
	2) if receive done,  set USART_RX_STA's bit15 to 1
	3) others will be regarded as receive fail and data buffer will be cleared to zero
*/
u16 USART_RX_STA=0;       
  
void USART1_IRQHandler(void)
{
	u8 revChar;
    if(USART1->SR&(1<<5))
    {	 
        revChar=USART1->DR; 
		g_usartRevBuf[g_usartRevBufWrIdx] = revChar;
		g_usartRevBufWrIdx = (g_usartRevBufWrIdx+1) % USART_REV_LEN;
//		USART_SingleCharSend(revChar);
    }
	return;
} 


void USART_Init(u32 v_pclk2,u32 v_bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(v_pclk2*1000000)/(v_bound*16);
	mantissa=temp;				
	fraction=(temp-mantissa)*16; 
	mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;  
	RCC->APB2ENR|=1<<14;  
	GPIOA->CRH&=0XFFFFF00F;
	GPIOA->CRH|=0X000008B0;
		  
	RCC->APB2RSTR|=1<<14;  
	RCC->APB2RSTR&=~(1<<14);
	
 	USART1->BRR=mantissa; 
	USART1->CR1|=0X200C;  
	USART1->CR1|=1<<8;    
	USART1->CR1|=1<<5;  
	MY_NVIC_Init(3,3,USART1_IRQn,2);

}

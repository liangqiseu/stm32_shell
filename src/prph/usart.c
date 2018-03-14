/*   std lib     */
#include <string.h>

/*  stm32 lib*/
#include "usart.h"	  
 
#pragma import(__use_no_semihosting)             
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef¡¯ d in stdio.h. */ 
FILE __stdout;       


int _sys_exit(int x)
{
    x = x;
    return 0;
}



int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8)ch;      
	return ch;
}



void USART_SingleCharSend(const char v_ch)
{
        while (0 == (USART1->SR&0x40));
        USART1->DR = (u8)v_ch;
        return;
}


void USART_StringSend(char* v_pDataBuf)
{
    u32 index;
    u32 len = strlen(v_pDataBuf);
    
    for(index = 0; index < len; index++)
    {
        USART_SingleCharSend(v_pDataBuf[index]);
    }
    return;	
}


#if EN_USART1_RX   
u8 USART_RX_BUF[USART_REC_LEN];    


u16 USART_RX_STA=0;       
  
void USART1_IRQHandler(void)
{
    u8 res;	
    if(USART1->SR&(1<<5))
    {	 
        res=USART1->DR; 
        if((USART_RX_STA&0x8000)==0)
        {
            if(USART_RX_STA&0x4000)
            {
                if(res!=0x0a)USART_RX_STA=0;
                else USART_RX_STA|=0x8000;
            }
            else 
            {	
                if(res==0x0d)USART_RX_STA|=0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
                    USART_RX_STA++;
                    if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;
                }		 
            }
        }  		 									     
    }
} 
#endif										 


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
#if EN_USART1_RX		 
	USART1->CR1|=1<<8;    
	USART1->CR1|=1<<5;  
	MY_NVIC_Init(3,3,USART1_IRQn,2);
#endif

}

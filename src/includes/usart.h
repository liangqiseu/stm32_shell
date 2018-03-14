#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 

#define USART_REC_LEN  			200  	
#define EN_USART1_RX 			1		
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; 
extern u16 USART_RX_STA;         

void USART_Init(u32 v_pclk2,u32 v_bound);


void USART_SingleCharSend(const char v_ch);
void USART_StringSend(char* v_pDataBuf); 
#endif	   

















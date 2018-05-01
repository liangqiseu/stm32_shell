#ifndef _PRPH_API_H_
#define _PRPH_API_H_

#include "sys.h"
#include "config_api.h"

#define USART_REV_LEN  			200  	
#define EN_USART1_RX 			1		

#define LED0 PAout(8)	// PA8
#define LED1 PDout(2)	// PD2	
  	
extern u8  g_usartRevBuf[USART_REV_LEN];
extern u16 g_usartRevBufRdIdx;
extern u16 g_usartRevBufWrIdx;

void USART_Init(u32 v_pclk2,u32 v_bound);
void USART_SingleCharSend(const char v_ch);
void USART_StringSend(char* v_pDataBuf); 

void LED_Init(void);
#endif


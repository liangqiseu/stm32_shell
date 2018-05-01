#include "delay_api.h"
static u8  fac_us=0;  
static u16 fac_ms=0;

			   
void SYS_DelayInit(u8 v_sysclk)
{
 	SysTick->CTRL &= ~(1<<2);	
	fac_us = v_sysclk / 8;		
	fac_ms = (u16)fac_us * 1000;
}								    


void SYS_DelayUs(u32 v_us)
{		
    u32 temp;	    	 
    SysTick->LOAD = v_us * fac_us; 
    SysTick->VAL = 0x00;        
    SysTick->CTRL = 0x01 ;     
    do
    {
        temp=SysTick->CTRL;
    }
    while((temp&0x01) && !(temp&(1<<16)));
    SysTick->CTRL = 0x00;     
    SysTick->VAL = 0X00;      
}

void SYS_DelayMs(u16 v_ms)
{	 		  	  
    u32 temp;		   
    SysTick->LOAD = (u32)v_ms * fac_ms;
    SysTick->VAL = 0x00;         
    SysTick->CTRL = 0x01 ;          
    do
    {
        temp=SysTick->CTRL;
    }
    while((temp&0x01) && !(temp&(1<<16)));
    SysTick->CTRL = 0x00;       
    SysTick->VAL = 0X00;       
} 
			 




































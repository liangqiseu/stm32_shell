#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  

void SYS_DelayInit(u8 v_sysclk);
void SYS_DelayMs(u16 v_ms);
void SYS_DelayUs(u32 v_us);

#endif






























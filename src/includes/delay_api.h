#ifndef __DELAY_API_H_
#define __DELAY_API_H_ 			   
#include "sys.h"  

void SYS_DelayInit(u8 v_sysclk);
void SYS_DelayMs(u16 v_ms);
void SYS_DelayUs(u32 v_us);

#endif






























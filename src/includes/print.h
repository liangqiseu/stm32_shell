#ifndef _PRINT_H_
#define _PRINT_H_

#define USART_Printf(fmt,args...) USART_PrintfFunc("%s:%d:  "fmt,__FUNCTION__,__LINE__,##args)
void USART_PrintfFunc(const char* fmt,...);
#endif

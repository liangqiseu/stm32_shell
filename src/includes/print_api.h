#ifndef _PRINT_API_H_
#define _PRINT_API_H_

#define USART_Printf(fmt,args...) USART_PrintfFunc("%s:%d:  "fmt,__FUNCTION__,__LINE__,##args)
void USART_PrintfFunc(const char* fmt,...);
void DEBUG_CallTrace(void);
#endif

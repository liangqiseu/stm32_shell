#include <stdarg.h>
#include <stdio.h>
#include <unwind.h> // GCC's internal unwinder, part of libgcc
#include "prph_api.h"


#define PRINT_MAX_DATA_LENGTH 100


#ifdef _NO_LIBC_PRINTF_
/**These Macro defined in stdarg.h but we make it ourself**/
typedef char* VA_LIST;


#define _INTSIZEOF(n) ((sizeof(n) + sizeof(int) -1) & ~(sizeof(int)-1))
#define VA_START(ap,v) (ap = (VA_LIST)&v + _INTSIZEOF(v))
#define VA_ARG(ap,t) (*(t*)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define VA_END(ap) (ap = (VA_LIST)0)

void printCh(const char v_ch)
{
    USART_SingleCharSend(v_ch);
    return;
}

void printInt(const int v_int)
{
    if (0 == v_int)
    {
        return;
    }
    printInt(v_int / 10);
    USART_SingleCharSend((char)(v_int%10 + '0'));
    return;
}

void printStr(const char *v_str)
{
    while (*v_str)
    {
        USART_SingleCharSend(*v_str);
        v_str++;
    }
    return;
}


/************************************
  only support %c %d %s %p
  others please modify yourself
************************************/
void USART_Printf(const char* fmt,...)
{
    int valInt;
    char valChar;
    char* valStr;
    VA_LIST ap;
    VA_START(ap,fmt);
    while (*fmt)
    {
        if ('%' != *fmt)
        {
            USART_SingleCharSend(*fmt);
            fmt++;
        }
        else
        {
            fmt++;
            switch(*fmt)
            {
                case 'd':
                {
                    valInt = VA_ARG(ap,int);
                    printInt(valInt);
                    fmt++;
                    break;
                }
 
                case 's':
                {
                    valStr = VA_ARG(ap,char*);
                    printStr(valStr);
                    fmt++;
                    break;
                }

                case 'c':
                {
                    valChar = VA_ARG(ap,char);
                    printCh(valChar);
                    fmt++;
                    break;
                }

                case 'p':
                {
                    fmt++;
                    break;
                }
 
                default:
                {
                    USART_SingleCharSend(*fmt);
                    fmt++;
                    break;
                }
            }
        }
    }
    VA_END(ap);

    return;
}

#else

#define USART_Printf(fmt,...) USART_PrintfFunc(const char *fmt,_FUNCTION_,##_VA_ARGS_)

char dataBuf[PRINT_MAX_DATA_LENGTH] = {0};
void USART_PrintfFunc(const char *fmt,...)
{
    va_list ap;
    va_start(ap,fmt);
    vsprintf(dataBuf,fmt,ap);
    USART_StringSend(dataBuf);
    return;
}


_Unwind_Reason_Code trace_fcn(_Unwind_Context *ctx, void *d)
{
    int *depth = (int*)d;
    USART_PrintfFunc("\t#%d: program counter at 0x%x\r\n", *depth, _Unwind_GetIP(ctx));
    (*depth)++;
    return _URC_NO_REASON;
}



/*
	arm-none-eabi not support <execinfo.h>, so I copy an alternative procedure from StackFlow, 
	uh... it can't display the function name and only shows address. I know it is ugly but Just a moment please!
*/
void DEBUG_CallTrace(void)
{
    int depth = 0;
	USART_PrintfFunc("\r\n\t CallTrace info:\r\n");
    _Unwind_Backtrace(&trace_fcn, &depth);
	return;
}

#endif


#ifndef _SHELL_H_
#define _SHELL_H_
#include "os_api.h"

#define	SYMBOL_TEXT 0x1
#define	SYMBOL_BSS  0x2
#define SYMBOL_DATA 0x4
#define SYMBOL_ABS  0x8
#define SYMBOL_GLOABLE 0x10


typedef struct tagSHELL_NODE
{
	struct tagSHELL_NODE *next;
}SHELL_NODE_S;


typedef struct tagSYMBOL_TABAL
{
	SHELL_NODE_S magicNum;
	char* name;
	void *addr;
	u32 size;
	u32 symRef;
	u32 group;
	u32 type; 
}SYMBOL_TABAL_S;

OS_TASK_RETURN_E OS_ShellTask(void);

#endif

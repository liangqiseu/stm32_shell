#ifndef _SYMBOL_API_H_
#define _SYMBOL_API_H_

#define	SYMBOL_TEXT 0x1
#define	SYMBOL_BSS  0x2
#define SYMBOL_DATA 0x4
#define SYMBOL_ABS  0x8
#define SYMBOL_GLOBAL 0x10


typedef struct tagSHELL_NODE
{
	struct tagSHELL_NODE *next;
}SHELL_NODE_S;


typedef struct tagSYMBOL_TABAL
{
	SHELL_NODE_S magicNum;
	char* name;
	void *addr;
	unsigned int size;
	unsigned int symRef;
	unsigned int group;
	unsigned int type; 
}SYMBOL_TABAL_S;



#endif

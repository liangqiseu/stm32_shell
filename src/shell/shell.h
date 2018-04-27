#ifndef _SHELL_H_
#define _SHELL_H_

#define SHELL_CMD_NOT_FIND -1
#define SHELL_CMD_MAX_PARA_NUM 5
#define SHELL_CMD_MAX_PARA_LENGTH 10

typedef s32(*SHELL_FUN)(u32 v_p0, u32 v_p1, u32 v_p2, u32 v_p3, u32 v_p4); 

typedef struct stSHELL_CMD
{
	u8 cmdType;
	void *cmdAddr;
	char cmdPara[SHELL_CMD_MAX_PARA_NUM][SHELL_CMD_MAX_PARA_LENGTH];
}SHELL_CMD_S;

#endif

#ifndef __SYSCALL_H__
#define __SYSCALL_H__


#include "global.h"


extern WORD __syscall_NF_WritePage(WORD block,WORD page,BYTE* buffer);

extern WORD __syscall_NF_ReadPage(WORD block,WORD page,BYTE* buffer);

extern void __syscall_LCD_ClearScr(WORD color);


#endif

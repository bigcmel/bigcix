/*
hardware_proto.h: 该文件包含了供源文件调用的与硬件有关的函数声明，有了这些声明，
源文件才能调用这些函数
*/

#ifndef __HARDWARE_PROTO_H__
#define __HARDWARE_PROTO_H__

#include "global.h"


// memory.c:

extern void MC_init();
extern void MMU_init();
extern void MMU_SwitchContext( WORD app_idx );


/* nand_flash.c:
 注意这里的函数（写，读，擦除，标志坏块）在运行之前全都不检查是否是坏块，
 而且即使操作不成功，也不会标志为坏块，
 不管是检测坏块还是标志坏块都要在函数外部使用 NF_MarkBadBlock 和 NF_IsBadBlock 函数完成
*/
extern void NF_init();
extern void NF_GetBlockPageInfo(WORD* nf_blocknum, WORD* nf_pagepblock,
				WORD* nf_mainsize, WORD* nf_sparesize);
extern WORD NF_CheckId(); // 获取厂商ID和设备ID
extern WORD NF_WritePage(WORD block,WORD page,BYTE* buffer);
extern WORD NF_ReadPage(WORD block,WORD page,BYTE* buffer);
extern WORD NF_EraseBlock(WORD block); // 擦除一个块
extern WORD NF_MarkBadBlock(WORD block); // 把一个块标记为坏块
extern WORD NF_IsBadBlock(WORD block); // 判断一个块是不是坏块

/* lcd.c: */
extern void LCD_Init();
extern void LCD_EnvidOnOff(int onoff);
extern void LCD_PutPixel(WORD x, WORD y, WORD color);
extern void LCD_ClearScr(WORD color);
extern void LCD_PrintChar(WORD color, BYTE ch[]);


#endif

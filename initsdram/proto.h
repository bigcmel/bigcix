/*
proto.h: 该文件包含了供源文件调用的函数声明，有了这些声明，源文件才能调用这些函数
*/

#ifndef __PROTO_H__
#define __PROTO_H__

#include "global.h"

// wdog.c:
extern void WT_init();

// clock,c:
extern void CLK_init();

// gpio.c:
extern void GPIO_init();

// memory.c:
extern void MC_init();

/* lcd.c: */
extern void LCD_Init();
extern void LCD_EnvidOnOff(int onoff);
extern void LCD_PutPixel(WORD x, WORD y, WORD color);
extern void LCD_ClearScr(WORD color);


#endif

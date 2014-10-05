/*******************************************************************************/
/* lcd.c: lcd controller configure file for Samsung S3C2440                    */
/*******************************************************************************/

#include "global.h"

/* Define register address about lcd controller */

#define LCDCON1 (*(volatile WORD*)0x4D000000) // lcd controller register 1
#define LCDCON2 (*(volatile WORD*)0x4D000004) // lcd controller register 2
#define LCDCON3 (*(volatile WORD*)0x4D000008) // lcd controller register 3
#define LCDCON4 (*(volatile WORD*)0x4D00000C) // lcd controller register 4
#define LCDCON5 (*(volatile WORD*)0x4D000010) // lcd controller register 5

#define LCDSADDR1 (*(volatile WORD*)0x4D000014) // 帧缓冲开始地址寄存器 1
#define LCDSADDR2 (*(volatile WORD*)0x4D000018) // 帧缓冲开始地址寄存器 2
#define LCDSADDR3 (*(volatile WORD*)0x4D00001C) // 虚拟屏地址设置

#define REDLUT (*(volatile WORD*)0x4D000020) // 红色查找表寄存器
#define GREENLUT (*(volatile WORD*)0x4D000024) // 绿色查找表寄存器
#define BLUELUT (*(volatile WORD*)0x4D000028) // 蓝色查找表寄存器

#define DITHMODE (*(volatile WORD*)0x4D00004C) // 抖动模式寄存器(STN屏)
#define TPAL (*(volatile WORD*)0x4D000050) // 临时调色板寄存器(TFT屏)

#define LCDINTPND (*(volatile WORD*)0x4D000054) // LCD 中断挂起寄存器
#define LCDSRCPND (*(volatile WORD*)0x4D000058) // LCD 源挂起寄存器
#define LCDINTMSK (*(volatile WORD*)0x4D00005C) // LCD 中断屏蔽寄存器
#define TCONSEL (*(volatile WORD*)0x4D000060) // TCON 控制寄存器


/* Values of lcd controller register */

// LCDCON1 的设置参数
#define LCD_CLK_Val 9 // 手册P298，该参数决定了 VCLK 的频率
#define LCD_PNRMODE_Val 3 // 显示模式选择为 TFT LCD
#define LCD_BPPMODE_Val 13 // BPP（位每像素）模式：TFT的24bpp

// LCDCON2 的设置参数
#define LCD_VBPD_Val (15-1) // 垂直同步信号后肩（数据通过查询 LCD 屏的手册得知）
#define LCD_LINE_Val (240-1) // 垂直尺寸（数据通过查询 LCD 屏的手册得知）
#define LCD_VFPD_Val (12-1) // 垂直同步信号前肩（数据通过查询 LCD 屏的手册得知）
#define LCD_VSPW_Val (3-1) // 垂直同步信号脉宽（数据通过查询 LCD 屏的手册得知）

// LCDCON3 的设置参数
#define LCD_HBPD_Val (38-1) // 水平同步信号后肩（数据通过查询 LCD 屏的手册得知）
#define LCD_HOZ_Val (320-1) // 水平尺寸（数据通过查询 LCD 屏的手册得知）
#define LCD_HFPD_Val (20-1) // 水平同步信号前肩（数据通过查询 LCD 屏的手册得知）
#define LCD_HSPW_Val (30-1) // 水平同步信号脉宽（数据通过查询 LCD 屏的手册得知）

// LCDCON4 的设置参数
#define LCD_HSPW (30-1) // 水平同步信号脉宽（数据通过查询 LCD 屏的手册得知）

// LCDCON5 的设置参数
#define LCDCON5_Val 0x0000000C // 手册P300


/* 全局变量的定义 */

// 虚拟屏的宽
#define SCR_LCD_HOZ 320

// 虚拟屏的高
#define SCR_LCD_LINE 240

// 数据缓冲区
volatile static WORD LCD_BUFFER[SCR_LCD_HOZ][SCR_LCD_LINE];

// 视口相对于虚拟屏的偏移值，手册P296
#define OFFSET ((SCR_LCD_HOZ - (LCD_HOZ_Val+1)) * 2) // 乘2表示以半字为单位

#define PAGEWIDTH ((LCD_HOZ_Val + 1) * 2)


/* Function: Setup lcd controller for S3C2440 */

void LCD_init()
{
  LCDCON1 = (LCD_CLK_Val << 8) | (0 << 7) | (LCD_PNRMODE_Val << 5) | (LCD_BPPMODE_Val << 1) | 0;
  LCDCON2 = (LCD_VBPD_Val << 24) | (LCD_LINE_Val << 14) | (LCD_VFPD_Val << 6) | LCD_VSPW_Val;
  LCDCON3 = (LCD_HBPD_Val << 19) | (LCD_HOZ_Val << 8) | LCD_HFPD_Val;
  LCDCON4 = LCD_HSPW_Val;
  LCDCON5 = LCDCON5_Val;

  LCDSADDR1 = (((WORD)LCD_BUFFER>>22)<<21) | (0x1fffff & ((WORD)LCD_BUFFER>>1)); // 手册P301
  LCDSADDR2 = LCDSADDR1 + (PAGEWIDTH + OFFSET) * (LCD_LINE_Val + 1) * 2; // 手册P301
  LCDSADDR3 = (OFFSET << 11) | PAGEWIDTH; // 手册P302

  LCDINTMSK |= 3; // 手册P305，这里屏蔽了中断服务
  TPAL = 0; // 手册P304，这里禁止了临时调色板
}

/* Function: 若 onoff 为1，则开 envid，否则关之 */

void LCD_EnvidOnOff(int onoff)
{
  if(onoff == 1)
    LCDCON1 |= 1;
  else
    LCDCON1 = 0; // 关掉之后如果要再打开，需要再初始化一次
}

/* Function: 将某一像素点赋值 */

void LCD_PutPixel(WORD x, WORD y, WORD color)
{
  if((x < SCR_LCD_HOZ) && (y < SCR_LCD_LINE))
    LCD_BUFFER[x][y] = color;
}

/* Function: 用参数 color 代表的颜色清屏 */

void LCD_ClearScr(WORD color)
{
  WORD x, y;
  
  for(y=0 ; y<SCR_LCD_LINE ; y++)
    for(x=0 ; x<SCR_LCD_HOZ ; x++)
      LCD_BUFFER[x][y] = color;
}

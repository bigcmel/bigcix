/*******************************************************************************/
/* memory.c: memory controller configure file for Samsung S3C2440              */
/*******************************************************************************/

/* 这里写的代码是以 hy57v561620 * 2 的 SDRAM 为目标的，若目标 SDRAM 不为 K9F1208此，则不应用本代码
   hy57v561620 参数：(4M * 4Bank * 16bit)，所以一片的存储空间为32MB，寻址矩阵是13行，9列，所以能寻址到4M，由于配了2片，所以可以看成数据总线为32bit， 这样两片的存储空间就是64MB了
 */

#include "global.h"

/* Define register address about memory controller */

#define MC_BASE (*(volatile WORD*)0x48000000) // memory controller base address
#define BWSCON (*(volatile WORD*)(MC_BASE + 0x00)) // Bus width and wait status ctrl
#define BANKCON0 (*(volatile WORD*)(MC_BASE + 0x04)) // Bank 0 control register
#define BANKCON1 (*(volatile WORD*)(MC_BASE + 0x08)) // Bank 1 control register
#define BANKCON2 (*(volatile WORD*)(MC_BASE + 0x0C)) // Bank 2 control register
#define BANKCON3 (*(volatile WORD*)(MC_BASE + 0x10)) // Bank 3 control register
#define BANKCON4 (*(volatile WORD*)(MC_BASE + 0x14)) // Bank 4 control register
#define BANKCON5 (*(volatile WORD*)(MC_BASE + 0x18)) // Bank 5 control register
#define BANKCON6 (*(volatile WORD*)(MC_BASE + 0x1C)) // Bank 6 control register
#define BANKCON7 (*(volatile WORD*)(MC_BASE + 0x20)) // Bank 7 control register
#define REFRESH (*(volatile WORD*)(MC_BASE + 0x24)) // SDRAM refresh control register
#define BANKSIZE (*(volatile WORD*)(MC_BASE + 0x28)) // Flexible bank size register
#define MRSRB6 (*(volatile WORD*)(MC_BASE + 0x2C)) // Bank 6 mode register
#define MRSRB7 (*(volatile WORD*)(MC_BASE + 0x30)) // Bank 7 mode register


/* Values of memory register */

#define BWSCON_Val 0x22000000 // 手册P127，这里将 DW6 和 DW7 置10，即设置 Bank6 和 Bank7 的数据总线宽度为 32 位
#define BANKCON0_Val 0x00000700 // 手册P129，Tacc 置111，即设置访问周期为14个周期
#define BANKCON1_Val 0x00000700
#define BANKCON2_Val 0x00000700
#define BANKCON3_Val 0x00000700
#define BANKCON4_Val 0x00000700
#define BANKCON5_Val 0x00000700
#define BANKCON6_Val 0x00018005 // 手册P130，MT 两位置1，表示 bank6 使用 SDRAM，列地址数为 9 位
#define BANKCON7_Val 0x00018005 // 手册P130，MT 两位置1，表示 bank7 使用 SDRAM，列地址数为 9 位
#define REFRESH_Val 0x008404F3 // 手册P131，置了 23，18，10，7，6，5，4，1，0 位
#define BANKSIZE_Val 0x00000031 // 手册P132，这里设 bank6 和 bank7 的大小为 64M
#define MRSRB6_Val 0x00000020 // 手册P133
#define MRSRB7_Val 0x00000020


/* Function: Setup memory controller for S3C2440 */

void MC_init()
{
  BWSCON = BWSCON_Val;
  BANKCON0 = BANKCON0_Val;
  BANKCON1 = BANKCON1_Val;
  BANKCON2 = BANKCON2_Val;
  BANKCON3 = BANKCON3_Val;
  BANKCON4 = BANKCON4_Val;
  BANKCON5 = BANKCON5_Val;
  BANKCON6 = BANKCON6_Val;
  BANKCON7 = BANKCON7_Val;
  REFRESH = REFRESH_Val;
  BANKSIZE = BANKSIZE_Val;
  MRSRB6 = MRSRB6_Val;
  MRSRB7 = MRSRB7_Val;
}

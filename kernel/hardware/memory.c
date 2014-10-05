/*******************************************************************************/
/* memory.c: memory controller configure file for Samsung S3C2440              */
/*******************************************************************************/

/* 这里写的代码是以 hy57v561620 * 2 的 SDRAM 为目标的，若目标 SDRAM 不为 K9F1208此，则不应用本代码
   hy57v561620 参数：(4M * 4Bank * 16bit)，所以一片的存储空间为32MB，寻址矩阵是13行，9列，所以能寻址到4M，由于配了2片，所以可以看成数据总线为32bit， 这样两片的存储空间就是64MB了
 */

#include "../include/global.h"

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


#define SDRAM_BASE 0x30000000

void MMU_init()
{
  WORD ttb_base = SDRAM_BASE;

  /* 下面用到的页码均是《ARM体系结构与编程》这本书的页码 */

  __asm__
    (
     "MOV R0, #0\n"
     "MCR P15, 0, R0, C7, C7, 0\n" //p205 使无效指令cache和数据cache      
     "MCR P15, 0, R0, C7, C10, 4\n" //p205 清空写缓冲区
     "MCR P15, 0, R0, C8, C7, 0\n" //p189 使无效快表内容

     "MOV R4, %0\n" // 读取页表的基地址到 R4
     "MCR P15, 0, R4, C2, C0, 0\n" // C2 中存储着页表的基地址

     "MOV R0, #0x0000000D\n" // P188，该数表示域0为用户类型，域1为管理者类型
     "MCR P15, 0, R0, C3, C0, 0\n" // 设置 MMU 的域


     "MRC P15, 0, R0, C1, C0, 0\n" // 读控制寄存器 C1 的当前值

     // 以下设置参考 P174
     "BIC R0, R0, #0x0080\n" // 清除B位，选择小端内存模式
     "ORR R0, R0, #0x0002\n" // A位，使能地址对齐检查功能
     "ORR R0, R0, #0x0004\n" // C位，使能数据cache
     "ORR R0, R0, #0x1000\n" // I位，使能指令cache
     "ORR R0, R0, #0x0008\n" // W位，使能写入缓冲
     "ORR R0, R0, #0x2000\n" // V位，控制向量表的位置为 0xFFFF0000～0xFFFF001C
     "ORR R0, R0, #0x0001\n" // M位，使能 MMU

     "MCR P15, 0, R0, C1, C0, 0\n": /* 汇编语句部分结束 */ : /* 无输出部分 */
     "r"( ttb_base ) // 输入部分，传入一个参数，以占位符'%0'表示
     );

}

void MMU_SwitchContext( WORD app_idx )
{
  // 参考《ARM体系结构与编程》P208
  WORD PID = (app_idx << 25);

  __asm__
    (
     "MOV R0, %0\n"
     "MCR P15, 0, R0, C13, C0, 0\n": /*汇编语句结束*/ : /*没有输出*/
     "r"(PID) // %0 占位符即代表 PID
     );

}

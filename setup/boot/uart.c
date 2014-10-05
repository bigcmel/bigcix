/*******************************************************************************/
/* uart.c: uart configure file for Samsung S3C2440                             */
/*******************************************************************************/

#include "global.h"

/* Define register address about uart */

#define ULCON0 (*(volatile WORD*)0x50000000) // uart线路控制寄存器
#define UCON0 (*(volatile WORD*)0x50000004) // uart控制寄存器
#define UFCON0 (*(volatile WORD*)0x50000008) // uart FIFO 控制寄存器
#define UMCON0 (*(volatile WORD*)0x5000000C) // uart modem 控制寄存器
#define UTRSTAT0 (*(volatile WORD*)0x50000010) // uart TX/RX 状态寄存器
#define UERSTAT0 (*(volatile WORD*)0x50000014) // uart 错误状态寄存器
#define UFSTAT0 (*(volatile WORD*)0x50000018) // uart FIFO 状态寄存器
#define UMSTAT0 (*(volatile WORD*)0x5000001C) // uart modem 状态寄存器
#define UTXH0 (*(volatile WORD*)0x50000020) // uart 发送缓冲寄存器
#define URXH0 (*(volatile WORD*)0x50000024) // uart 接收缓冲寄存器
#define UBRDIV0 (*(volatile WORD*)0x50000028) // uart 波特率分频寄存器

/* Values of uart register */

#define ULCON0_Val 0x3 // 手册P235
#define UCON0_Val 0x5 // 手册P236
#define UFCON0_Val 0x0 // 手册P238
#define UMCON0_Val 0x0 // 手册P238

/*/ clock_init in startup.c :
int FCLK = 40500000; // FCLK = 40500000
int HCLK = 101250000; // HCLK = FCLK / 4
int PCLK = 50625000; // PCLK = HCLK / 2
*/

/* Function: Setup uart for S3C2440 */

void Uart_init(int baud)
{
  int i;

  ULCON0 = ULCON0_Val;
  UCON0 = UCON0_Val;
  UFCON0 = UFCON0_Val;
  UMCON0 = UMCON0_Val;

  // 波特率算术公式： UBRDIV0 = (int)((int)PCLK/16*baud+0.5)-1;
  UBRDIV0 = 26;

  for(i=0;i<100;i++); 
}

/* FIFO 使能下使用
#define Tx_FIFO_FULL 0x4000
*/
void Uart_SendByte(char ch)
{
  // while( UFSTAT0 & Tx_FIFO_FULL );// 当发送 FIFO 未满时才继续，FIFO 使能下使用

  while(!(UTRSTAT0 & 0x2));
  UTXH0 = ch;
}

int Uart_SendString(const char* str, unsigned int len)
{
  unsigned int counter;

  for(counter = 0 ; counter < len ; counter++)
    Uart_SendByte(str[counter]);

  return counter;
}

/*******************************************************************************/
/* gpio.c: gpio configure file for Samsung S3C2440                             */
/*******************************************************************************/

#include "global.h"

/* Define register address about gpio */

#define GPA_CON (*(volatile WORD*)0x56000000) // GPA control register address
#define GPB_CON (*(volatile WORD*)0x56000010) // GPB control register address
#define GPC_CON (*(volatile WORD*)0x56000020) // GPC control register address
#define GPD_CON (*(volatile WORD*)0x56000030) // GPD control register address
#define GPE_CON (*(volatile WORD*)0x56000040) // GPE control register address
#define GPF_CON (*(volatile WORD*)0x56000050) // GPF control register address
#define GPG_CON (*(volatile WORD*)0x56000060) // GPG control register address
#define GPH_CON (*(volatile WORD*)0x56000070) // GPH control register address
#define GPJ_CON (*(volatile WORD*)0x560000D0) // GPJ control register address

#define GPA_DAT (*(volatile WORD*)(GPA_CON + 0x04)) // GPA data register
#define GPB_DAT (*(volatile WORD*)(GPB_CON + 0x04)) // GPB data register
#define GPC_DAT (*(volatile WORD*)(GPC_CON + 0x04)) // GPC data register
#define GPD_DAT (*(volatile WORD*)(GPD_CON + 0x04)) // GPD data register
#define GPE_DAT (*(volatile WORD*)(GPE_CON + 0x04)) // GPE data register
#define GPF_DAT (*(volatile WORD*)(GPF_CON + 0x04)) // GPF data register
#define GPG_DAT (*(volatile WORD*)(GPG_CON + 0x04)) // GPG data register
#define GPH_DAT (*(volatile WORD*)(GPH_CON + 0x04)) // GPH data register
#define GPJ_DAT (*(volatile WORD*)(GPJ_CON + 0x04)) // GPJ data register

// GPA 没有上拉使能寄存器

#define GPB_UP (*(volatile WORD*)(GPB_CON + 0x08)) // GPB pull-up disable register
#define GPC_UP (*(volatile WORD*)(GPC_CON + 0x08)) // GPC pull-up disable register
#define GPD_UP (*(volatile WORD*)(GPD_CON + 0x08)) // GPD pull-up disable register
#define GPE_UP (*(volatile WORD*)(GPE_CON + 0x08)) // GPE pull-up disable register
#define GPF_UP (*(volatile WORD*)(GPF_CON + 0x08)) // GPF pull-up disable register
#define GPG_UP (*(volatile WORD*)(GPG_CON + 0x08)) // GPG pull-up disable register
#define GPH_UP (*(volatile WORD*)(GPH_CON + 0x08)) // GPH pull-up disable register
#define GPJ_UP (*(volatile WORD*)(GPJ_CON + 0x08)) // GPJ pull-up disable register


/* Values of gpio register */

// GPA 的引脚都是用于传输存储器信号的，包括 nand_flash 存储器
#define GPACON_Val 0x00FFFFFF // 手册P191，这里相当于将GPA0～GPA11这些引脚作为ADDR0，ADDR16～ADDR26的功能引脚，而不是作为输出引脚，并启用了所有的 nand_flash 相关引脚

#define GPBCON_Val 0x00000000
#define GPBUP_Val 0x00000000 // 清0代表使能附加上拉功能到相应端口管脚

// GPC 与 GPD 的引脚都使用于传输 LCD 信号的
#define GPCCON_Val 0xaaaafeaa // 手册P193,VD[0]~VD[7],LEND,VCLK,VLINE,VFRAME,VM,其他保留
#define GPCUP_Val 0xFFFFFFFF // Disable pull-up register

#define GPDCON_Val 0xaaaaaaaa // 手册P194，VD[8]~VD[23]，用于LCD数据的传输 
#define GPDUP_Val 0xFFFFFFFF // Disable pull-up register

#define GPECON_Val 0x00000000
#define GPEUP_Val 0x00000000

#define GPFCON_Val 0x00000000
#define GPFUP_Val 0x00000000

#define GPGCON_Val 0x00000000
#define GPGUP_Val 0x00000000

// GPH 的引脚多半是作为串口的接口
#define GPHCON_Val 0xfaaa // 这里启用了 uart0 和 uart1 的 RXD，TXD，CTS，RTS
#define GPHUP_Val 0x7ff // Disable pull-up register

#define GPJCON_Val 0x00000000
#define GPJUP_Val 0x00000000


/* Function: Setup gpio for S3C2440 */

void GPIO_init()
{
  GPA_CON = GPACON_Val;

  GPB_CON = GPBCON_Val;
  GPB_UP = GPBUP_Val;

  GPC_CON = GPCCON_Val;
  GPC_UP = GPCUP_Val;

  GPD_CON = GPDCON_Val;
  GPD_UP = GPDUP_Val;

  GPE_CON = GPECON_Val;
  GPE_UP = GPEUP_Val;

  GPF_CON = GPFCON_Val;
  GPF_UP = GPFUP_Val;

  GPG_CON = GPGCON_Val;
  GPG_UP = GPGUP_Val;

  GPH_CON = GPHCON_Val;
  GPH_UP = GPHUP_Val;

  GPJ_CON = GPJCON_Val;
  GPJ_UP = GPJUP_Val;

}

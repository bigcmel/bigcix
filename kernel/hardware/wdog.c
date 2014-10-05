/*******************************************************************************/
/* wdog.c: watchdog configure file for Samsung S3C2440                        */
/*******************************************************************************/

#include "../include/global.h"

/* Define register address about watchdog */

#define WT_BASE (*(volatile WORD*)0x53000000) // watchdog timer base address
#define WT_CON (*(volatile WORD*)(WT_BASE + 0x00)) //watchdog timer control register address
#define WT_DAT (*(volatile WORD*)(WT_BASE + 0x04)) //watchdog timer data register address
#define WT_CNT (*(volatile WORD*)(WT_BASE + 0x08)) //watchdog timer count register address


/* Values of watchdog register */

#define WTCON_Val 0x00000000 // 手册P325 这里禁止了看门狗
#define WTDAT_Val 0x00008000


/* Function: Setup watchdog for S3C2440 */

void WT_init()
{
  WT_CNT = WTDAT_Val;
  WT_DAT = WTDAT_Val;
  WT_CON = WTCON_Val;
}

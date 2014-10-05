/*******************************************************************************/
/* clock.c: clock configure file for Samsung S3C2440                           */
/*******************************************************************************/

#include "global.h"

/* Define register address about clock */

#define CLK_BASE (*(volatile WORD*)0x4C000000) // clock base address
#define LOCKTIME (*(volatile WORD*)(CLK_BASE + 0x00)) // PLL lock time count register
#define MPLLCON (*(volatile WORD*)(CLK_BASE + 0x04)) // MPLL configuration register
#define UPLLCON (*(volatile WORD*)(CLK_BASE + 0x08)) // UPLL configuration register
#define CLK_CON (*(volatile WORD*)(CLK_BASE + 0x0C)) // clock generator control register
#define CLK_SLOW (*(volatile WORD*)(CLK_BASE + 0x10)) // clock slow control register
#define CLK_DIVN (*(volatile WORD*)(CLK_BASE + 0x14)) // divider control register
#define CAM_DIVN (*(volatile WORD*)(CLK_BASE + 0x18)) // clock divider register


/* Values of clock register */

#define LOCKTIME_Val 0x0FFF0FFF // 手册P168
#define MPLLCON_Val 0x00043011
#define UPLLCON_Val 0x00038021
#define CLKCON_Val 0x001FFFF0 // 手册P170
#define CLKSLOW_Val 0x00000004 // 手册P171
#define CLKDIVN_Val 0x0000000F // 手册P172
#define CAMDIVN_Val 0x00000000 // 手册P173


/* Function: Setup clock for S3C2440 */

void CLK_init()
{
  LOCKTIME = LOCKTIME_Val;
  CLK_DIVN = CLKDIVN_Val;
  CAM_DIVN = CAMDIVN_Val;
  MPLLCON = MPLLCON_Val;
  UPLLCON = UPLLCON_Val;
  CLK_SLOW = CLKSLOW_Val;
  CLK_CON = CLKCON_Val;
}

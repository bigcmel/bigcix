#include "proto.h"

void __main()
{

  /* 各种初始化 */

  WT_init();
  
  CLK_init();

  MC_init();

  GPIO_init();

  LCD_init();
  LCD_EnvidOnOff(1);
  LCD_ClearScr(0x0C0C0C);

};

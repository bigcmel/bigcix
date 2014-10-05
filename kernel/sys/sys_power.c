#include "sys_power.h"

void sys_power_sleep()
{
  Uart_SendString("NOW SLEEP.\n",11);
  while(1){};
}


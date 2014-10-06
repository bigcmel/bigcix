#include "proto.h"

#define SETUPLOADER_BASE_ADDR 0x31000000


BYTE* __main()
{
  GPIO_init();

  Uart_init(115200);

  Uart_SendString("setup boot.\n", 12);

  return (BYTE*)SETUPLOADER_BASE_ADDR;
}

// gcc 的静态库要求链接到的函数，为空就好
void raise()
{}

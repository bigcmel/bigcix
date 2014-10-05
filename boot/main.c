#include "proto.h"

#define LOADER_BASE_ADDR 0x31000000 // loader.bin 加载到内存中的地址

#define LOADER_BLOCK_BASE 0 // loader.bin 在 nand flash 中所处的块号
#define LOADER_PAGE_BASE 2 // loader.bin 在 nand flash 中所处的页号
#define LOADER_BLOCK_NUM 1 // loader.bin 在 nand 中所占俄块数
#define LOADER_PAGE_NUM 3 // loader.bin 所占的页数

void print_nand_id();

BYTE* __main()
{

  BYTE* loader_base;
  BYTE* ptr;

  WORD i;

  // 获取 nand_flash 的块数，页数，页大小等等信息
  WORD nf_blocknum, nf_pagepblock, nf_mainsize, nf_sparesize;
  
  loader_base = (BYTE*)LOADER_BASE_ADDR;
  ptr = loader_base;


  GPIO_init();

  Uart_init(115200);
  Uart_SendString("Boot!\n",6);

  //  while(1){}

  NF_init();

  /* 将 nand_flash 的 0 号块的 2～4 号页的内容（即存储了 loader.bin 的一段程序）
   复制到内存的 LOADER_BASE_ADDR 处
  */

  // 获取 nand_flash 的块数，页数，页大小等等信息

  NF_GetBlockPageInfo(&nf_blocknum, &nf_pagepblock, &nf_mainsize, &nf_sparesize);

  for(i=0 ; i<LOADER_PAGE_NUM ; i++)
    {
      if( NF_ReadPage(LOADER_BLOCK_BASE, LOADER_PAGE_BASE+i, ptr) )
	ptr += nf_mainsize;
      else
	Uart_SendString("Read Fail!\n",11);
    }

  return (BYTE*)LOADER_BASE_ADDR;
}

// gcc 的静态库要求链接到的函数，为空就好
void raise()
{}

void print_nand_id()
{
  HWORD id;
  BYTE maker, device;

  /*
  device = (BYTE)id;
  maker = (BYTE)(id >> 8);
  Uart_SendByte(maker);
  Uart_SendByte(device);
  Uart_SendByte('\n');
  */

  id = NF_CheckId();
  device = (BYTE)id;
  maker = (BYTE)(id >> 8);

  Uart_SendByte(maker);
  Uart_SendByte(device);
  Uart_SendByte('\n');
}

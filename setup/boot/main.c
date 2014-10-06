#include "proto.h"

#define BOOT_BASE_ADDR 0x30000000
#define LOADER_BASE_ADDR 0x30001000
#define KERNEL_BASE_ADDR 0x30004000
#define SETUPLOADER_BASE_ADDR 0x31000000

#define BOOT_BLOCK_BASE 0
#define BOOT_PAGE_BASE 0
#define BOOT_BLOCK_NUM 1
#define BOOT_PAGE_NUM 2

#define LOADER_BLOCK_BASE 0
#define LOADER_PAGE_BASE 2
#define LOADER_BLOCK_NUM 1
#define LOADER_PAGE_NUM 3

#define KERNEL_BLOCK_BASE 1
#define KERNEL_PAGE_BASE 0
#define KERNEL_BLOCK_NUM 1
#define KERNEL_PAGE_NUM 32

BYTE* __main()
{

  int i;
  BYTE* src_ptr;

  // 获取 nand_flash 的块数，页数，页大小等等信息
  WORD nf_blocknum, nf_pagepblock, nf_mainsize, nf_sparesize;


  GPIO_init();

  Uart_init(115200);

  NF_init();

  // 获取 nand_flash 的块数，页数，页大小等等信息
  NF_GetBlockPageInfo(&nf_blocknum, &nf_pagepblock, &nf_mainsize, &nf_sparesize);

  // 要写之前必须先擦除
  for(i=BOOT_BLOCK_BASE; i<(KERNEL_BLOCK_BASE+1); i++)
    if( NF_EraseBlock(i) == 0 )
      Uart_SendString("Erase Fail!\n",12);

  // 开始安装 boot 到 nand
  src_ptr = (BYTE*)BOOT_BASE_ADDR;

  if(BOOT_BLOCK_NUM == 1)
    {
      Uart_SendString("setup boot to nand...\n",22);

      for(i=BOOT_PAGE_BASE; i<(BOOT_PAGE_BASE + BOOT_PAGE_NUM); i++)
	{
	  if( NF_WritePage(BOOT_BLOCK_BASE, i, src_ptr) == 0 )
	    Uart_SendString("Write Fail!\n",12);
      
	  src_ptr += nf_mainsize;
	}
    }

  // 开始安装 loader 到 nand
  src_ptr = (BYTE*)LOADER_BASE_ADDR;

  if(LOADER_BLOCK_NUM == 1)
    {
      Uart_SendString("setup loader to nand...\n",24);

      for(i=LOADER_PAGE_BASE; i<(LOADER_PAGE_BASE + LOADER_PAGE_NUM); i++)
	{
	  if( NF_WritePage(LOADER_BLOCK_BASE, i, src_ptr) == 0 )
	    Uart_SendString("Write Fail!\n",12);
      
	  src_ptr += nf_mainsize;
	}
    }

  // 开始安装 kernel 到 nand
  src_ptr = (BYTE*)KERNEL_BASE_ADDR;

  if(KERNEL_BLOCK_NUM == 1)
    {
      Uart_SendString("setup kernel to nand...\n",24);

      for(i=KERNEL_PAGE_BASE; i<(KERNEL_PAGE_BASE + KERNEL_PAGE_NUM); i++)
	{
	  if( NF_WritePage(KERNEL_BLOCK_BASE, i, src_ptr) == 0 )
	    Uart_SendString("Write Fail!\n",12);
      
	  src_ptr += nf_mainsize;
	}
    }


  while(1){}

  return (BYTE*)SETUPLOADER_BASE_ADDR;
}

// gcc 的静态库要求链接到的函数，为空就好
void raise()
{}

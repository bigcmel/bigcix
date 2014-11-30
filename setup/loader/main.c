#include "proto.h"

#define BOOT_BASE_ADDR 0x30000000
#define LOADER_BASE_ADDR 0x30001000
#define KERNEL_BASE_ADDR 0x30004000
#define INITD_BASE_ADDR 0x32000000
#define SERVM_BASE_ADDR 0x31200000
#define UART_BASE_ADDR 0x31202000
#define APPM_BASE_ADDR 0x31204000
#define POWER_BASE_ADDR 0x31206000
/* 下面这些 bin 暂且没有写入 nand flash
#define APP_APP_1_BASE_ADDR 0x02000000
*/

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

#define INITD_BLOCK_BASE 1
#define INITD_PAGE_BASE 32
#define INITD_BLOCK_NUM 1
#define INITD_PAGE_NUM 32

#define SERVM_BLOCK_BASE 2
#define SERVM_PAGE_BASE 0
#define SERVM_BLOCK_NUM 1
#define SERVM_PAGE_NUM 4

#define UART_BLOCK_BASE 2
#define UART_PAGE_BASE 4
#define UART_BLOCK_NUM 1
#define UART_PAGE_NUM 4

#define APPM_BLOCK_BASE 2
#define APPM_PAGE_BASE 8
#define APPM_BLOCK_NUM 1
#define APPM_PAGE_NUM 4

#define POWER_BLOCK_BASE 2
#define POWER_PAGE_BASE 12
#define POWER_BLOCK_NUM 1
#define POWER_PAGE_NUM 4

#define FINAL_BLOCK_BASE 2

void __main()
{

  int i;
  BYTE* src_ptr;

  // 获取 nand_flash 的块数，页数，页大小等等信息
  WORD nf_blocknum, nf_pagepblock, nf_mainsize, nf_sparesize;


  Uart_SendString("setup loader.\n", 14);


  NF_init();

  // 获取 nand_flash 的块数，页数，页大小等等信息
  NF_GetBlockPageInfo(&nf_blocknum, &nf_pagepblock, &nf_mainsize, &nf_sparesize);

  // 要写之前必须先擦除
  for(i=BOOT_BLOCK_BASE; i<(FINAL_BLOCK_BASE + 1); i++)
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

  // 开始安装 initd 到 nand
  src_ptr = (BYTE*)INITD_BASE_ADDR;

  if(INITD_BLOCK_NUM == 1)
    {
      Uart_SendString("setup initd to nand...\n",23);

      for(i=INITD_PAGE_BASE; i<(INITD_PAGE_BASE + INITD_PAGE_NUM); i++)
	{
	  if( NF_WritePage(INITD_BLOCK_BASE, i, src_ptr) == 0 )
	    Uart_SendString("Write Fail!\n",12);
      
	  src_ptr += nf_mainsize;
	}
    }

  // 开始安装 servm 到 nand
  src_ptr = (BYTE*)SERVM_BASE_ADDR;

  if(SERVM_BLOCK_NUM == 1)
    {
      Uart_SendString("setup servm to nand...\n",23);

      for(i=SERVM_PAGE_BASE; i<(SERVM_PAGE_BASE + SERVM_PAGE_NUM); i++)
	{
	  if( NF_WritePage(SERVM_BLOCK_BASE, i, src_ptr) == 0 )
	    Uart_SendString("Write Fail!\n",12);
      
	  src_ptr += nf_mainsize;
	}
    }

  // 开始安装 uart 到 nand
  src_ptr = (BYTE*)UART_BASE_ADDR;

  if(UART_BLOCK_NUM == 1)
    {
      Uart_SendString("setup uart to nand...\n",22);

      for(i=UART_PAGE_BASE; i<(UART_PAGE_BASE + UART_PAGE_NUM); i++)
	{
	  if( NF_WritePage(UART_BLOCK_BASE, i, src_ptr) == 0 )
	    Uart_SendString("Write Fail!\n",12);
      
	  src_ptr += nf_mainsize;
	}
    }

  // 开始安装 appm 到 nand
  src_ptr = (BYTE*)APPM_BASE_ADDR;

  if(APPM_BLOCK_NUM == 1)
    {
      Uart_SendString("setup appm to nand...\n",22);

      for(i=APPM_PAGE_BASE; i<(APPM_PAGE_BASE + APPM_PAGE_NUM); i++)
	{
	  if( NF_WritePage(APPM_BLOCK_BASE, i, src_ptr) == 0 )
	    Uart_SendString("Write Fail!\n",12);
      
	  src_ptr += nf_mainsize;
	}
    }

  // 开始安装 power 到 nand
  src_ptr = (BYTE*)POWER_BASE_ADDR;

  if(POWER_BLOCK_NUM == 1)
    {
      Uart_SendString("setup power to nand...\n",23);

      for(i=POWER_PAGE_BASE; i<(POWER_PAGE_BASE + POWER_PAGE_NUM); i++)
	{
	  if( NF_WritePage(POWER_BLOCK_BASE, i, src_ptr) == 0 )
	    Uart_SendString("Write Fail!\n",12);
      
	  src_ptr += nf_mainsize;
	}
    }

  while(1){};
}

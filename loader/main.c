#include "proto.h"

const WORD SDRAM_BASE =  0x30000000; // sdram 的起始地址
#define IRAM_BASE 0x40000000 // Internal Memory Base Address
#define LOADER_BASE 0x31000000 // loader 的加载地址
#define VECTORS_VIR_BASE 0xFFFF0000 // 高位向量表的虚拟基地址
#define VECTORS_PHY_BASE 0x30F00000 // 高位向量表的物理基地址

WORD* MMU_TTB_PHY_BASE; // 页表基地址，这当然就是物理地址了，因为此时页表还没加载

#define KERNEL_VIR_BASE_ADDR 0x30004000 // kernel.bin 加载到内存中的地址，这是在建立了页表后，启动了 MMU 后的虚拟地址

#define KERNEL_BLOCK_BASE 1 // kernel.bin 的基址在 nand flash 中所处的块号
#define KERNEL_BLOCK_NUM 1 // kernel.bin 所占的块数
#define KERNEL_PAGE_BASE 0 // kernel.bin 的基址在 nand flash 中所处的页号
#define KERNEL_PAGE_NUM 32 // kernel.bin 在每块所占的页数


// 创建页表
static void create_page_table();

void print_nand_id();


BYTE* __main()
{

  /* 变量声明 */

  BYTE* kernel_base;
  BYTE* ptr;

  WORD i, j;

  // 为获取 nand_flash 的块数，页数，页大小等等信息所声明的参数
  WORD nf_blocknum, nf_pagepblock, nf_mainsize, nf_sparesize;


  Uart_SendString("Loader!\n",8);
  Uart_SendString("Create page table\n",18);

  kernel_base = (BYTE*)KERNEL_VIR_BASE_ADDR;
  ptr = kernel_base;

  /* 建立页表，并启动 MMU */

  MMU_TTB_PHY_BASE = (WORD*)SDRAM_BASE; // 确定页表的物理地址，在 SDRAM 的开头

  create_page_table(); // 建立页表

  MMU_init(); // 启动 MMU，该函数在 memory.c 中定义
  
  Uart_SendString("Create page table end\n",22);


  /* 将 nand_flash 的 0 号块的 8～15 号页的内容（即存储了 kernel.bin 的一段程序）
   复制到内存的 KERNEL_BASE_ADDR 处  
  */
  // 获取 nand_flash 的块数，页数，页大小等等信息

  NF_GetBlockPageInfo(&nf_blocknum, &nf_pagepblock, &nf_mainsize, &nf_sparesize);

  for(j=0 ; j<KERNEL_BLOCK_NUM ; j++)
    {
      for(i=0 ; i<KERNEL_PAGE_NUM ; i++)
	{
      
	  if( NF_ReadPage(KERNEL_BLOCK_BASE+j, KERNEL_PAGE_BASE+i, ptr) )
	      ptr += nf_mainsize;
	  else
	    Uart_SendString("Read Fail!\n",11);
	}
    }


  // 示意代码到此为止没出错

  //  ptr = (BYTE*)0x20000000;
  //  *ptr = 0x4a;

  //  while(1){}
  
  return (BYTE*)KERNEL_VIR_BASE_ADDR;
}

static void create_page_table()
{

  /* 这里参考的书籍是《ARM体系结构与编程》（杜春雷）第一版，
   下面所注页码均为本书页码 */


#define MMU_AP_SYS (2 << 10) // 访问权限位AP，P188，这里为2，表示该段在特权模式下允许读写，在用户模式下只读
#define MMU_AP_USER (3 << 10) // 这里AP为3，表示该段不管在特权模式，还是在用户模式下都可读可写
#define MMU_DOMAIN_0 (1 << 5) // 该段所处的域，此处是0域，P180
#define MMU_SPECIAL (1 << 4) // 该位为 1，P180
#define MMU_CACHEABLE (1 << 3) // C位为1，允许 cache
#define MMU_BUFFERABLE (1 << 2) // B位为1，允许 write buffer
#define MMU_SECTION (2 << 0) // 表示这是段描述符

  // 段描述符
#define MMU_SYS_SECDESC (MMU_AP_SYS | MMU_DOMAIN_0 | MMU_SPECIAL | MMU_CACHEABLE | MMU_BUFFERABLE | MMU_SECTION)
#define MMU_USER_SECDESC (MMU_AP_USER | MMU_DOMAIN_0 | MMU_SPECIAL | MMU_CACHEABLE | MMU_BUFFERABLE | MMU_SECTION)


  const WORD RAM_END_ADDR = 0xFF000000; // 最大寻址界限，这里暂且这样写，其实最好设为有效的最大地址

  const WORD MMU_SECTION_SIZE = 0x00100000; // 一个段占据的 1M
  const WORD PID_SECTION_SIZE = 0x02000000; // 一个进程代码段有 32M

  const WORD PID_NUM = 16; // p206，快速上下文切换技术中的进程代码段的数量

  WORD viraddr, phyaddr; // 虚拟地址与物理地址

  int i;


  /* 系统的 32M 内存空间 */
  viraddr = SDRAM_BASE;
  phyaddr = SDRAM_BASE;

  for(i=0; i<32; i++)
    {
      *(MMU_TTB_PHY_BASE + (viraddr >> 20)) = (phyaddr & 0xFFF00000) | MMU_SYS_SECDESC;
      viraddr += MMU_SECTION_SIZE;
      phyaddr += MMU_SECTION_SIZE;
    }


  /* 用户的 32M 内存空间 */
  viraddr = 0;

  for(i=0; i<PID_NUM; i++)
    {
      *(MMU_TTB_PHY_BASE + (viraddr >> 20)) = (phyaddr & 0xFFF00000) | MMU_SYS_SECDESC;
      phyaddr += MMU_SECTION_SIZE;

      *(MMU_TTB_PHY_BASE + ((viraddr + MMU_SECTION_SIZE) >> 20)) = (phyaddr & 0xFFF00000) | MMU_SYS_SECDESC;
      phyaddr += MMU_SECTION_SIZE;

      viraddr += PID_SECTION_SIZE;
    }
  

  /* 接下来就是为 IRAM_BASE 之后的地址空间重映射了，之后的空间虚拟物理保持一致 */
  viraddr = IRAM_BASE;
  phyaddr = IRAM_BASE;

  while( phyaddr < RAM_END_ADDR )
    {
      *(MMU_TTB_PHY_BASE + (viraddr >> 20)) = (phyaddr & 0xFFF00000) | MMU_SYS_SECDESC;
      viraddr += MMU_SECTION_SIZE;
      phyaddr += MMU_SECTION_SIZE;
    }


  // 高位的向量表地址
  viraddr = VECTORS_VIR_BASE;
  phyaddr = VECTORS_PHY_BASE;
  *(MMU_TTB_PHY_BASE + (viraddr >> 20)) = (phyaddr & 0xFFF00000) | MMU_SYS_SECDESC;


  /*  
  viraddr = 0x20000000;
  phyaddr = 0x30F00000;
  *(MMU_TTB_PHY_BASE + (viraddr >> 20)) = (phyaddr & 0xFFF00000) | MMU_SYS_SECDESC;
  */
}

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

#ifndef __MM_H__
#define __MM_H__

#include "../include/global.h"


// 表示 mm 是否有变动过
int MM_IS_APPLY;

#define MM_APPLY 1
#define MM_UNAPPLY 0


// ttb 表的虚拟地址
#define MM_VIR_TTB_BASE 0x30000000
#define MM_VIR_TTB_LENGTH 4096


// sdram 的物理基地址
#define MM_PHY_SDRAM_BASE 0x30000000

// SDRAM 的大小，以兆为单位
#define MM_SDRAM_SIZE 64
// 虚拟空间大小，以兆为单位
#define MM_VIR_SIZE 4000 // 即 4G

// 全局变量，记录了物理地址的每一M的使用情况
HWORD MM_SDRAM_TABLE[MM_SDRAM_SIZE];


// 变量定义于 mm.h，用于指向操作系统使用的虚拟内存映射页表
WORD* MM_PAGETABLE;

extern void mm_setup();

extern void mm_run();

extern void mm_setup_pagetable( BYTE* pagetable_base_addr );

extern WORD mm_apply_sdram(WORD app_idx, WORD vir_addr, WORD length );


#endif

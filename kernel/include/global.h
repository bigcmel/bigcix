#ifndef __GLOBAL_H__
#define __GLOBAL_H__


#define Mode_USR 0x10
#define Mode_FIQ 0x11
#define Mode_IRQ 0x12
#define Mode_SVC 0x13
#define Mode_ABT 0x17
#define Mode_UND 0x1B
#define Mode_SYS 0x1F


#define NULL 0x0
#define RETURN_SUCCESS 1
#define RETURN_FAILED (-1)


typedef unsigned char BYTE;
typedef unsigned short HWORD; // 半字，两个字节
typedef unsigned long WORD; // 字，四个字节


typedef void(*PTRFUNC)(void); // PTRFUNC 是一个指向函数的指针



// 内存的分配情况如下

// 页表
#define PAGE_TABLE_BASE 0x30000000
#define PAGE_TABLE_LIMIT 0x00004000

// 内核
#define KERNEL_BASE 0x30004000
#define KERNEL_LIMIT (INITD_REGISTER_TABLE_BASE - KERNEL_BASE)

// INITD
#define INITD_REGISTER_TABLE_BASE 0x30EFF000
#define INITD_REGISTER_TABLE_LIMIT 0x00001000

// LOADER
#define LOADER_BASE 0x31000000
#define LOADER_LIMIT 0x00100000

// SERV
#define SERV_REGISTER_TABLE_BASE 0x31100000
#define SERV_REGISRER_TABLE_LIMIT 0x00100000

#define SERV_CODE_BASE 0x31200000
#define SERV_CODE_LIMIT 0x00C00000

#define SERV_SHARE_RAM_BASE 0x31E00000
#define SERV_SHARE_RAM_LIMIT 0x00100000

// 内核栈空间（栈是从高到地生长，所以base是高地址）
#define SYS_STACK_BASE 0x32000000
#define SYS_STACK_LIMIT 0x00100000



#endif


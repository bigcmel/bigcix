#ifndef __SERV_INFO_H__
#define __SERV_INFO_H__

// 该服务的代码段起始地址
#define SERV_CODE_SEG_BASE 0x31202000


/* Define register address about uart */

#define ULCON0 (*(volatile WORD*)0x50000000) // uart线路控制寄存器
#define UCON0 (*(volatile WORD*)0x50000004) // uart控制寄存器
#define UFCON0 (*(volatile WORD*)0x50000008) // uart FIFO 控制寄存器
#define UMCON0 (*(volatile WORD*)0x5000000C) // uart modem 控制寄存器
#define UTRSTAT0 (*(volatile WORD*)0x50000010) // uart TX/RX 状态寄存器
#define UERSTAT0 (*(volatile WORD*)0x50000014) // uart 错误状态寄存器
#define UFSTAT0 (*(volatile WORD*)0x50000018) // uart FIFO 状态寄存器
#define UMSTAT0 (*(volatile WORD*)0x5000001C) // uart modem 状态寄存器
#define UTXH0 (*(volatile WORD*)0x50000020) // uart 发送缓冲寄存器
#define URXH0 (*(volatile WORD*)0x50000024) // uart 接收缓冲寄存器
#define UBRDIV0 (*(volatile WORD*)0x50000028) // uart 波特率分频寄存器

/* Values of uart register */

#define ULCON0_Val 0x3 // 手册P235
#define UCON0_Val 0x5 // 手册P236
#define UFCON0_Val 0x0 // 手册P238
#define UMCON0_Val 0x0 // 手册P238



// 下面即该服务提供的所有函数所对应的操作码
#define SERV_UART_INIT 0x1
#define SERV_UART_SendByte 0x2
#define SERV_UART_SendString 0x3


// 下面即该服务提供的所有函数
// 下面的函数全部出于 serv_lib_funcs.c
extern void serv_uart_init();
extern void serv_uart_SendByte(WORD* para_list, WORD para_num);
extern void serv_uart_SendString(WORD* para_list, WORD para_num);



#endif

#ifndef __APP_GLOBAL_H__
#define __APP_GLOBAL_H__


#define NULL 0x0


typedef unsigned char BYTE;
typedef unsigned short HWORD; // 半字，两个字节
typedef unsigned long WORD; // 字，四个字节

typedef void(*PTRFUNC)(void); // PTRFUNC 是一个指向函数的指针


// 一个 sys 服务的参数值上限
#define SERV_MAX_PARA_NUM 8


// servm 服务部分
#define SERV_SERVM_IDX 0x0

// 下面即该服务提供的所有函数所对应的操作码
#define SERV_SERVM_init 0x1
#define SERV_SERVM_add_serv 0x2
#define SERV_SERVM_register_serv 0x3
#define SERV_SERVM_get_empty_idx 0x4

#define SERV_SERVM_INIT_PARA_NUM 0
extern WORD serv_servm_init();

#define SERV_SERVM_ADD_SERV_PARA_NUM 8
extern WORD serv_servm_add_serv(WORD serv_idx, WORD code_seg_base, WORD code_seg_limit, WORD para_seg_base, WORD block_base, WORD page_base, WORD block_num, WORD page_num);

#define SERV_SERVM_REGISTER_SERV_PARA_NUM 3
extern WORD serv_servm_register_serv(WORD code_seg_base, WORD code_seg_limit, WORD para_seg_base);

#define SERV_SERVM_GET_EMPTY_IDX_PARA_NUM 0
extern WORD serv_servm_get_empty_idx();


// uart 服务部分
#define SERV_UART_IDX 0x1

#define SERV_UART_init 0x1        // 操作码定义
#define SERV_UART_SendByte 0x2
#define SERV_UART_SendString 0x3
#define SERV_UART_RecLine 0x4
#define SERV_UART_SendLine 0x5
#define SERV_UART_RecBin 0x6

#define SERV_UART_init_PARA_NUM 0
extern WORD serv_uart_init();

#define SERV_UART_SendByte_PARA_NUM 1
extern WORD serv_uart_SendByte(char ch);

#define SERV_UART_SendString_PARA_NUM 2
extern WORD serv_uart_SendString(const char* str, unsigned int len);

#define SERV_UART_RecLine_PARA_NUM 1
extern WORD serv_uart_RecLine(BYTE* line);

#define SERV_UART_SendLine_PARA_NUM 1
extern WORD serv_uart_SendLine(const char* line);

#define SERV_UART_RecBin_PARA_NUM 2
extern WORD serv_uart_RecBin(WORD app_id, WORD app_len);


// appm 服务部分
#define SERV_APPM_IDX 0x2

#define SERV_APPM_init 0x1
#define SERV_APPM_run 0x2
#define SERV_APPM_register_app 0x3
#define SERV_APPM_show_app 0x4

#define SERV_APPM_init_PARA_NUM 0
extern WORD serv_appm_init();

#define SERV_APPM_run_PARA_NUM 0
extern WORD serv_appm_run();

#define SERV_APPM_register_app_PARA_NUM 3
extern WORD serv_appm_register_app(BYTE* app_name, BYTE* app_binary_base, WORD app_binary_length);

#define SERV_APPM_show_app_PARA_NUM 0
extern WORD serv_appm_show_app();


// power 服务部分
#define SERV_POWER_IDX 0x3

#define SERV_POWER_init 0x1
#define SERV_POWER_sleep 0x2

#define SERV_POWER_init_PARA_NUM 0
extern WORD serv_appm_init();

#define SERV_POWER_sleep_PARA_NUM 0
extern WORD serv_appm_sleep();



#endif

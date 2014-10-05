#ifndef __SERV_INFO_H__
#define __SERV_INFO_H__

// 该服务的代码段起始地址
#define SERV_CODE_SEG_BASE 0x31200000


// 在 SYS 中的进程的几种状态
#define SERV_STATUS_READY 0
#define SERV_STATUS_RUNNING 1
#define SERV_STATUS_UNREADY 2
#define SERV_STATUS_EMPTY 3  // 该索引还没有注册进服务


// 下面即该服务提供的所有函数所对应的操作码
#define SERV_SERVM_INIT 0x1
#define SERV_SERVM_add_serv 0x2
#define SERV_SERVM_register_serv 0x3
#define SERV_SERVM_get_empty_idx 0x4


// 下面即该服务提供的所有函数
// 下面的函数全部出于 serv_lib_funcs.c
extern void serv_servm_init();
extern void serv_servm_add_serv(WORD* para_list, WORD para_num);
extern void serv_servm_register_serv(WORD* para_list, WORD para_num);
extern WORD serv_servm_get_empty_idx();


/* serv 服务的相关参数 */

#define SERV_REGISTER_TABLE_BASE 0x31100000
#define SERV_REGISRER_TABLE_LIMIT 0x00100000

#define SERV_CODE_BASE 0x31200000
#define SERV_CODE_LIMIT 0x00C00000

#define SERV_SHARE_RAM_BASE 0x31E00000
#define SERV_SHARE_RAM_LIMIT 0x00100000


#endif

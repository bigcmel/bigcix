#ifndef __SERV_GLOBAL_H__
#define __SERV_GLOBAL_H__

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


/* sys 服务的相关参数 */

#define SERV_REGISTER_TABLE_BASE 0x31100000
#define SERV_REGISRER_TABLE_LIMIT 0x00100000

#define SERV_CODE_BASE 0x31200000
#define SERV_CODE_LIMIT 0x00C00000

#define SERV_SHARE_RAM_BASE 0x31E00000
#define SERV_SHARE_RAM_LIMIT 0x00100000


WORD CODE_SEG_LIMIT;
WORD PARA_SEG_BASE;

WORD OPT_CODE_BASE;
WORD RETURN_CODE_BASE;


// sys 服务数量的最大值
#define SERV_MAX_NUM 99


// 描述一个在 sys 服务表中的表项
typedef struct serv_table
{
  WORD CODE_SEG_BASE;  // 代码段基地址
  WORD CODE_SEG_LIMIT; // 代码段长度
  WORD PARA_SEG_BASE;  // 共享数据段基地址，不用写共享数据段的长度，因为长度指定为 3 个WORD
  WORD SERV_STATUS;    // serv 当前的状态
}SERV_TABLE, *ptr_serv_table;

// 指向当前要执行的 sys 服务表项
ptr_serv_table PTR_SERV_TABLE;

#define SERV_TABLE_LEN 16 // 一个该结构体占 16 个字节


// 描述一个在共享数据区中的 sys 服务的表项
typedef struct serv_para
{
  WORD OPT_CODE;  // 操作码
  WORD PARA_BASE; // 参数列表的基地址
  WORD PARA_NUM;  // 参数的数量
}SERV_PARA, *ptr_serv_para;

// 指向当前要执行的 sys 服务参数表项
ptr_serv_para PTR_SERV_PARA;

#define SERV_PARA_LEN 12 // 一个该结构体占 12 个字节


// 注册了所有的 sys 服务的全局表
ptr_serv_table SERV_GLOBAL_TABLE;


// 指向当前需要执行的 sys 服务
WORD SERV_IDX;

// 表示未定义的 sys 服务索引，通常表示当前没有 sys 服务需要执行
#define SERV_UND_IDX 100


// 记录最近发生错误的错误码
int SERV_ERR_CODE;

// 各种错误的定义
#define SERV_ERR_UND_IDX 0x1
#define SERV_ERR_UND_OPT 0x2


// sys 服务中操作码的定义，这里的操作码都是规定好的，所有的 sys 服务都必须包含这些操作码
#define SERV_RETURN_OPT 0x0


// 下面函数定义于 serv_global.c
extern void serv_handle_error();

extern WORD serv_get_idx(WORD code_seg_base);



#endif

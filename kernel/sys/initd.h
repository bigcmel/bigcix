#ifndef __INITD_H__
#define __INITD_H__


#include "../include/global.h"
#include "../include/hardware_proto.h"

// initd 中进程的最大数量
#define INITD_APP_NUM 15
#define INITD_FIRST_APP_IDX 0

// initd 进程的索引号，在 initd_setup 中获得
WORD INITD_IDX;


// 示意 INITD 中没有应用程序进程
#define INITD_EMPTY 0

// 表示 INITD 已无法再添加应用程序进程了
#define INITD_FILL 99


// initd 中进程名的最大长度
#define INITD_APP_NAME_LENGTH 20

// 用于描述一个应用程序进程的结构体
typedef struct initd_node
{
  BYTE name[INITD_APP_NAME_LENGTH]; // 进程名
  WORD name_length; // 进程名长度
  WORD idx; // 进程索引值
  WORD status; // 进程所处的状态
  BYTE* binary_base; // 二进制代码在内存中的起始地址
  WORD binary_length; // 二进制代码的长度，以字节为单位
}INITD_NODE, *ptr_initd_node;

// initd 的全局进程表
ptr_initd_node INITD_TABLE;


// initd 中应用程序进程的各种状态
#define INITD_APP_STATUS_READY 0
#define INITD_APP_STATUS_RUNNING 1
#define INITD_APP_STATUS_EMPTY 2
#define INITD_APP_STATUS_FINISHED 3



extern void initd_setup();

extern void initd_run();

extern WORD initd_register_app(BYTE* app_name, BYTE* app_binary_base, WORD app_binary_length);



#endif

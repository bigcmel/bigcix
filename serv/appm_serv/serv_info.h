#ifndef __SERV_INFO_H__
#define __SERV_INFO_H__

// 该服务的代码段起始地址
#define SERV_CODE_SEG_BASE 0x31204000


// appm 中进程的最大数量
#define APPM_APP_NUM 15

// 不算 app_0，因为 app_0 是特殊的进程，是系统申请的用户进程，不是用户申请的
#define APPM_FIRST_APP_IDX 1

// 表示 appm 中当前有多少应用程序进程
int APPM_FILL_APP_NUM;

// 示意 APPM 中没有应用程序进程
#define APPM_EMPTY 0

// 表示 APPM 已无法再添加应用程序进程了
#define APPM_FILL 99

// appm 的令牌，指向当前正在运行的进程
WORD APPM_TOKEN;


// appm 中进程名的最大长度
#define APPM_APP_NAME_LENGTH 20

// 用于描述一个应用程序进程的结构体
typedef struct appm_node
{
  BYTE name[APPM_APP_NAME_LENGTH]; // 进程名
  WORD name_length; // 进程名长度
  WORD idx; // 进程索引值
  WORD status; // 进程所处的状态
  BYTE* binary_base; // 二进制代码在内存中的起始地址
  WORD binary_length; // 二进制代码的长度，以字节为单位
}APPM_NODE, *ptr_appm_node;

// appm 的全局进程表
ptr_appm_node APPM_TABLE;


// appm 中应用程序进程的各种状态
#define APPM_APP_STATUS_READY 0
#define APPM_APP_STATUS_RUNNING 1
#define APPM_APP_STATUS_EMPTY 2
#define APPM_APP_STATUS_FINISHED 3



// 下面即该服务提供的所有函数所对应的操作码
#define SERV_APPM_init 0x1
#define SERV_APPM_run 0x2
#define SERV_APPM_register_app 0x3


// 下面即该服务提供的所有函数
// 下面的函数全部出于 serv_lib_funcs.c
extern void serv_appm_init();
extern void serv_appm_run();
extern void serv_appm_register_app(WORD* para_list, WORD para_num);


// apps table 的在内存中的位置
#define APPM_REGISTER_TABLE_BASE 0x30EFF000
#define APPM_REGISTER_TABLE_LIMIT 0x00001000



#endif

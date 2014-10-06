#ifndef __SERV_INFO_H__
#define __SERV_INFO_H__

// 该服务的代码段起始地址
#define SERV_CODE_SEG_BASE 0x31206000


/* Define register address about power */


/* Values of power register */



// 下面即该服务提供的所有函数所对应的操作码
#define SERV_POWER_INIT 0x1
#define SERV_POWER_SLEEP 0x2


// 下面即该服务提供的所有函数
// 下面的函数全部出于 serv_lib_funcs.c
extern void serv_power_init();
extern void serv_power_sleep();



#endif

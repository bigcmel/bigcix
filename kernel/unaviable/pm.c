#include "pm.h"

// pm 的错误处理函数
static void pm_err_handler();

// 开始运行 pm 中拿到令牌的进程
static void pm_run();

// 得到 sys 进程是否申请 CPU
static int pm_get_sys_is_apply( WORD sys_idx );

// 添加一个进程项到全局进程表中
static void pm_add_to_pm_table(WORD proc_idx, PTRFUNC proc_setup);

// 查看 initd 中是否有应用程序进程
static int pm_initd_is_empty();


// 为操作系统安装 pm，并将其初始化
void pm_setup()
{
  WORD proc_idx;

  static PTRFUNC ptr_initd_setup = initd_setup;
  static PTRFUNC ptr_serv_setup = serv_setup;


  // 生成全局进程表
  pm_add_to_pm_table(PM_INITD_IDX, ptr_initd_setup);
  pm_ptr_initd = &PM_TABLE[PM_INITD_IDX]; // 为全局指针赋值，方便其他会用到该指针的地方

  pm_add_to_pm_table(PM_SERV_IDX, ptr_serv_setup);
  pm_ptr_serv = &PM_TABLE[PM_SERV_IDX];

  
  // 为每一个进程执行他们自己的初始化函数
  for(proc_idx=0 ; proc_idx<PM_PROC_NUM ; proc_idx++)
    {
      (*(PM_TABLE[proc_idx].proc_setup_func))();
      PM_TABLE[proc_idx].status = PM_PROC_STATUS_READY;
    }

}

/* pm 的调度算法
   如 initd 有任务，则将令牌给 initd
   如 sys 进程需要整理，则给该 sys 进程令牌
   如果都不满足，则让系统休眠
 */

void pm_scheduling()
{

  PM_TOKEN = PM_SLEEP;

  if((( pm_ptr_initd->status == PM_PROC_STATUS_READY ) && ( pm_initd_is_empty()!=INITD_EMPTY )) || ( pm_ptr_initd->status == PM_PROC_STATUS_RUNNING ))
    {
      PM_TOKEN = PM_INITD_IDX;
      Uart_SendString("INITD\n",6);

      PM_TABLE[PM_TOKEN].status = PM_PROC_STATUS_RUNNING;
      pm_run();
      PM_TABLE[PM_TOKEN].status = PM_PROC_STATUS_READY;
    }
 
}


static void pm_run()
{
  switch( PM_TOKEN )
    {
    case PM_INITD_IDX:
      initd_run();
      break;
    case PM_SLEEP:
      break;
    default:
      PM_ERR_CODE = PM_ERR_UND_TOKEN;
      pm_err_handler();
      break;
    }
  
}

static void pm_add_to_pm_table(WORD proc_idx, PTRFUNC proc_setup)
{
  ptr_pm_node pm_node_tmp = &PM_TABLE[proc_idx];

  pm_node_tmp->idx = proc_idx;

  pm_node_tmp->proc_setup_func = proc_setup;

  pm_node_tmp->status = PM_PROC_STATUS_UNREADY;
}

static int pm_initd_is_empty()
{
  return INITD_FILL_APP_NUM;
}


static void pm_err_handler()
{}




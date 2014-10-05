#include "../serv_global.h"
#include "serv_info.h"



void serv_choose_opt(WORD opt_code, WORD* para_list, WORD para_num)
{
  switch( opt_code )
    {
    case SERV_APPM_init:
      serv_appm_init();
      break;
    case SERV_APPM_run:
      serv_appm_run();
      break;
    case SERV_APPM_register_app:
      serv_appm_register_app(para_list, para_num);
      break;
    default:
      SERV_ERR_CODE = SERV_ERR_UND_OPT;
      serv_handle_error();
      break;
    }
}



// 该函数定义于 APP_Handler.S 汇编文件中
extern void __APP_S_jmp_to_app( BYTE* binary_base_addr );


static void appm_scheduling();

static void appm_jmp_to_app( WORD app_idx );

static WORD appm_get_empty_idx();

static void appm_clean_finished_app();

static void MMU_SwitchContext( WORD app_idx );


void serv_appm_init()
{
  WORD app_idx;


  //  Uart_SendString("appm setup.\n",12);

  APPM_TABLE = (ptr_appm_node)APPM_REGISTER_TABLE_BASE;
  

/*  下面这些步骤都在内核中的 initd 做过了，所以不需再做一次

  // 初始化 appm 里没有进程
  APPM_FILL_APP_NUM = APPM_EMPTY;

  // 先将所有进程的状态初始化为 未准备好
  for(app_idx=APPM_FIRST_APP_IDX ; app_idx<APPM_APP_NUM ; app_idx++)
    {
      APPM_TABLE[app_idx].status = APPM_APP_STATUS_EMPTY;
    }

*/
}

void serv_appm_run()
{
  if( APPM_FILL_APP_NUM != APPM_EMPTY )
    {
      appm_scheduling();


      APPM_TABLE[APPM_TOKEN].status = APPM_APP_STATUS_RUNNING;


      // 切换上下文，对应到正确的代码段
      MMU_SwitchContext( APPM_TOKEN );
      
      appm_jmp_to_app( APPM_TOKEN );


      APPM_TABLE[APPM_TOKEN].status = APPM_APP_STATUS_FINISHED;
      APPM_FILL_APP_NUM--;

    }
}

// 为全局进程表添加一个新的应用程序
void serv_appm_register_app(WORD* para_list, WORD para_num)
{
  WORD i;
  BYTE char_tmp;
  WORD app_idx;

  BYTE* app_name;
  BYTE* app_binary_base;
  WORD app_binary_length;

  WORD* opt_code_base;
  WORD* return_code_base;  

  //  Uart_SendString("appm register.\n",15);

  app_name = (BYTE*)para_list[0];
  app_binary_base = (BYTE*)para_list[1];
  app_binary_length = para_list[2];

  
  opt_code_base = (WORD*)OPT_CODE_BASE;
  *opt_code_base = SERV_RETURN_OPT;


  i = 0;
  app_idx = appm_get_empty_idx();

  APPM_TABLE[app_idx].idx = app_idx;
  APPM_TABLE[app_idx].status = APPM_APP_STATUS_READY;

  while( (char_tmp=app_name[i]) != 0 )
    {
      APPM_TABLE[app_idx].name[i] = char_tmp;
      i++;
    }
  APPM_TABLE[app_idx].name[i] = 0;
  APPM_TABLE[app_idx].name_length = i;
  
  APPM_TABLE[app_idx].binary_base = app_binary_base;
  APPM_TABLE[app_idx].binary_length = app_binary_length;

  APPM_FILL_APP_NUM++;


  return_code_base = (WORD*)RETURN_CODE_BASE;
  *return_code_base = app_idx;
  

}


// 先不把进程调度搞得太复杂，暂且只是遍历所有的进程，逐一轮询
static void appm_scheduling()
{
  WORD app_idx;
  WORD status_tmp;

  for(app_idx=APPM_FIRST_APP_IDX ; app_idx<APPM_APP_NUM ; app_idx++)
    {
      status_tmp = APPM_TABLE[app_idx].status;

      if( status_tmp == APPM_APP_STATUS_READY )
	{
	  APPM_TOKEN = app_idx;
	  break;
	}
    }

}

// 进入到被调度到的应用程序
static void appm_jmp_to_app( WORD app_idx )
{
  BYTE* binary_base_addr = APPM_TABLE[app_idx].binary_base;
  
  // 调用在 APP_Handler.S 定义的函数，进入到应用程序进程中
  __APP_S_jmp_to_app( binary_base_addr );

}

static WORD appm_get_empty_idx()
{
  WORD app_idx;
  WORD status_tmp;

  for(app_idx=APPM_FIRST_APP_IDX ; app_idx<APPM_APP_NUM ; app_idx++)
    {
      status_tmp = APPM_TABLE[app_idx].status;

      if( status_tmp == APPM_APP_STATUS_EMPTY )
	{
	  return app_idx;
	}
    }

  return APPM_FILL;
}

static void appm_clean_finished_app()
{
  WORD app_idx;
  WORD* status_tmp;

  for(app_idx=APPM_FIRST_APP_IDX ; app_idx<APPM_APP_NUM ; app_idx++)
    {
      status_tmp = &APPM_TABLE[app_idx].status;

      if( *status_tmp == APPM_APP_STATUS_FINISHED )
	{
	  *status_tmp = APPM_APP_STATUS_EMPTY;
	}
    }

}

static void MMU_SwitchContext( WORD app_idx )
{
  // 参考《ARM体系结构与编程》P208
  WORD PID = (app_idx << 25);

  __asm__
    (
     "MOV R0, %0\n"
     "MCR P15, 0, R0, C13, C0, 0\n": /*汇编语句结束*/ : /*没有输出*/
     "r"(PID) // %0 占位符即代表 PID
     );

}

#include "app_global.h"


extern void __syscall_serv_send_para_and_idx(WORD serv_idx, WORD opt_code, WORD para_base, WORD para_num);
extern void __syscall_serv_run(WORD* result);


// servm 部分

WORD serv_servm_init()
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_SERVM_IDX;
  opt_code = SERV_SERVM_init;
  para_num = SERV_SERVM_INIT_PARA_NUM;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}

WORD serv_servm_add_serv(WORD new_serv_idx, WORD code_seg_base, WORD code_seg_limit, WORD para_seg_base, WORD block_base, WORD page_base, WORD block_num, WORD page_num)
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_SERVM_IDX;
  opt_code = SERV_SERVM_add_serv;
  para_num = SERV_SERVM_ADD_SERV_PARA_NUM;

  para_base[0] = new_serv_idx;
  para_base[1] = code_seg_base;
  para_base[2] = code_seg_limit;
  para_base[3] = para_seg_base;
  para_base[4] = block_base;
  para_base[5] = page_base;
  para_base[6] = block_num;
  para_base[7] = page_num;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}

WORD serv_servm_register_serv(WORD code_seg_base, WORD code_seg_limit, WORD para_seg_base)
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_SERVM_IDX;
  opt_code = SERV_SERVM_register_serv;
  para_num = SERV_SERVM_REGISTER_SERV_PARA_NUM;

  para_base[0] = code_seg_base;
  para_base[1] = code_seg_limit;
  para_base[2] = para_seg_base;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}

WORD serv_servm_get_empty_idx()
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_SERVM_IDX;
  opt_code = SERV_SERVM_get_empty_idx;
  para_num = SERV_SERVM_GET_EMPTY_IDX_PARA_NUM;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}


// uart 部分

WORD serv_uart_init()
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_UART_IDX;
  opt_code = SERV_UART_init;
  para_num = SERV_UART_init_PARA_NUM;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}

WORD serv_uart_SendByte(char ch)
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_UART_IDX;
  opt_code = SERV_UART_SendByte;
  para_num = SERV_UART_SendByte_PARA_NUM;

  para_base[0] = (WORD)(&ch);


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}

WORD serv_uart_SendString(const char* str, unsigned int len)
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_UART_IDX;
  opt_code = SERV_UART_SendString;
  para_num = SERV_UART_SendString_PARA_NUM;

  para_base[0] = (WORD)str;
  para_base[1] = (WORD)len;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);


  return result;
}

WORD serv_uart_SendLine(const char* line)
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_UART_IDX;
  opt_code = SERV_UART_SendLine;
  para_num = SERV_UART_SendLine_PARA_NUM;

  para_base[0] = (WORD)line;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);


  return result;
}

WORD serv_uart_RecLine(BYTE* line)
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_UART_IDX;
  opt_code = SERV_UART_RecLine;
  para_num = SERV_UART_RecLine_PARA_NUM;

  para_base[0] = (WORD)line;

  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}

WORD serv_uart_RecBin(WORD app_id, WORD app_len)
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_UART_IDX;
  opt_code = SERV_UART_RecBin;
  para_num = SERV_UART_RecBin_PARA_NUM;

  para_base[0] = app_id;
  para_base[1] = app_len;

  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}


// appm 部分

WORD serv_appm_init()
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_APPM_IDX;
  opt_code = SERV_APPM_init;
  para_num = SERV_APPM_init_PARA_NUM;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}

WORD serv_appm_run()
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_APPM_IDX;
  opt_code = SERV_APPM_run;
  para_num = SERV_APPM_run_PARA_NUM;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);


  /* 
     下面需要回到用户模式，因为这个申请会导致执行一个用户进程，
     当用户进程结束时，会返回到 SVC 模式，但该申请由用户模式
     调用，所以不能维持在 SVC 模式，需要回到用户模式来
  */
  //  return_to_usr_mode();


  return result;
}

WORD serv_appm_register_app(BYTE* app_name, BYTE* app_binary_base, WORD app_binary_length)
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_APPM_IDX;
  opt_code = SERV_APPM_register_app;
  para_num = SERV_APPM_register_app_PARA_NUM;

  para_base[0] = (WORD)app_name;
  para_base[1] = (WORD)app_binary_base;
  para_base[2] = app_binary_length;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}

WORD serv_appm_show_app()
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_APPM_IDX;
  opt_code = SERV_APPM_show_app;
  para_num = SERV_APPM_show_app_PARA_NUM;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}


// power 部分

WORD serv_power_init()
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_POWER_IDX;
  opt_code = SERV_POWER_init;
  para_num = SERV_POWER_init_PARA_NUM;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}

WORD serv_power_sleep()
{
  WORD serv_idx;
  WORD opt_code;
  WORD para_base[SERV_MAX_PARA_NUM];
  WORD para_num;

  WORD result;


  serv_idx = SERV_POWER_IDX;
  opt_code = SERV_POWER_sleep;
  para_num = SERV_POWER_sleep_PARA_NUM;


  __syscall_serv_send_para_and_idx(serv_idx, opt_code, (WORD)para_base, para_num);
  __syscall_serv_run(&result);

  return result;
}

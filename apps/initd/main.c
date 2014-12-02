#include "../app_global.h"
#include "servs_info.h"

extern int Uart_SendString(const char* str, unsigned int len);

extern void shell(void);

void main()
{
  WORD return_code;
 

  //  while(1){};

  //  Uart_SendString("app_0\n",6);

  // 向 servm 申请下列 serv
  serv_servm_init();
  serv_servm_add_serv(SERV_UART_IDX, SERV_UART_CODE_SEG_BASE, SERV_UART_CODE_SEG_LIMIT, SERV_UART_PARA_SEG_BASE, SERV_UART_BLOCK_BASE, SERV_UART_PAGE_BASE, SERV_UART_BLOCK_NUM, SERV_UART_PAGE_NUM);
  serv_servm_add_serv(SERV_APPM_IDX, SERV_APPM_CODE_SEG_BASE, SERV_APPM_CODE_SEG_LIMIT, SERV_APPM_PARA_SEG_BASE, SERV_APPM_BLOCK_BASE, SERV_APPM_PAGE_BASE, SERV_APPM_BLOCK_NUM, SERV_APPM_PAGE_NUM);
  serv_servm_add_serv(SERV_POWER_IDX, SERV_POWER_CODE_SEG_BASE, SERV_POWER_CODE_SEG_LIMIT, SERV_POWER_PARA_SEG_BASE, SERV_POWER_BLOCK_BASE, SERV_POWER_PAGE_BASE, SERV_POWER_BLOCK_NUM, SERV_POWER_PAGE_NUM);

  // uart serv 部分测试
  serv_uart_init();
  return_code = serv_uart_SendString("Enter app_0.\n",13);
  if(return_code == 13)
    serv_uart_SendString("return operation OK.\n",21);
  else
    serv_uart_SendString("return operation NO.\n",21);

  // power serv 部分测试
  serv_power_init();

  // appm serv 部分测试
  serv_appm_init();


  //进入 shell，不断从用户那接受请求
  shell();

  
  // 如果 appm 中注册的 app 都完成，进入 power sleep。
  while(1)
    {
      if(serv_appm_run() == 0x99)
	{
	  serv_uart_SendString("NOW SLEEPING.\n",14);
	  serv_power_sleep();	  
	}
      else
	serv_uart_SendString("app_1 finished.\n",16);
    }

}

#include "../app_global.h"
#include "servs_info.h"

extern int Uart_SendString(const char* str, unsigned int len);

void main()
{
  WORD return_code;

  //  while(1){};

  //  Uart_SendString("app_0\n",6);

  // 向 servm 申请下列 serv
  serv_servm_init();
  serv_servm_add_serv(SERV_UART_IDX, SERV_UART_CODE_SEG_BASE, SERV_UART_CODE_SEG_LIMIT, SERV_UART_PARA_SEG_BASE);
  serv_servm_add_serv(SERV_APPM_IDX, SERV_APPM_CODE_SEG_BASE, SERV_APPM_CODE_SEG_LIMIT, SERV_APPM_PARA_SEG_BASE);
  serv_servm_add_serv(SERV_POWER_IDX, SERV_POWER_CODE_SEG_BASE, SERV_POWER_CODE_SEG_LIMIT, SERV_POWER_PARA_SEG_BASE);

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
  serv_appm_register_app("app_1", 0x0, 4096);
  serv_uart_SendString("setup app_1 in app_0.\n",22);
  
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

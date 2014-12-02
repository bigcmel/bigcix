#include "../app_global.h"

#define MAX_SHELL_LINE 2048
BYTE SHELL_LINE[MAX_SHELL_LINE];

void shell(void)
{
  WORD app_idx;

  /* 注册这么多 app，测试用 
  serv_appm_register_app("app_1", 0x0, 4096);
  serv_appm_register_app("app_2", 0x0, 4096);
  serv_appm_register_app("app_3", 0x0, 4096);
  serv_appm_register_app("app_4", 0x0, 4096);
  serv_appm_register_app("app_5", 0x0, 4096);
  serv_appm_register_app("app_6", 0x0, 4096);
  serv_appm_register_app("app_7", 0x0, 4096);
  serv_appm_register_app("app_8", 0x0, 4096);
  serv_appm_register_app("app_9", 0x0, 4096);
  serv_appm_register_app("app_10", 0x0, 4096);
  serv_appm_register_app("app_11", 0x0, 4096);
  */

  while(1)
    {
      serv_uart_SendString("bigcix >", 8);
      serv_uart_RecLine(SHELL_LINE);

      switch(SHELL_LINE[0])
	{
	case '1':
	  serv_appm_show_app();
	  break;
	case '2':
	  app_idx = serv_appm_register_app("app_1", 0x0, 4096);
	  serv_uart_SendByte('s');       // 给上位机的信号，示意可以开始传送 bin 文件了
	  serv_uart_RecBin(app_idx);

	  serv_uart_SendString("APP ", 4);

	  // 输出 app id号
	  if((app_idx < 10) && (app_idx > 0))
	    serv_uart_SendByte('0' + app_idx);
	  else if((app_idx < 16) && (app_idx > 9))
	    serv_uart_SendByte('A' + app_idx - 10);
	  else            // 没有该 id 号的 app
	    serv_uart_SendByte('N');

	  serv_uart_SendLine(" has been registered.\n");

	  break;
	default:
	  serv_uart_SendLine("Invaild command!\n");	  
	}

    }
}

/*
serv_appm_register_app("app_1", 0x0, 4096);
serv_uart_RecBin(app_id);
serv_uart_RecLine(SHELL_LINE);
serv_uart_SendLine(SHELL_LINE);

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

*/


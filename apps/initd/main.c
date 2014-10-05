#include "../app_global.h"
#include "servs_info.h"

extern int Uart_SendString(const char* str, unsigned int len);

void main()
{
  //  while(1){};

  //  Uart_SendString("app_0\n",6);

  serv_servm_init();
  serv_servm_add_serv(SERV_UART_IDX, SERV_UART_CODE_SEG_BASE, SERV_UART_CODE_SEG_LIMIT, SERV_UART_PARA_SEG_BASE);
  serv_servm_add_serv(SERV_APPM_IDX, SERV_APPM_CODE_SEG_BASE, SERV_APPM_CODE_SEG_LIMIT, SERV_APPM_PARA_SEG_BASE);

  serv_uart_init();
  serv_uart_SendString("Enter app_0.\n",13);

  serv_appm_init();
  serv_appm_register_app("app_1", 0x0, 4096);
  serv_uart_SendString("setup app_1 in app_0.\n",22);
  serv_appm_run();
  serv_uart_SendString("app_1 finished.\n",16);

  //  while(1){};
}

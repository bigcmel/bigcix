extern int Uart_SendString(const char* str, unsigned int len);

void main()
{
  //  while(1){};
 
  //  Uart_SendString("app_1\n",6);

  serv_uart_SendString("Enter app_1.\n",13);

}

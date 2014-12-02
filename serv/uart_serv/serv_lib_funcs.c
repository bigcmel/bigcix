#include "../serv_global.h"
#include "serv_info.h"



void serv_choose_opt(WORD opt_code, WORD* para_list, WORD para_num)
{
  switch( opt_code )
    {
    case SERV_UART_INIT:
      serv_uart_init();
      break;
    case SERV_UART_SendByte:
      serv_uart_SendByte(para_list, para_num);
      break;
    case SERV_UART_SendString:
      serv_uart_SendString(para_list, para_num);
      break;
    case SERV_UART_RecLine:
      serv_uart_RecLine(para_list, para_num);
      break;
    case SERV_UART_SendLine:
      serv_uart_SendLine(para_list, para_num);
      break;
    case SERV_UART_RecBin:
      serv_uart_RecBin(para_list, para_num);
      break;
    default:
      SERV_ERR_CODE = SERV_ERR_UND_OPT;
      serv_handle_error();
      break;
    }
}



void serv_uart_init()
{
  int i;

  ULCON0 = ULCON0_Val;
  UCON0 = UCON0_Val;
  UFCON0 = UFCON0_Val;
  UMCON0 = UMCON0_Val;

  // 波特率算术公式： UBRDIV0 = (int)((int)PCLK/16*baud+0.5)-1;
  UBRDIV0 = 26;

  for(i=0;i<100;i++); 
}


void serv_uart_SendByte(WORD* para_list, WORD para_num)
{
  char* ch_addr;

  ch_addr = (char*)para_list[0];

  while(!(UTRSTAT0 & 0x2));
  UTXH0 = *ch_addr;
}


void serv_uart_SendString(WORD* para_list, WORD para_num)
{
  unsigned int counter;
  char* str;
  unsigned int len;
  char ch;

  WORD* opt_code_base;
  WORD* return_code_base;


  str = (char*)(para_list[0]);
  len = (unsigned int)(para_list[1]);

  for(counter = 0 ; counter < len ; counter++)
    {
      ch = str[counter];

      while(!(UTRSTAT0 & 0x2));
      UTXH0 = ch;
    }


  opt_code_base = (WORD*)OPT_CODE_BASE;
  return_code_base = (WORD*)RETURN_CODE_BASE;

  *opt_code_base = SERV_RETURN_OPT;
  *return_code_base = counter;
}


void serv_uart_SendLine(WORD* para_list, WORD para_num)
{
  char* line;
  int count;

  WORD* opt_code_base;
  WORD* return_code_base;


  line = (char*)para_list[0];
  count = 0;
  
  while(*line != '\n')
    {
      while(!(UTRSTAT0 & 0x2));
      UTXH0 = *line;
      line++;
      count++;
    }

  while(!(UTRSTAT0 & 0x2));
  UTXH0 = *line;

  opt_code_base = (WORD*)OPT_CODE_BASE;
  return_code_base = (WORD*)RETURN_CODE_BASE;

  *opt_code_base = SERV_RETURN_OPT;
  *return_code_base = (WORD)count;
}


void serv_uart_RecLine(WORD* para_list, WORD para_num)
{
  int i;
  char* data;

  WORD* opt_code_base;
  WORD* return_code_base;


  data = (char*)para_list[0];

  i = 0;
  do
    {
      while((!(UTRSTAT0 & 0x1)));
      data[i] = URXH0;
    }while(data[i++] != '\n');


  opt_code_base = (WORD*)OPT_CODE_BASE;
  return_code_base = (WORD*)RETURN_CODE_BASE;

  *opt_code_base = SERV_RETURN_OPT;
  *return_code_base = (WORD)i;
}


#define APP_ROM_SIZE 0x02000000
#define BIN_FINAL_CHAR '\n'
void serv_uart_RecBin(WORD* para_list, WORD para_num)
{
  int i;
  WORD app_id;
  BYTE* data;

  WORD* opt_code_base;
  WORD* return_code_base;


  app_id = para_list[0];

  data = (BYTE*)(app_id * APP_ROM_SIZE);

  i = 0;
  do
    {
      while((!(UTRSTAT0 & 0x1)));
      *data = URXH0;
      i++;
    }while((*data != BIN_FINAL_CHAR) && (data++));


  opt_code_base = (WORD*)OPT_CODE_BASE;
  return_code_base = (WORD*)RETURN_CODE_BASE;

  *opt_code_base = SERV_RETURN_OPT;
  *return_code_base = (WORD)i;
}



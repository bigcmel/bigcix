#include "../include/global.h"


void C_SWI_Handler( WORD number, WORD* reg )
{
  WORD param_0 = reg[0];
  WORD param_1 = reg[1];
  WORD param_2 = reg[2];
  WORD param_3 = reg[3];

  WORD* result;
  

  switch( number )
    {
    case 0: // 进入特权模式，已进入，所以这里什么都不做
      break;
    case 1: // 送出参数到共享内存，用以提供给 serv
      serv_send_para_and_idx(param_0, param_1, param_2, param_3);
      break;
    case 2: // 运行 serv 
      result = (WORD*)param_0;
      (*result) = serv_run();
      break;
    default: // 无效的 SWI 号时执行的代码
      break;
    }
}

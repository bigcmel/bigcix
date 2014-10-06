#include "../serv_global.h"
#include "serv_info.h"



void serv_choose_opt(WORD opt_code, WORD* para_list, WORD para_num)
{
  switch( opt_code )
    {
    case SERV_POWER_INIT:
      serv_power_init();
      break;
    case SERV_POWER_SLEEP:
      serv_power_sleep();
      break;
    default:
      SERV_ERR_CODE = SERV_ERR_UND_OPT;
      serv_handle_error();
      break;
    }
}



void serv_power_init()
{

}


void serv_power_sleep()
{
  while(1){};
}


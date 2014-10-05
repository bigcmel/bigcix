#include "serv_global.h"


void serv_handle_error()
{}


WORD serv_get_idx(WORD code_seg_base)
{
  WORD serv_idx;
  ptr_serv_table ptr_serv_table_tmp;

  
  ptr_serv_table_tmp = (ptr_serv_table)SERV_REGISTER_TABLE_BASE;

  //  这里还要对全局变量 CODE_SEG_LIMIT 和 PARA_SEG_BASE 赋值
  for(serv_idx = 0; serv_idx < SERV_MAX_NUM; serv_idx ++)
    {
      if((ptr_serv_table_tmp -> CODE_SEG_BASE) == code_seg_base)
	{
	  CODE_SEG_LIMIT = ptr_serv_table_tmp -> CODE_SEG_LIMIT;
	  PARA_SEG_BASE = ptr_serv_table_tmp -> PARA_SEG_BASE;
	  break;
	}
      
      ptr_serv_table_tmp ++;
    }


  // 在获得了全局变量 PARA_SEG_BASE 后，就可以得到要存储操作码的地址 OPT_CODE_BASE 和要存储返回值的地址 RETURN_CODE_BASE
  OPT_CODE_BASE = PARA_SEG_BASE;
  RETURN_CODE_BASE = PARA_SEG_BASE + 4;

  return serv_idx;
}

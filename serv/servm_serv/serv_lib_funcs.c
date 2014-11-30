#include "../serv_global.h"
#include "serv_info.h"



void serv_choose_opt(WORD opt_code, WORD* para_list, WORD para_num)
{
  switch( opt_code )
    {
    case SERV_SERVM_INIT:
      serv_servm_init();
      break;
    case SERV_SERVM_add_serv:
      serv_servm_add_serv(para_list, para_num);
      break;
    case SERV_SERVM_register_serv:
      serv_servm_register_serv(para_list, para_num);
      break;
    case SERV_SERVM_get_empty_idx:
      serv_servm_get_empty_idx();
    default:
      SERV_ERR_CODE = SERV_ERR_UND_OPT;
      serv_handle_error();
      break;
    }
}



void serv_servm_init()
{
  SERV_GLOBAL_TABLE = (ptr_serv_table)SERV_REGISTER_TABLE_BASE;

}


void serv_servm_add_serv(WORD* para_list, WORD para_num)
{ 
  WORD serv_idx;
  WORD code_seg_base;
  WORD code_seg_limit;
  WORD para_seg_base;
  WORD block_base;
  WORD page_base;
  WORD block_num;
  WORD page_num;

  BYTE* ptr;
  int i, j;

  WORD nf_blocknum, nf_pagepblock, nf_mainsize, nf_sparesize;
  
  serv_idx = para_list[0];
  code_seg_base = para_list[1];
  code_seg_limit = para_list[2];
  para_seg_base = para_list[3];
  block_base = para_list[4];
  page_base = para_list[5];
  block_num = para_list[6];
  page_num = para_list[7];

 
  NF_GetBlockPageInfo(&nf_blocknum, &nf_pagepblock, &nf_mainsize, &nf_sparesize);

  ptr = (BYTE*)code_seg_base;

  for(j=0 ; j<block_num ; j++)
    {
      for(i=0 ; i<page_num ; i++)
	{
      
	  if( NF_ReadPage(block_base+j, page_base+i, ptr) )
	    ptr += nf_mainsize;
	  //	  else
	    //	    Uart_SendString("Read Fail!\n",11);
	}
    }

  // 得到索引指向的 sys 表项
  PTR_SERV_TABLE = (ptr_serv_table)(SERV_REGISTER_TABLE_BASE + serv_idx * SERV_TABLE_LEN);

  PTR_SERV_TABLE -> CODE_SEG_BASE = code_seg_base;
  PTR_SERV_TABLE -> CODE_SEG_LIMIT = code_seg_limit;
  PTR_SERV_TABLE -> PARA_SEG_BASE = para_seg_base;

  SERV_GLOBAL_TABLE[serv_idx].SERV_STATUS = SERV_STATUS_READY;

}


void serv_servm_register_serv(WORD* para_list, WORD para_num)
{
  WORD serv_idx;
  WORD code_seg_base;
  WORD code_seg_limit;
  WORD para_seg_base;
  
  code_seg_base = para_list[0];
  code_seg_limit = para_list[1];
  para_seg_base = para_list[2];

  serv_idx = serv_servm_get_empty_idx();

  // 得到索引指向的 sys 表项
  PTR_SERV_TABLE = (ptr_serv_table)(SERV_REGISTER_TABLE_BASE + serv_idx * SERV_TABLE_LEN);

  PTR_SERV_TABLE -> CODE_SEG_BASE = code_seg_base;
  PTR_SERV_TABLE -> CODE_SEG_LIMIT = code_seg_limit;
  PTR_SERV_TABLE -> PARA_SEG_BASE = para_seg_base;

  SERV_GLOBAL_TABLE[serv_idx].SERV_STATUS = SERV_STATUS_READY;

}


WORD serv_servm_get_empty_idx()
{
  WORD serv_idx;

  WORD* opt_code_base;
  WORD* return_code_base;

  
  for(serv_idx=0; serv_idx<SERV_MAX_NUM; serv_idx++)
    if( SERV_GLOBAL_TABLE[serv_idx].SERV_STATUS == SERV_STATUS_EMPTY )
      break;


  opt_code_base = (WORD*)OPT_CODE_BASE;
  return_code_base = (WORD*)RETURN_CODE_BASE;

  *opt_code_base = SERV_RETURN_OPT;
  *return_code_base = serv_idx;

  return serv_idx;
}


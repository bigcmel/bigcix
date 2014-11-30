#include "serv.h"


static WORD serv_get_empty_idx();

static void add_serv(WORD serv_idx, WORD code_seg_base, WORD code_seg_limit, WORD para_seg_base);

static void add_serv_load_from_nand(WORD serv_idx, WORD code_seg_base, WORD code_seg_limit, WORD para_seg_base, WORD block_base, WORD page_base, WORD block_num, WORD page_num);

static WORD register_serv(WORD code_seg_base, WORD code_seg_limit, WORD para_seg_base);

static WORD serv_recv_result();


/* 初始化 sys 这项服务 */

void serv_setup()
{
  int i;


  Uart_SendString("servm setup.\n", 13);


  SERV_GLOBAL_TABLE = (ptr_serv_table)SERV_REGISTER_TABLE_BASE;

  // 初始化 sys 服务的全局表
  for(i=0; i<SERV_MAX_NUM; i++)
      SERV_GLOBAL_TABLE[i].SERV_STATUS = SERV_STATUS_EMPTY;


  // 只添加这唯一的一个 serv，即 servm
  add_serv_load_from_nand(SERV_SERVM_IDX, SERV_SERVM_CODE_SEG_BASE, SERV_SERVM_CODE_SEG_LIMIT, SERV_SERVM_PARA_SEG_BASE, SERV_SERVM_BLOCK_BASE, SERV_SERVM_PAGE_BASE, SERV_SERVM_BLOCK_NUM, SERV_SERVM_PAGE_NUM);

  Uart_SendString("servm setup end.\n", 17);

  SERV_IDX = SERV_UND_IDX;
}


/* 运行 serv  */

WORD serv_run()
{
  WORD serv_code_seg_base;
  WORD result;

  if( SERV_IDX == SERV_UND_IDX )
    return;


  SERV_GLOBAL_TABLE[SERV_IDX].SERV_STATUS = SERV_STATUS_RUNNING;

  // 利用 SERV_IDX 参数来确定要执行哪个 sys 服务
  PTR_SERV_TABLE = (ptr_serv_table)(SERV_REGISTER_TABLE_BASE + SERV_IDX * SERV_TABLE_LEN);
  serv_code_seg_base = PTR_SERV_TABLE -> CODE_SEG_BASE;
  
  // 下面这个函数定义在 SERV_Handle.S 中
  __SERV_S_jmp_to_serv(serv_code_seg_base);

  result = serv_recv_result();

  // 执行完毕，为下面的参数复值
  SERV_GLOBAL_TABLE[SERV_IDX].SERV_STATUS = SERV_STATUS_READY;
  SERV_IDX = SERV_UND_IDX;


  return result;
}


void serv_send_para(WORD opt_code, WORD para_base, WORD para_num)
{
  // 利用 SERV_IDX 参数来确定要执行哪个 sys 服务
  PTR_SERV_TABLE = (ptr_serv_table)(SERV_REGISTER_TABLE_BASE + SERV_IDX * SERV_TABLE_LEN);
  PTR_SERV_PARA = (ptr_serv_para)(PTR_SERV_TABLE -> PARA_SEG_BASE);

  PTR_SERV_PARA -> OPT_CODE = opt_code;
  PTR_SERV_PARA -> PARA_BASE = para_base;
  PTR_SERV_PARA -> PARA_NUM =para_num;
}


void serv_send_para_and_idx(WORD serv_idx, WORD opt_code, WORD para_base, WORD para_num)
{
  SERV_IDX = serv_idx;

  serv_send_para(opt_code, para_base, para_num);
}


static WORD serv_get_empty_idx()
{
  WORD serv_idx;
  
  for(serv_idx=0; serv_idx<SERV_MAX_NUM; serv_idx++)
    if( SERV_GLOBAL_TABLE[serv_idx].SERV_STATUS == SERV_STATUS_EMPTY )
      break;

  return serv_idx;
}

static void add_serv(WORD serv_idx, WORD code_seg_base, WORD code_seg_limit, WORD para_seg_base)
{
  // 得到索引指向的 sys 表项
  PTR_SERV_TABLE = (ptr_serv_table)(SERV_REGISTER_TABLE_BASE + serv_idx * SERV_TABLE_LEN);

  PTR_SERV_TABLE -> CODE_SEG_BASE = code_seg_base;
  PTR_SERV_TABLE -> CODE_SEG_LIMIT = code_seg_limit;
  PTR_SERV_TABLE -> PARA_SEG_BASE = para_seg_base;

  SERV_GLOBAL_TABLE[serv_idx].SERV_STATUS = SERV_STATUS_READY;

}

static void add_serv_load_from_nand(WORD serv_idx, WORD code_seg_base, WORD code_seg_limit, WORD para_seg_base, WORD block_base, WORD page_base, WORD block_num, WORD page_num)
{
  BYTE* ptr;
  int i, j;

  WORD nf_blocknum, nf_pagepblock, nf_mainsize, nf_sparesize;

  // 将 nand flash 上的内容读入内存

  NF_GetBlockPageInfo(&nf_blocknum, &nf_pagepblock, &nf_mainsize, &nf_sparesize);

  ptr = (BYTE*)code_seg_base;

  for(j=0 ; j<block_num ; j++)
    {
      for(i=0 ; i<page_num ; i++)
	{
      
	  if( NF_ReadPage(block_base+j, page_base+i, ptr) )
	    ptr += nf_mainsize;
	  else
	    Uart_SendString("Read Fail!\n",11);
	}
    }

  add_serv(serv_idx, code_seg_base, code_seg_limit, para_seg_base);
}

static WORD register_serv(WORD code_seg_base, WORD code_seg_limit, WORD para_seg_base)
{
  WORD serv_idx;

  serv_idx = serv_get_empty_idx();

  add_serv(serv_idx, code_seg_base, code_seg_limit, para_seg_base);

  return serv_idx;
}

static WORD serv_recv_result()
{
  WORD opt_code;
  WORD result;

  // 利用 SERV_IDX 参数来确定要执行哪个 sys 服务
  PTR_SERV_TABLE = (ptr_serv_table)(SERV_REGISTER_TABLE_BASE + SERV_IDX * SERV_TABLE_LEN);
  PTR_SERV_PARA = (ptr_serv_para)(PTR_SERV_TABLE -> PARA_SEG_BASE);

  opt_code = PTR_SERV_PARA -> OPT_CODE;

  if(opt_code == SERV_RETURN_OPT)
    {
      result = PTR_SERV_PARA -> PARA_BASE;
      return result;
    }

  return NULL;
}

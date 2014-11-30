#include "initd.h"


// 该函数定义于 APP_Handler.S 汇编文件中
extern void __APP_S_jmp_to_app( BYTE* binary_base_addr );


static void initd_jmp_to_app( WORD app_idx );

static WORD initd_get_empty_idx();

static void initd_clean_finished_app();

static WORD load_user_app_to_initd( BYTE* app_name, BYTE* app_binary_base, WORD app_binary_length );

static void load_initd_from_nand(WORD block_base, WORD page_base, WORD block_num, WORD page_num);


void initd_setup()
{
  short int app_idx;

  WORD binary_base;
  WORD binary_length;


  Uart_SendString("initd setup.\n", 13);

  INITD_TABLE = (ptr_initd_node)INITD_REGISTER_TABLE_BASE;


  // 先将所有进程的状态初始化为 未准备好
  for(app_idx=INITD_FIRST_APP_IDX ; app_idx<INITD_APP_NUM ; app_idx++)
    {
      INITD_TABLE[app_idx].status = INITD_APP_STATUS_EMPTY;
    }


  binary_base = (WORD)INITD_CODE_SEG_BASE;
  binary_length = 2;  // 单位为 M

  // 加载初始化用户进程 initd
  INITD_IDX = load_user_app_to_initd( "initd", (BYTE*)binary_base, binary_length );
  load_initd_from_nand(INITD_BLOCK_BASE, INITD_PAGE_BASE, INITD_BLOCK_NUM, INITD_PAGE_NUM);
  Uart_SendString("initd setup end.\n", 17);  
}

void initd_run()
{
  WORD app_idx;
  WORD status_tmp;


  app_idx = INITD_IDX;
  status_tmp = INITD_TABLE[app_idx].status;

  if( status_tmp == INITD_APP_STATUS_READY )
    {
      INITD_TABLE[app_idx].status = INITD_APP_STATUS_RUNNING;

      // 切换上下文，对应到正确的代码段
      MMU_SwitchContext( app_idx );
      
      initd_jmp_to_app( app_idx );

      // initd 执行结束
      INITD_TABLE[app_idx].status = INITD_APP_STATUS_FINISHED;
    }
  else
    Uart_SendString("initd is unready.\n", 18);

}

// 为全局进程表添加一个新的应用程序
WORD initd_register_app(BYTE* app_name,
				BYTE* app_binary_base,
				WORD app_binary_length)
{
  WORD i;
  BYTE char_tmp;
  WORD app_idx;
  
  i = 0;
  app_idx = initd_get_empty_idx();

  INITD_TABLE[app_idx].idx = app_idx;

  while( (char_tmp=app_name[i]) != 0 )
    {
      INITD_TABLE[app_idx].name[i] = char_tmp;
      i++;
    }
  INITD_TABLE[app_idx].name[i] = 0;

  INITD_TABLE[app_idx].name_length = i;
  
  INITD_TABLE[app_idx].binary_base = app_binary_base;

  INITD_TABLE[app_idx].binary_length = app_binary_length;


  INITD_TABLE[app_idx].status = INITD_APP_STATUS_READY;


  return app_idx;
}


// 进入到被调度到的应用程序
static void initd_jmp_to_app( WORD app_idx )
{
  BYTE* binary_base_addr = INITD_TABLE[app_idx].binary_base;
  
  // 调用在 APP_Handler.S 定义的函数，进入到应用程序进程中
  __APP_S_jmp_to_app( binary_base_addr );

}

static WORD initd_get_empty_idx()
{
  WORD app_idx;
  int status_tmp;

  for(app_idx=INITD_FIRST_APP_IDX ; app_idx<INITD_APP_NUM ; app_idx++)
    {
      status_tmp = INITD_TABLE[app_idx].status;

      if( status_tmp == INITD_APP_STATUS_EMPTY )
	{
	  return app_idx;
	}
    }

  return INITD_FILL;
}

static void initd_clean_finished_app()
{
  WORD app_idx;
  WORD* status_tmp;

  for(app_idx=INITD_FIRST_APP_IDX ; app_idx<INITD_APP_NUM ; app_idx++)
    {
      status_tmp = &INITD_TABLE[app_idx].status;

      if( *status_tmp == INITD_APP_STATUS_FINISHED )
	{
	  *status_tmp = INITD_APP_STATUS_EMPTY;
	}
    }

}

static WORD load_user_app_to_initd( BYTE* app_name, BYTE* app_binary_base, WORD app_binary_length )
{
  WORD status; // 函数执行情况
  WORD i;

  WORD app_idx;

  WORD block;
  WORD page;
  WORD page_num;
  BYTE* buffer;
  
  WORD nf_blocknum;
  WORD nf_pagepblock;
  WORD nf_mainsize;
  WORD nf_sparesize;


  /* 先注册 initd 的进程表 */

  app_binary_base = 0x00000000;
  app_binary_length = 4096;

  app_idx = initd_register_app(app_name, app_binary_base, app_binary_length);


  /* 这里要切换一下上下文，切换到 app_idx 对应的代码断去 */
  
  MMU_SwitchContext( app_idx );

  
  /* 向 sys_mm 申请需要的内存空间 

  if( ! sys_mm_apply_sdram(app_idx, (WORD)app_binary_base, app_binary_length) )
    return RETURN_FAILED;
  */


  /* 调用系统调用（ SWI 中断），将代码从 SDRAM 读到内存中 
  
  NF_GetBlockPageInfo(&nf_blocknum, &nf_pagepblock, &nf_mainsize, &nf_sparesize);

  block = 5;
  page = 0;
  page_num = ((INITD_TABLE[app_idx].binary_length - 1) / nf_mainsize) + 1;
  buffer = INITD_TABLE[app_idx].binary_base;

  for(i=0; i<page_num; i++)
    {
      status = NF_WritePage( block, page, buffer );
      page++;
      buffer += nf_mainsize;
      
      if(page == 32)
	{
	  block++;
	  page = 0;
	}
    }
  */
  return app_idx;
}

void load_initd_from_nand(WORD block_base, WORD page_base, WORD block_num, WORD page_num)
{
  BYTE* ptr;
  int i, j;

  WORD nf_blocknum, nf_pagepblock, nf_mainsize, nf_sparesize;

  // 将 nand flash 上的内容读入内存

  NF_GetBlockPageInfo(&nf_blocknum, &nf_pagepblock, &nf_mainsize, &nf_sparesize);

  ptr = (BYTE*)INITD_CODE_SEG_BASE;

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
}

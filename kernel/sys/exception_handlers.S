/*
  该文件包含了除 Reset_Handler 以外的所有异常处理函数，
  Reset_Handler 在 startup.S 中定义了
*/
	.include	"include/global.inc"

	.TEXT
	.ARM

	.GLOBL	Undef_Handler
	.GLOBL	SWI_Handler
	.GLOBL	PAbt_Handler
	.GLOBL	DAbt_Handler
	.GLOBL	IRQ_Handler
	.GLOBL	FIQ_Handler

	
/* Function: 未定义的指令 */

Undef_Handler:
	B	.


/* Function: 软件中断 */

SWI_Handler:

/*
	MOV	R1,	#Mode_SYS
	ORR	R1,	#I_BIT
	ORR	R1,	#F_BIT
        MSR     CPSR_c, R1
*/	
	.EXTERN	C_SWI_Handler	@ 定义于 SWI_Handler 中

	STMFD	SP!,	{R0-R12,LR}

	LDR	R0,	[LR,#-4]
	BIC	R0, R0, #0xFF000000
	
	@ 第二级处理程序开始，即跳入到定义于 SWI_Handler.c 内的 C_SWI_handler 函数中

	MOV	R1,	SP	@ 将栈指针作为第二参数传递给C程序类型的第2级中断处理程序
	BL	C_SWI_Handler
	
	@ 第二级处理程序结束

	LDMFD	SP!,	{R0-R12,PC}


/* Function: 指令预取中止 */

PAbt_Handler:
	B	.


/* Function: 数据访问中止 */

DAbt_Handler:
	B	.


/* Function: 外部中断请求 */

IRQ_Handler:
	B	.


/* Function: 快速中断请求 */

FIQ_Handler:
	B	.
	

	.END

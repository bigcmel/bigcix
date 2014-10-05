/*****************************************************************************/
/* S3C2440.S: Startup file kernel for Samsung S3C2440                        */
/*****************************************************************************/


/* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs     */

	.include	"include/global.inc"

	
/*----------------------- Stack and Heap Definitions -------------------------*/

	.EQU	UND_Stack_Size,	0x00010000	@ 64K
	.EQU	SVC_Stack_Size,	0x00080000	@ 特权模式栈的大小为 512K
	.EQU	ABT_Stack_Size,	0x00010000	@ 64K
	.EQU	FIQ_Stack_Size,	0x00020000	@ 128K
	.EQU	IRQ_Stack_Size,	0x00040000	@ 256K
	.EQU	USR_Stack_Size,	0x00200000	@ 用户栈大小为 2M

	.EQU	SYS_Stack_Base, 0x32000000	@ 系统空间栈的基地址
	.EQU	USR_Stack_Base, 0x1E200000	@ 用户空间栈的基地址
	

/* 	Heap Configuration	*/

	.EQU	Heap_Size,	0x00000000

	.BSS
	.ALIGN	3

__heap_base:
Heap_Mem:
	.SPACE   Heap_Size
__heap_limit:	


/*----------------------- Memory Definitions ---------------------------------*/

	.EQU	VECTORS_TABLE_BASE,	0xFFFF0000	@ 高端异常中断向量表的地址
	.EQU	KERNEL_VIR_BASE,	0x30004000	@ kernel.bin 的基地址


/*----------------------- CODE -----------------------------------------------*/

/* Area Definition and Entry Point
@  Startup Code must be linked first at Address at which it expects to run.*/

	.TEXT
	.ARM

	.GLOBL	_start		@ default entry point
_start:		
	
copy_vectors:	
@ Copy Exception Vectors to Internal RAM ---------------------------------------

                ADR     R8,  Vectors    @ Source
                LDR     R9,  =VECTORS_TABLE_BASE	@ Destination
                LDMIA   R8!, {R0-R7}    @ Load Vectors 
                STMIA   R9!, {R0-R7}    @ Store Vectors 
                LDMIA   R8!, {R0-R7}    @ Load Handler Addresses 
                STMIA   R9!, {R0-R7}    @ Store Handler Addresses


set_stack:	
@ Setup Stack for each mode ----------------------------------------------------

                LDR     R0, =SYS_Stack_Base

@  Enter Supervisor Mode and set its Stack Pointer
		MOV	R1,	#Mode_SVC
		ORR	R1,	#I_BIT
		ORR	R1,	#F_BIT
                MSR     CPSR_c, R1
                MOV     SP, R0
                SUB     R0, R0, #SVC_Stack_Size

@  Enter IRQ Mode and set its Stack Pointer
		MOV	R1,	#Mode_IRQ
		ORR	R1,	#I_BIT
		ORR	R1,	#F_BIT
                MSR     CPSR_c, R1
                MOV     SP, R0
                SUB     R0, R0, #IRQ_Stack_Size

@  Enter FIQ Mode and set its Stack Pointer
                MOV	R1,	#Mode_FIQ
		ORR	R1,	#I_BIT
		ORR	R1,	#F_BIT
                MSR     CPSR_c, R1
                MOV     SP, R0
                SUB     R0, R0, #FIQ_Stack_Size
	
@  Enter Undefined Instruction Mode and set its Stack Pointer
		MOV	R1,	#Mode_UND
		ORR	R1,	#I_BIT
		ORR	R1,	#F_BIT
                MSR     CPSR_c, R1
                MOV     SP, R0
                SUB     R0, R0, #UND_Stack_Size

@  Enter Abort Mode and set its Stack Pointer
                MOV	R1,	#Mode_ABT
		ORR	R1,	#I_BIT
		ORR	R1,	#F_BIT
                MSR     CPSR_c, R1
                MOV     SP, R0
                SUB     R0, R0, #ABT_Stack_Size


		LDR     R0, =USR_Stack_Base
	
@  Enter User Mode and set its Stack Pointer	
                MSR     CPSR_c, #Mode_USR
                MOV     SP, R0
                SUB     SL, SP, #USR_Stack_Size

@ 由于刚才进入了用户模式，所以这里要通过 SWI 中断的方式回到特权模式

	SWI	0x0

	
@ Enter the C code -------------------------------------------------------------

	.EXTERN __main
                BL	__main	@ __main函数将返回 kernel.bin 的加载地址到 R0

		B	.

	
/* Exception Vectors
@  Mapped to Address 0.
@  Absolute addressing mode must be used.
@  Dummy Handlers are implemented as infinite loops which can be modified.*/

Vectors:
	LDR     PC, Reset_Addr
	LDR     PC, Undef_Addr
	LDR     PC, SWI_Addr
	LDR     PC, PAbt_Addr
	LDR     PC, DAbt_Addr
	NOP
	LDR     PC, IRQ_Addr
	LDR     PC, FIQ_Addr

@ 下面的函数都出自 sys/execption_handles.S
	
	.EXTERN  Undef_Handler
	.EXTERN  SWI_Handler
	.EXTERN  PAbt_Handler
	.EXTERN  DAbt_Handler
	.EXTERN  IRQ_Handler
	.EXTERN  FIQ_Handler	


Reset_Addr:
	.WORD     Reset_Handler
Undef_Addr:
	.WORD     Undef_Handler
SWI_Addr:
	.WORD     SWI_Handler
PAbt_Addr:
	.WORD     PAbt_Handler
DAbt_Addr:
	.WORD     DAbt_Handler
	.WORD     0                   @ Reserved Address 
IRQ_Addr:
	.WORD     IRQ_Handler
FIQ_Addr:
	.WORD     FIQ_Handler


/* Reset Handler	*/
	.GLOBL  Reset_Handler
Reset_Handler:	

	LDR	PC, =KERNEL_VIR_BASE

/*	
@ Get User Initial Stack & Heap Address
	.TEXT
	.ARM

	.GLOBL  __user_initial_stackheap
__user_initial_stackheap:	

                LDR     R0, = Heap_Mem
                LDR     R1, = (Stack_Mem + USR_Stack_Size)
                LDR     R2, = (Heap_Mem + Heap_Size)
                LDR    	R3, = Stack_Mem
                BX	LR
*/

	
	.END

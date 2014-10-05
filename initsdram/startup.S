/*****************************************************************************/
/* S3C2440.S: Startup file for boot.bin of Samsung S3C2440                   */
/*****************************************************************************/

/* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs     */

	.EQU	Mode_USR,	0x10
	.EQU	Mode_FIQ,	0x11
	.EQU	Mode_IRQ,	0x12
	.EQU	Mode_SVE,	0x13
	.EQU	Mode_ABT,	0x17
	.EQU	Mode_UND,	0x1B
	.EQU	Mode_SYS,	0x1F

	.EQU	I_BIT,	0x80	@ when I bit is set, IRQ is disabled
	.EQU	F_BIT,	0x40	@ when F bit is set, FIQ is disabled


/*----------------------- Stack Definitions ---------------------------------*/

	.EQU	SVE_Stack_Size,	0x00000008

	.BSS
	.ALIGN	3

Stack_Mem:
	.SPACE   SVE_Stack_Size
Stack_Top:


/*----------------------- CODE -----------------------------------------------*/

/* Area Definition and Entry Point
@  Startup Code must be linked first at Address at which it expects to run.*/

	.TEXT
	.ARM

	.GLOBL	_start		@ default entry point
_start:	
	
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

	
Undef_Handler:
	B       Undef_Handler
SWI_Handler:
	B       SWI_Handler
PAbt_Handler:
	B       PAbt_Handler
DAbt_Handler:
	B       DAbt_Handler
IRQ_Handler:
	B       .
FIQ_Handler:
	B       FIQ_Handler


/* Reset Handler	*/

Reset_Handler:	
	
@ Setup Stack for each mode ----------------------------------------------------

                LDR     R0, =Stack_Top

@  Enter Supervisor Mode and set its Stack Pointer
		MOV	R1,	#Mode_SVE
		ORR	R1,	#I_BIT
		ORR	R1,	#F_BIT
                MSR     CPSR_c, R1
                MOV     SP, R0
                SUB     R0, R0, #SVE_Stack_Size


@ Enter the C code -------------------------------------------------------------

	.EXTERN __main
                LDR     R0, =__main
                BLX	R0	@ __main函数将返回 loader.bin 的加载地址到 R0
	
		B	.


	.END

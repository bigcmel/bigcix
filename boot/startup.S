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
		BL	set_svc
		BL	disable_watchdog
		BL	disable_interrupt
		BL	disable_mmu
		BL	init_clock
		BL	init_sdram
		BL	init_stack

@ Enter C
	.EXTERN	__main
		BL	__main

		MOV	PC,	R0 	@ 跳近 loader

		B	.

set_svc:
		MRS	R0, 	CPSR
		BIC	R0, 	r0, 	#0x1f
		ORR	R0, 	R0, 	#0xd3
		MSR	CPSR, 	R0
		MOV	PC, 	LR
	
disable_watchdog:
	.EQU	pWTCON,	 	0x53000000
	
		LDR	R0, 	=pWTCON
		MOV	R1, 	#0x0
		STR	R1, 	[R0]
		MOV	PC, 	LR

disable_interrupt:
		MVN	R1,	#0x0
		LDR	R0,	=0x4a000008
		STR	R1,	[R0]
		MOV	PC,	LR

disable_mmu:
		MCR	P15, 0, R0, C7, C7, 0
		MRC	P15, 0, R0, C1, C0, 0
		BIC	R0, R0, #0x0000007
		MCR	P15, 0, R0, C1, C0, 0
		MOV	PC,	LR

init_clock:
	.EQU	MPLLCON,	0x4c000004 	@ p168
	.EQU	UPLLCON,	0x4c000008 	@ p168
	.EQU	CLKDIVN,	0x4c000014 	@ p172
	.EQU	MPLL_405MHZ,	((127 << 12) | (2 << 4) | (1 << 0)) 	@ p169
	.EQU	UPLL_48MHZ,	((56 << 12) | (2 << 4) | (2 << 0)) 	@ p169

		LDR	R0,	=UPLLCON
		LDR	R1,	=UPLL_48MHZ
		STR	R1,	[R0]
	
		LDR	R0,	=MPLLCON
		LDR	R1,	=MPLL_405MHZ
		STR	R1,	[R0]

		LDR	R0,	=CLKDIVN
		MOV	R1,	#0x5 		@ FCLK:HCLK:PCLK = 8:2:1
		STR	R1,	[R0]

		MRC	P15, 0, R0, C1, C0, 0		@ p159
		ORR	R0, R0, #0xc0000000
		MCR	P15, 0, R0, C1, C0, 0
		MOV	PC,	LR

init_sdram:
	.EQU	mem_contrl,	0x48000000

		LDR	R0,	=mem_contrl
		ADD	R3,	R0,	#4*13
		ADRL	R1,	mem_data
	
tag0:
		LDR	R2,	[R1],	#4
		STR	R2,	[R0],	#4
		CMP	R0,	R3
		BNE	tag0
		MOV	PC,	LR

mem_data:
	.LONG	0x22000000
	.LONG	0x00000700
	.LONG	0x00000700
	.LONG	0x00000700
	.LONG	0x00000700
	.LONG	0x00000700
	.LONG	0x00000700
	.LONG	0x00018001
	.LONG	0x00018001
	.LONG	0x008c04f5
	.LONG	0x000000b1
	.LONG	0x00000030
	.LONG	0x00000030
	
init_stack:
		LDR	SP,	=0x32000000
		MOV	PC,	LR
	
	
	.END

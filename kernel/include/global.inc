/* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs     */

	.EQU	Mode_USR,	0x10
	.EQU	Mode_FIQ,	0x11
	.EQU	Mode_IRQ,	0x12
	.EQU	Mode_SVC,	0x13
	.EQU	Mode_ABT,	0x17
	.EQU	Mode_UND,	0x1B
	.EQU	Mode_SYS,	0x1F

	.EQU	I_BIT,	0x80	@ when I bit is set, IRQ is disabled
	.EQU	F_BIT,	0x40	@ when F bit is set, FIQ is disabled

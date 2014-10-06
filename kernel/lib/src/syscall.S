	.EQU	Mode_USR,	0x10

	.TEXT
	.ARM
	
	
	.GLOBAL __syscall_serv_send_para_and_idx
__syscall_serv_send_para_and_idx:

	STMFD	SP!,	{R0-R12,LR}	@ 保留当前用户进程的各个寄存器

	SWI	0x1
	MSR     CPSR_c, #Mode_USR	@ 此时需要切换回用户模式，由于是从用户模式跳转过来的，所以只有用户模式里的 LR 才指向着正确的返回地址

	LDMFD	SP!,	{R0-R12,PC}	@ 恢复各个寄存器到初始状态

	
	.GLOBAL __syscall_serv_run
__syscall_serv_run:

	STMFD	SP!,	{R0-R12,LR}
	
	SWI	0x2
	MSR     CPSR_c, #Mode_USR

	LDMFD	SP!,	{R0-R12,PC}

	
	.END

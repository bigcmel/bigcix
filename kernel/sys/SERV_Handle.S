	.include	"include/global.inc"

	.TEXT
	.ARM
	
	.GLOBL  __SERV_S_jmp_to_serv
__SERV_S_jmp_to_serv:

		STR	LR,	LR_BACKUP_ADDR

		LDR	LR, 	=RETURN

		BX	 R0 	@ 跳进 sys 服务的代码
	
RETURN:
	
		LDR	LR,	LR_BACKUP_ADDR
		BX	LR

LR_BACKUP_ADDR:
	.LONG	0

	
	.END

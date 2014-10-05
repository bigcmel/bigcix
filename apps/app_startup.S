	.TEXT
	.ARM

	
	.GLOBL	_start		@ default entry point
	.EXTERN	main
_start:

		STR	LR,	LR_BACKUP_ADDR

		LDR	R0,	=main
		BX	R0

		LDR	LR,	LR_BACKUP_ADDR
		BX	LR

LR_BACKUP_ADDR:
	.LONG	0
	
	.END


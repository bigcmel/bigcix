/*****************************************************************************/
/* S3C2440.S: Startup file for loader.bin of Samsung S3C2440                 */
/*****************************************************************************/
	

/*----------------------- CODE -----------------------------------------------*/

/* Area Definition and Entry Point
@  Startup Code must be linked first at Address at which it expects to run.*/

	.TEXT
	.ARM

	.GLOBL	_start		@ default entry point
_start:

@ Enter the C code -------------------------------------------------------------

	.EXTERN __main
                BL	__main	@ __main函数将返回 kernel.bin 的加载地址到 R0
	
		MOV	PC, R0	@ 此时跳入 kernel

		B	.


	.END

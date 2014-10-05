###################################
# Makefile for Kernel of nansl    #
###################################

ENTRYPOINT	= 0x30004000

# Programs, flags, etc.
ARM-TOOLS-PATH	= /home/bigc/Apps/arm-linux-toolchain-4.4.3/bin
HARDWARE-PATH	= ./hardware
SYS-FILES-PATH	= ./sys
OBJ-FILES-PATH	= ./OBJS
LIB-FILES-PATH	= ./lib
ASM		= $(ARM-TOOLS-PATH)/arm-linux-as
DASM		= $(ARM-TOOLS-PATH)/arm-linux-objdump
CC		= $(ARM-TOOLS-PATH)/arm-linux-gcc
LD		= $(ARM-TOOLS-PATH)/arm-linux-ld
OBJ		= $(ARM-TOOLS-PATH)/arm-linux-objcopy
ASMFLAGS	= 
CFLAGS		= -c
LDFLAGS		= -Ttext $(ENTRYPOINT)
OBJFLAGS	= -O binary -S
DASMFLAGS	= -D -b binary -m arm
LIBPATH		= -lgcc -L $(ARM-TOOLS-PATH)/../lib/gcc/arm-none-linux-gnueabi/4.4.3	# 添加 gcc 提供的静态库，比如提供了除法功能的软实现，因为 arm 的硬件没有提供除法功能

# This Program
NANSLKERNEL	= kernel.bin
NANSLKERNELELF	= kernel.elf
OBJS		= $(OBJ-FILES-PATH)/startup.o $(OBJ-FILES-PATH)/main.o \
		$(OBJ-FILES-PATH)/nand_flash.o $(OBJ-FILES-PATH)/uart.o  \
		$(OBJ-FILES-PATH)/exception_handlers.o  \
		$(OBJ-FILES-PATH)/initd.o $(OBJ-FILES-PATH)/serv.o \
		$(OBJ-FILES-PATH)/sys_power.o $(OBJ-FILES-PATH)/APP_Handler.o \
		$(OBJ-FILES-PATH)/SWI_Handler.o $(OBJ-FILES-PATH)/memory.o \
		$(OBJ-FILES-PATH)/SERV_Handle.o
LIBS		= $(LIB-FILES-PATH)/syscall.lib
DASMOUTPUT	= kernel.bin.asm

# All Phony Targets
.PHONY : everything final clean realclean disasm all

# Default starting position
everything : $(NANSLKERNEL) $(LIBS)

all : realclean everything

final : all clean

clean :
	rm -f $(OBJS) $(LIBS) $(DASMOUTPUT) $(NANSLKERNELELF)

realclean :
	rm -f $(NANSLKERNEL) $(OBJS) $(LIBS) $(DASMOUTPUT) $(NANSLKERNELELF)

disasm :
	$(DASM) $(DASMFLAGS) $(NANSLKERNEL) > $(DASMOUTPUT)


# All object dependencies files
$(NANSLKERNEL) : $(NANSLKERNELELF)
	$(OBJ) $(OBJFLAGS) $< $@

$(NANSLKERNELELF) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBPATH)


$(OBJ-FILES-PATH)/main.o: main.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/startup.o: startup.S
	$(ASM) $(ASMFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/nand_flash.o: $(HARDWARE-PATH)/nand_flash.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/uart.o: $(HARDWARE-PATH)/uart.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/exception_handlers.o: $(SYS-FILES-PATH)/exception_handlers.S
	$(ASM) $(ASMFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/initd.o: $(SYS-FILES-PATH)/initd.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/serv.o: $(SYS-FILES-PATH)/serv.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/sys_power.o: $(SYS-FILES-PATH)/sys_power.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/APP_Handler.o: $(SYS-FILES-PATH)/APP_Handler.S
	$(ASM) $(ASMFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/SWI_Handler.o: $(SYS-FILES-PATH)/SWI_Handler.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/syscall.o: $(SYS-FILES-PATH)/syscall.S
	$(ASM) $(ASMFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/mm.o: $(SYS-FILES-PATH)/mm.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/cpum.o: $(SYS-FILES-PATH)/cpum.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/memory.o:  $(HARDWARE-PATH)/memory.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/SERV_Handle.o: $(SYS-FILES-PATH)/SERV_Handle.S
	$(ASM) $(ASMFLAGS) -o $@ $<




$(LIB-FILES-PATH)/syscall.lib: $(LIB-FILES-PATH)/src/syscall.S
	$(ASM) $(ASMFLAGS) -o $@ $<

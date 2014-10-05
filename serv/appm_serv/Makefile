#################################
# Makefile for uart_serv	#
#################################

ENTRYPOINT	= 0x31204000

SERV_NAME	= appm

# Programs, flags, etc.
OBJ-FILES-PATH	= ./OBJS
#LIB-FILES-PATH	= /home/bigc/workspace/testkernel/lib
ARM-TOOLS-PATH	= /home/bigc/Apps/arm-linux-toolchain-4.4.3/bin
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

# This Program
SERV		= $(SERV_NAME).serv
SERVELF		= $(SERV_NAME).elf
OBJS		= $(OBJ-FILES-PATH)/serv_startup.o $(OBJ-FILES-PATH)/main.o \
	   	$(OBJ-FILES-PATH)/serv_lib_funcs.o $(OBJ-FILES-PATH)/serv_global.o \
		$(OBJ-FILES-PATH)/APP_Handler.o
DASMOUTPUT	= $(SERV_NAME).bin.asm

# All Phony Targets
.PHONY : everything final clean realclean disasm all

# Default starting position
everything : $(SERV)

all : realclean everything

final : all clean

clean :
	rm -f $(OBJS) $(DASMOUTPUT) $(SERVELF)

realclean :
	rm -f $(OBJS) $(DASMOUTPUT) $(SERVELF) $(SERV)

disasm :
	$(DASM) $(DASMFLAGS) $(SERV) > $(DASMOUTPUT)


# All object dependencies files
$(SERV) : $(SERVELF)
	$(OBJ) $(OBJFLAGS) $< $@

$(SERVELF) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)


$(OBJ-FILES-PATH)/main.o: main.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/serv_startup.o: ../serv_startup.S
	$(ASM) $(ASMFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/serv_lib_funcs.o: serv_lib_funcs.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/serv_global.o: ../serv_global.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/APP_Handler.o: APP_Handler.S
	$(ASM) $(ASMFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/uart.o: ../../testkernel/hardware/uart.c
	$(CC) $(CFLAGS) -o $@ $<

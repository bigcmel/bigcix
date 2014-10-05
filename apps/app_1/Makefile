#################################
# Makefile for app_0		#
#################################

ENTRYPOINT	= 0x00000000

APP_NAME	= app_1

# Programs, flags, etc.
OBJ-FILES-PATH	= ./OBJS
LIB-FILES-PATH	= ../../kernel/lib
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
APP		= $(APP_NAME).app
APPELF		= $(APP_NAME).elf
OBJS		= $(OBJ-FILES-PATH)/app_startup.o $(OBJ-FILES-PATH)/main.o \
		$(OBJ-FILES-PATH)/app_lib_funcs.o $(OBJ-FILES-PATH)/uart.o
LIBS		= $(LIB-FILES-PATH)/syscall.lib
DASMOUTPUT	= $(APP_NAME).bin.asm

# All Phony Targets
.PHONY : everything final clean realclean disasm all

# Default starting position
everything : $(APP)

all : realclean everything

final : all clean

clean :
	rm -f $(OBJS) $(DASMOUTPUT) $(APPELF)

realclean :
	rm -f $(OBJS) $(DASMOUTPUT) $(APPELF) $(APP)

disasm :
	$(DASM) $(DASMFLAGS) $(APP) > $(DASMOUTPUT)


# All object dependencies files
$(APP) : $(APPELF)
	$(OBJ) $(OBJFLAGS) $< $@

$(APPELF) : $(OBJS) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)


$(OBJ-FILES-PATH)/main.o: main.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/app_startup.o: ../app_startup.S
	$(ASM) $(ASMFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/app_lib_funcs.o: ../app_lib_funcs.c
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ-FILES-PATH)/uart.o: ../../kernel/hardware/uart.c
	$(CC) $(CFLAGS) -o $@ $<
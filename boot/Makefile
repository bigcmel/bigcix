#################################
# Makefile for Boot of nansl    #
#################################

ENTRYPOINT	= 0x00000000

# Programs, flags, etc.
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
LIBPATH		= -lgcc -L $(ARM-TOOLS-PATH)/../lib/gcc/arm-none-linux-gnueabi/4.4.3	# 添加 gcc 提供的静态库，比如提供了除法功能的软实现，因为 arm 的硬件没有提供除法功能

# This Program
NANSLBOOT	= boot.bin
NANSLBOOTELF	= boot.elf
OBJS		= startup.o main.o clock.o gpio.o uart.o nand_flash.o
DASMOUTPUT	= boot.bin.asm

# All Phony Targets
.PHONY : everything final clean realclean disasm all

# Default starting position
everything : $(NANSLBOOT)

all : realclean everything

final : all clean

clean :
	rm -f $(OBJS) $(DASMOUTPUT) $(NANSLBOOTELF)

realclean :
	rm -f $(OBJS) $(DASMOUTPUT) $(NANSLBOOTELF) $(NANSLBOOT)

disasm :
	$(DASM) $(DASMFLAGS) $(NANSLBOOT) > $(DASMOUTPUT)

# All object dependencies files
$(NANSLBOOT) : $(NANSLBOOTELF)
	$(OBJ) $(OBJFLAGS) $< $@

$(NANSLBOOTELF) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBPATH)

startup.o: startup.S
	$(ASM) $(ASMFLAGS) -o $@ $<

main.o:	main.c
	$(CC) $(CFLAGS) -o $@ $<

clock.o: clock.c
	$(CC) $(CFLAGS) -o $@ $<

lcd.o: lcd.c
	$(CC) $(CFLAGS) -o $@ $<

gpio.o: gpio.c
	$(CC) $(CFLAGS) -o $@ $<

uart.o: uart.c
	$(CC) $(CFLAGS) -o $@ $<

nand_flash.o: nand_flash.c
	$(CC) $(CFLAGS) -o $@ $<

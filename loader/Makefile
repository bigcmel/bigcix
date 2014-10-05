###################################
# Makefile for Loader of nansl    #
###################################

ENTRYPOINT	= 0x31000000

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

# This Program
NANSLLOADER	= loader.bin
NANSLLOADERELF	= loader.elf
OBJS		= startup.o main.o nand_flash.o lcd.o memory.o uart.o
DASMOUTPUT	= loader.bin.asm

# All Phony Targets
.PHONY : everything final clean realclean disasm all

# Default starting position
everything : $(NANSLLOADER)

all : realclean everything

final : all clean

clean :
	rm -f $(OBJS) $(DASMOUTPUT) $(NANSLLOADERELF)

realclean :
	rm -f $(OBJS) $(DASMOUTPUT) $(NANSLLOADERELF) $(NANSLLOADER)

disasm :
	$(DASM) $(DASMFLAGS) $(NANSLLOADER) > $(DASMOUTPUT)

# All object dependencies files
$(NANSLLOADER) : $(NANSLLOADERELF)
	$(OBJ) $(OBJFLAGS) $< $@

$(NANSLLOADERELF) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $<

startup.o: startup.S
	$(ASM) $(ASMFLAGS) -o $@ $<

nand_flash.o: nand_flash.c
	$(CC) $(CFLAGS) -o $@ $<

lcd.o: lcd.c
	$(CC) $(CFLAGS) -o $@ $<

memory.o: memory.c
	$(CC) $(CFLAGS) -o $@ $<

uart.o: uart.c
	$(CC) $(CFLAGS) -o $@ $<


# SPDX-License-Identifier: GPL-2.0 #
# Copyright (C) 2025 - Aleksandr Loktionov #

MCU=cortex-m0
DEF=-DSTM32F030x6 #-DMYBRD

OBJ=main.o
TARGET=m0blink

OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
READELF=arm-none-eabi-readelf
CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
DB=arm-none-eabi-gdb
SIZE=arm-none-eabi-size
INC = -I./include -I./CMSIS/device -I./CMSIS/core
CFLAGS=-mthumb -mcpu=$(MCU) -g -Os -Wall $(DEF) $(INC)
ASFLAGS=-mthumb -mcpu=$(MCU) -g -Wall
LDFLAGS=-T./ld/Link.ld

.PHONY: all clean

%.o:	%.c
	$(CC) -c -o $@ $< $(CFLAGS)
%.o:	%.s
	$(CC) -c -o $@ $< $(ASFLAGS)

$(TARGET).elf:	$(OBJ)
	$(LD) $(LDFLAGS) -g  -o $(TARGET).elf  $(OBJ)
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin
	$(SIZE)  $(TARGET).elf

install: all
	st-flash  write $(TARGET).bin 0x08000000
flash: $(TARGET).elf
	$(DB) ./$(TARGET).elf -ex "target remote localhost:3333" -ex "load ./$(TARGET).elf" -ex "monitor reset" -ex "set confirm off" -ex "quit"
debug: $(TARGET).elf
	$(DB) ./$(TARGET).elf -ex "target remote localhost:3333" -ex "load ./$(TARGET).elf" -ex "monitor reset halt"
####
edit:
	vim main.c
load: $(TARGET).bin
	../openocd.sh -c "program $(TARGET).bin reset exit"
size: $(TARGET).elf
	$(SIZE) --format=SysV -x $^
readelf: $(TARGET).elf
	$(READELF) $^ -a
objdump: $(TARGET).elf
	$(OBJDUMP) -D > $(TARGET).dmp $^
nm: $(TARGET).elf
	$(NM) $^
###
clean:
	@rm -vf $(TARGET).elf $(TARGET).bin $(OBJ) *.map *.dmp

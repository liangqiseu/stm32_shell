CC:=arm-none-eabi-gcc
ASM:=arm-none-eabi-as
LD:=arm-none-eabi-ld
OBJCOPY:=arm-none-eabi-objcopy
OBJDUMP:=arm-none-eabi-objdump
TCL:=tclsh8.6


INCLUDES:=-I. -I../src/includes
CFLAG:=--specs=nosys.specs -mcpu=cortex-m3 -mfloat-abi=soft -funwind-tables
ASFLAG:=-mcpu=cortex-m3
LDFLAG:=-mcpu=cortex-m3 --specs=nosys.specs 

DIR_SRC:=../src
DIR_OBJ:=.
DIR_SYMBOL_C:=../src/sys

SRC_C:=$(wildcard $(DIR_SRC)/*.c)
SRC_C+=$(wildcard $(DIR_SRC)/boot/*.c)
SRC_C+=$(wildcard $(DIR_SRC)/sys/*.c)
SRC_C+=$(wildcard $(DIR_SRC)/prph/*.c)
SRC_C+=$(wildcard $(DIR_SRC)/os/*.c)
SRC_C+=$(wildcard $(DIR_SRC)/shell/*.c)
SRC_S:=$(wildcard $(DIR_SRC)/boot/*.s)


OBJ_O:=$(patsubst %.c,%.o,$(SRC_C))
OBJ_O+=$(patsubst %.s,%.o,$(SRC_S))

OUTFILE:=start

.PHONY:all clean
all:$(OBJ_O)
	$(CC) $(LDFLAG) -T stm32f10x_flash.ld -o $(OUTFILE).elf -Wl,-Map=$(OUTFILE).map ./*.o
	$(TCL) exportSymbol.tcl
	mv symbol.c  $(DIR_SYMBOL_C)
	$(CC) $(INCLUDES) $(CFLAG) -c $(DIR_SYMBOL_C)/symbol.c -o $(DIR_OBJ)/symbol.o
	$(CC) $(LDFLAG) -T stm32f10x_flash.ld -o $(OUTFILE).elf -Wl,-Map=start.map ./*.o
	
	$(OBJCOPY) $(OUTFILE).elf -Oihex $(OUTFILE).hex
%.o:%.c
	$(CC) $(INCLUDES) $(CFLAG) -c $< -o $(DIR_OBJ)/$(notdir $@)
%.o:%.s
	$(ASM) $(ASFLAG) -c $< -o $(DIR_OBJ)/$(notdir $@)

asm:
	$(OBJDUMP) -D $(OUTFILE).elf > $(OUTFILE).asm

clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.map 
	rm -f $(DIR_SYMBOL_C)/symbol.c
	rm -f *.asm

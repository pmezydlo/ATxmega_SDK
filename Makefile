# Name: Makefile
# Author: <insert your name here>
# Copyright: <insert your copyright message here>
# License: <insert your license reference here>

# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected. We recommend that you leave it undefined and
#                add settings like this to your ~/.avrduderc file:
#                   default_programmer = "stk500v2"
#                   default_serial = "avrdoper"
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     = atxmega128a1
DEVICE2     = x128a1
CLOCK      = 32000000
PROGRAMMER = -c avrispmkII
OBJECTS    = main.o
OBJECTS    += Library/Usart/usart.o
OBJECTS    += Library/Debug/debug.o
OBJECTS    += Library/Ebi/ebi.o
OBJECTS    += Library/Clk/clk.o

#FUSES      = -U lfuse:w:0x5e:m -U hfuse:w:0x91:m -U efuse:w:0xf3:m
PORT		= usb

# For computing fuse byte values for other devices and options see
# the fuse bit calculator at http://www.engbedded.com/fusecalc/


# Tune the lines below only if you know what you are doing:

#AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)  -P $(PORT) -b57600 -D
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE2)  -P $(PORT)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -e -U flash:w:main.hex:i
#	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

program:
	$(AVRDUDE) -e -U flash:w:main.hex:i
#	$(AVRDUDE) -U flash:w:main.hex:i

burn:
	#python reset.py $(PORT)
	#sleep 2
	avrdude -p $(DEVICE2) -cavr109 -P$(PORT) -b57600 -D -Uflash:w:./Blink.cpp.hex:i

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
#	avr-size --format=avr --mcu=$(DEVICE) main.elf #configOutput.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c

# AVR-GCC Makefile
#---Prefs---
PROJECT=ROV_waterside
MMCU=atmega328p
F_CPU=8000000 # 16 MHz
#FUSEBITS=0x62
#-----------



SOURCES=main.c adc.c spi.c uart.c
CC=avr-gcc
OBJCOPY=avr-objcopy

CFLAGS=-g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wstrict-prototypes -DF_CPU=$(F_CPU) -Wa,-adhlns=$(<:.c=.lst) -I./ -mmcu=$(MMCU) -Wall
AVRDUDEFLAGS=-p $(MMCU)

ALL: $(PROJECT).hex filesize

$(PROJECT).hex: $(PROJECT).elf
	avr-objcopy -j .text -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROJECT).elf $(SOURCES)

program-isp: $(PROJECT).hex
	avrdude $(AVRDUDEFLAGS) -c usbtiny -F -e -U flash:w:$(PROJECT).hex

program: $(PROJECT).hex
	avrdude $(AVRDUDEFLAGS) -c stk500v1 -P /dev/ttyUSB0 -b 19200 -D -U flash:w:$(PROJECT).hex:i

burn-fuse:
	avrdude $(AVRDUDEFLAGS) -u -U lfuse:w:$(FUSEBITS):m

# Display size of file.
filesize:
	avr-size -C --mcu=$(MMCU) $(PROJECT).elf 

clean:
	rm -f $(PROJECT).elf $(PROJECT).hex $(PROJECT).lst

# AVR-GCC Makefile
#---Prefs---
PROJECT=ROV_waterside
MMCU=atmega168
F_CPU=16000000 # 16 MHz
FUSEBITS=0x62
#-----------



SOURCES=$(wildcard *.c)
CC=avr-gcc
OBJCOPY=avr-objcopy

CFLAGS=-g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wstrict-prototypes -DF_CPU=$(F_CPU) -Wa,-adhlns=$(<:.c=.lst) -I./ -mmcu=$(MMCU) -Wall
AVRDUDEFLAGS=-p $(MMCU)

ALL: $(PROJECT).hex filesize

$(PROJECT).hex: $(PROJECT).elf
	avr-objcopy -j .text -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROJECT).elf $(SOURCES)

program: $(PROJECT).hex
	avrdude $(AVRDUDEFLAGS) -c usbtiny -e -U flash:w:$(PROJECT).hex

program-serial: $(PROJECT).hex
	avrdude $(AVRDUDEFLAGS) -C arduino -P /dev/ttyUSB0 -b 19200 -D -U flash:w:$(PROJECT).hex:i

burn-fuse:
	avrdude $(AVRDUDEFLAGS) -u -U lfuse:w:$(FUSEBITS):m

# Display size of file.
filesize:
	avr-size -C --mcu=$(MMCU) $(PROJECT).elf 

clean:
	rm -f $(PROJECT).elf $(PROJECT).hex $(PROJECT).lst

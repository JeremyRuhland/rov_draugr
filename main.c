/*******************************************************************************
* ROV Waterside firmware
* 2014 WWU Marine Technology Club
* http://wwumarinetech.org/winter2014/Software/ROVWaterside
* Jeremy Ruhland
*
*******************************************************************************/

#include "includes.h"

void initGpio(void);

volatile uint8_t ReceivedCommand;
volatile ShiftRegister16Bit ShiftOut;

int main(void) {
    initGpio();
    initUart();
    initSpi();
    initAdc();

    for(;;) {
        getCommand();
	    runCommand();
    }
}

void initGpio(void) {
    PORTD |= (1<<PD7);
    DDRD |= ((1<<PD1) | (1<<PD7));

    PORTC |= (1<<PC5);
    DDRC |= (1<<PC5);

    PORTB |= ((1<<PB0) | (1<<PB1));
    DDRB |= ((1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB5));
}

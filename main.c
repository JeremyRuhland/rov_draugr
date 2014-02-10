/*******************************************************************************
* ROV Waterside firmware                                                       *
* 2014 WWU Marine Technology Club                                              *
* http://wwumarinetech.org/winter2014/Software/ROVWaterside                    *
* Jeremy Ruhland                                                               *
*                                                                              *
*******************************************************************************/

#include "includes.h"

void initGpio(void);

volatile uint8_t ReceivedCommand;
volatile ShiftRegister16Bit ShiftOut;
volatile uint16_t AnalogValues[8];

int main(void) {
    initGpio();
    initUart();
    initSpi();
    initAdc();

    sei();

    for(;;) {
        getCommand();
	    runCommand();
    }
}

void initGpio(void) {
    // Set all chip select lines high to disable on startup
    PORTD |= (1<<PD7);
    DDRD |= ((1<<PD1) | (1<<PD7));

    PORTC |= (1<<PC5);
    DDRC |= (1<<PC5);

    PORTB |= ((1<<PB0) | (1<<PB1));
    DDRB |= ((1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB5));
}

ISR(ADC_vect) {
    static uint8_t adcMux = 0;

    if (adcMux <= 4) { // Motor 0-4
        AnalogValues[adcMux] = adcResult();

        adcMux++;

        ADMUX = ((ADMUX & ~(0x0F)) | adcMux); // Switch to next mux

        // Start new conversion
        ADCSRA |= (1<<ADSC);
    } else if (adcMux == 5) { // Skip EN1
        adcMux++;

        // Start new conversion
        ADCSRA |= (1<<ADSC);
    } else if (adcMux <= 7) { // Temp 0-1
        AnalogValues[adcMux+1] = adcResult();

        adcMux++;

        ADMUX = ((ADMUX & ~(0x0F)) | (adcMux+1)); // Switch to next mux

        // Start new conversion
        ADCSRA |= (1<<ADSC);
    } else { // Internal Temp
        AnalogValues[adcMux+1] = adcResult();

        adcMux = 0; // Reset cycle

        ADMUX = ((ADMUX & ~(0x0F)) | (adcMux+1)); // Switch to next mux

        // Start new conversion
        ADCSRA |= (1<<ADSC);
    }
}

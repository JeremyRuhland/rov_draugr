/*******************************************************************************
* ROV Waterside firmware                                                       *
* 2014 WWU Marine Technology Club                                              *
* http://wwumarinetech.org/winter2014/Software/ROVWaterside                    *
* Jeremy Ruhland                                                               *
*                                                                              *
*******************************************************************************/

#include "includes.h"

void initGpio(void);
void runCommand(void);
void updateShift(void);

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

void runCommand(void) {
    static uint8_t oldCommand = 0x00;

    if (oldCommand != ReceivedCommand) {
        cli(); // Disable interrupt

        oldCommand = ReceivedCommand;
        ReceivedCommand = 0x00; // Reset received command to null

        // Case tree for received commands
        switch (oldCommand) {
            // Motor 1, stop
            case 0x01 :

                break;

            // Motor 1, forward
            case 0x02 :

                break;

            // Motor 1, backward
            case 0x03 :

                break;

            // Motor 2, stop
            case 0x04 :

                break;

            // Motor 2, forward
            case 0x05 :

                break;

            // Motor 2, backward
            case 0x06 :

                break;

            // Motor 3, stop
            case 0x08 :

                break;

            // Motor 3, forward
            case 0x09 :

                break;

            // Motor 3, backward
            case 0x0A:

                break;

            // Motor 4, stop
            case 0x0B :

                break;

            // Motor 4, forward
            case 0x0C :

                break;

            // Motor 4, backward
            case 0x0D :

                break;

            // Motor 5, stop
            case 0x0E :

                break;

            // Motor 5, forward
            case 0x0F :

                break;

            // Motor 5, backward
            case 0x10 :

                break;

            // All motors stop
            case 0x11 :

                break;

            // Get all motor status
            case 0x12 :

                break;

            // Get all temps
            case 0x13 :

                break;

            // Get water conductivity measurement
            case 0x14 :

                break;

            // Get sonar distance measuremnt
            case 0x15 :

                break;

            default:
        }

        sei(); // Reenable interrupts
    }
}

void updateShift(void) {
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

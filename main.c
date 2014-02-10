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
            // Motor 0, stop
            case ' ' :
                ShiftOut.motor0 = OFF;
                updateShift();
                break;

            // Motor 0, forward
            case '!' :
                #ifdef MOTOR0_REVERSE
                ShiftOut.motor0 = FORWARD;
                #else
                ShiftOut.motor0 = BACKWARDS;
                #endif
                updateShift();
                break;

            // Motor 0, backward
            case '"' :
                #ifdef MOTOR0_REVERSE
                ShiftOut.motor0 = BACKWARDS;
                #else
                ShiftOut.motor0 = FORWARDS;
                #endif
                updateShift();
                break;

            // Motor 1, stop
            case '#' :

                break;

            // Motor 1, forward
            case '$' :

                break;

            // Motor 1, backward
            case '%' :

                break;

            // Motor 2, stop
            case '&' :

                break;

            // Motor 2, forward
            case '\'' :

                break;

            // Motor 2, backward
            case '(':

                break;

            // Motor 3, stop
            case ')' :

                break;

            // Motor 3, forward
            case '*' :

                break;

            // Motor 3, backward
            case '+' :

                break;

            // Motor 5, stop
            case ',' :

                break;

            // Motor 5, forward
            case '-' :

                break;

            // Motor 5, backward
            case '.' :

                break;

            // All motors stop
            case '/' :

                break;

            // Get all motor status
            case '0' :

                break;

            // Get all temps
            case '1' :

                break;

            // Get water conductivity measurement
            case '2' :

                break;

            // Get sonar distance measuremnt
            case '3' :

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

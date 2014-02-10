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
                ShiftOut.motor0 = BACKWARDS;
                #else
                ShiftOut.motor0 = FORWARDS;
                #endif
                updateShift();
                break;

            // Motor 0, backward
            case '"' :
                #ifdef MOTOR0_REVERSE
                ShiftOut.motor0 = FORWARDS;
                #else
                ShiftOut.motor0 = BACKWARDS;
                #endif
                updateShift();
                break;

            // Motor 1, stop
            case '#' :
                ShiftOut.motor1 = OFF;
                break;

            // Motor 1, forward
            case '$' :
                #ifdef MOTOR1_REVERSE
                ShiftOut.motor1 = BACKWARDS;
                #else
                ShiftOut.motor1 = FORWARDS;
                #endif
                updateShift();
                break;

            // Motor 1, backward
            case '%' :
                #ifdef MOTOR1_REVERSE
                ShiftOut.motor1 = FORWARDS;
                #else
                ShiftOut.motor1 = BACKWARDS;
                #endif
                updateShift();
                break;

            // Motor 2, stop
            case '&' :
                ShiftOut.motor2 = OFF;
                break;

            // Motor 2, forward
            case '\'' :
                #ifdef MOTOR2_REVERSE
                ShiftOut.motor2 = BACKWARDS;
                #else
                ShiftOut.motor2 = FORWARDS;
                #endif
                updateShift();
                break;

            // Motor 2, backward
            case '(':
                #ifdef MOTOR2_REVERSE
                ShiftOut.motor2 = FORWARDS;
                #else
                ShiftOut.motor2 = BACKWARDS;
                #endif
                updateShift();
                break;

            // Motor 3, stop
            case ')' :
                ShiftOut.motor3 = OFF;
                break;

            // Motor 3, forward
            case '*' :
                #ifdef MOTOR3_REVERSE
                ShiftOut.motor3 = BACKWARDS;
                #else
                ShiftOut.motor3 = FORWARDS;
                #endif
                updateShift();
                break;

            // Motor 3, backward
            case '+' :
                #ifdef MOTOR3_REVERSE
                ShiftOut.motor3 = FORWARDS;
                #else
                ShiftOut.motor3 = BACKWARDS;
                #endif
                updateShift();
                break;

            // Motor 4, stop
            case ',' :
                ShiftOut.motor4 = OFF;
                break;

            // Motor 4, forward
            case '-' :
                #ifdef MOTOR4_REVERSE
                ShiftOut.motor4 = BACKWARDS;
                #else
                ShiftOut.motor4 = FORWARDS;
                #endif
                updateShift();
                break;

            // Motor 4, backward
            case '.' :
                #ifdef MOTOR4_REVERSE
                ShiftOut.motor4 = FORWARDS;
                #else
                ShiftOut.motor4 = BACKWARDS;
                #endif
                updateShift();
                break;

            // All motors stop
            case '/' :
                ShiftOut.motor0 = OFF;
                ShiftOut.motor1 = OFF;
                ShiftOut.motor2 = OFF;
                ShiftOut.motor3 = OFF;
                ShiftOut.motor4 = OFF;
                
                updateShift();
                break;

            // Toggle SW 0
            case '' :
                ShiftOut.switch0 = !(ShiftOut.switch0);

                updateShift();
                break;

            // Toggle SW 1
            case '' :
                ShiftOut.switch1 = !(ShiftOut.switch1);

                updateShift();
                break;

            // Toggle SW 2
            case '' :
                ShiftOut.switch2 = !(ShiftOut.switch2);

                updateShift();
                break;

            // Toggle SW 3
            case '' :
                ShiftOut.switch3 = !(ShiftOut.switch3);

                updateShift();
                break;

            // Toggle SW 4
            case '' :
                ShiftOut.switch4 = !(ShiftOut.switch4);

                updateShift();
                break;

            // Toggle SW 5
            case '' :
                ShiftOut.switch5 = !(ShiftOut.switch5);

                updateShift();
                break;

            // All switches off
            case '' :
                ShiftOut.switch0 = OFF;
                ShiftOut.switch1 = OFF;
                ShiftOut.switch2 = OFF;
                ShiftOut.switch3 = OFF;
                ShiftOut.switch4 = OFF;
                ShiftOut.switch5 = OFF;

                updateShift();
                break;

            // Get all motor status
            case '' :

                break;

            // Get all temps
            case '' :

                break;

            // Get water conductivity measurement
            case '' :

                break;

            // Get sonar distance measurement
            case '' :

                break;

            default:
        }
    }
}

void updateShift(void) {
    
}

ISR(ADC_vect) {
    static uint8_t adcMux = 0;

    if (adcMux <= 3) { // Motor 0-4
        AnalogValues[adcMux] = adcResult();

        adcMux++;

        ADMUX = ((ADMUX & ~(0x0F)) | adcMux); // Switch to next mux

        // Start new conversion
        ADCSRA |= (1<<ADSC);
    } else if (adcMux == 4) { // Skip EN1
        AnalogValues[adcMux] = adcResult();
    
        adcMux += 2;

        ADMUX = ((ADMUX & ~(0x0F)) | adcMux); // Switch to next mux
        
        // Start new conversion
        ADCSRA |= (1<<ADSC);
    } else if (adcMux <= 7) { // Temp 0-1
        AnalogValues[adcMux-1] = adcResult();

        adcMux++;

        ADMUX = ((ADMUX & ~(0x0F)) | adcMux); // Switch to next mux

        // Start new conversion
        ADCSRA |= (1<<ADSC);
    } else { // Internal Temp
        AnalogValues[adcMux-1] = adcResult();

        adcMux = 0; // Reset cycle

        ADMUX = ((ADMUX & ~(0x0F)) | adcMux); // Switch to next mux

        // Start new conversion
        ADCSRA |= (1<<ADSC);
    }
}

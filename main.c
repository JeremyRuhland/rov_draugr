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
void sendTemps(void);
void sendCurrents(void);

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
        ReceivedCommand = uartRx();
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

                uartTx(TRUE); // ACK after command
                break;

            // Motor 0, forward
            case '!' :
                #ifdef MOTOR0_REVERSE
                ShiftOut.motor0 = BACKWARDS;
                #else
                ShiftOut.motor0 = FORWARDS;
                #endif
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 0, backward
            case '"' :
                #ifdef MOTOR0_REVERSE
                ShiftOut.motor0 = FORWARDS;
                #else
                ShiftOut.motor0 = BACKWARDS;
                #endif
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 1, stop
            case '#' :
                ShiftOut.motor1 = OFF;
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 1, forward
            case '$' :
                #ifdef MOTOR1_REVERSE
                ShiftOut.motor1 = BACKWARDS;
                #else
                ShiftOut.motor1 = FORWARDS;
                #endif
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 1, backward
            case '%' :
                #ifdef MOTOR1_REVERSE
                ShiftOut.motor1 = FORWARDS;
                #else
                ShiftOut.motor1 = BACKWARDS;
                #endif
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 2, stop
            case '&' :
                ShiftOut.motor2 = OFF;
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 2, forward
            case '\'' :
                #ifdef MOTOR2_REVERSE
                ShiftOut.motor2 = BACKWARDS;
                #else
                ShiftOut.motor2 = FORWARDS;
                #endif
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 2, backward
            case '(':
                #ifdef MOTOR2_REVERSE
                ShiftOut.motor2 = FORWARDS;
                #else
                ShiftOut.motor2 = BACKWARDS;
                #endif
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 3, stop
            case ')' :
                ShiftOut.motor3 = OFF;
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 3, forward
            case '*' :
                #ifdef MOTOR3_REVERSE
                ShiftOut.motor3 = BACKWARDS;
                #else
                ShiftOut.motor3 = FORWARDS;
                #endif
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 3, backward
            case '+' :
                #ifdef MOTOR3_REVERSE
                ShiftOut.motor3 = FORWARDS;
                #else
                ShiftOut.motor3 = BACKWARDS;
                #endif
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 4, stop
            case ',' :
                ShiftOut.motor4 = OFF;
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 4, forward
            case '-' :
                #ifdef MOTOR4_REVERSE
                ShiftOut.motor4 = BACKWARDS;
                #else
                ShiftOut.motor4 = FORWARDS;
                #endif
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Motor 4, backward
            case '.' :
                #ifdef MOTOR4_REVERSE
                ShiftOut.motor4 = FORWARDS;
                #else
                ShiftOut.motor4 = BACKWARDS;
                #endif
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // All motors stop
            case '/' :
                ShiftOut.motor0 = OFF;
                ShiftOut.motor1 = OFF;
                ShiftOut.motor2 = OFF;
                ShiftOut.motor3 = OFF;
                ShiftOut.motor4 = OFF;
                
                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Toggle SW 0
            case '0' :
                ShiftOut.switch0 = !(ShiftOut.switch0);

                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Toggle SW 1
            case '1' :
                ShiftOut.switch1 = !(ShiftOut.switch1);

                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Toggle SW 2
            case '2' :
                ShiftOut.switch2 = !(ShiftOut.switch2);

                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Toggle SW 3
            case '3' :
                ShiftOut.switch3 = !(ShiftOut.switch3);

                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Toggle SW 4
            case '4' :
                ShiftOut.switch4 = !(ShiftOut.switch4);

                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Toggle SW 5
            case '5' :
                ShiftOut.switch5 = !(ShiftOut.switch5);

                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // All switches off
            case '6' :
                ShiftOut.switch0 = OFF;
                ShiftOut.switch1 = OFF;
                ShiftOut.switch2 = OFF;
                ShiftOut.switch3 = OFF;
                ShiftOut.switch4 = OFF;
                ShiftOut.switch5 = OFF;

                updateShift();

                uartTx(TRUE); // ACK after command
                break;

            // Get all motor status
            case '7' :
                //((PORTD & 0x7C)>>2)
                break;

            // Get all temps
            case '8' :
                sendTemps();
                break;

            // Get all motor currents
            case '9' :
                sendCurrents();
                break;

            // Get water conductivity measurement
            case ':' :

                break;

            // Get sonar distance measurement
            case ';' :

                break;

            // Get compass heading
            case '<' :

                break;

            default:
        }
    }
}

void sendTemps(void) {
    int8_t temps[3];
    uint8_t i;

    cli(); // Disable interrupts

    for (i = 5; i <= 7; i++) {
        temp[i-5] = (int8_t) adcFormatTemp(AnalogValues[i]);
    }

    for (i = 0; i <= 3; i++) {
        
    }

    sei(); // Reenable interrupts
}

void sendCurrents(void) {
    uint8_t temps[5];
    uint8_t i;

    cli(); // Disable interrupts

    

    sei(); // Reenable interrupts
}

void updateShift(void) {
    cli(); // Disable interrupts

    spiPeripheralSelect(0); // Disable all peripherals

    // Transmit new shift register data
    (void) spiByte((uint8_t) ShiftOut);
    (void) spiByte((uint8_t) (ShiftOut>>8));

    _delay_us(0.025); // Delay 25nS for setup time of shift register

    // Rising edge on latch, delay and drop
    PORTB |= (1<<PB2);
    _delay_us(0.025); // Delay 25nS for setup time of shift register
    PORTB &= ~(1<<PB2);

    sei(); // Reenable interrupts
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

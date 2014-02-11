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
    uartInit();
    spiInit();
    adcInit();

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
                ShiftOut.bit.motor0 = OFF;
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 0, forward
            case '!' :
              #ifdef MOTOR0_REVERSE
                ShiftOut.bit.motor0 = BACKWARD;
              #else
                ShiftOut.bit.motor0 = FORWARD;
              #endif
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 0, backward
            case '"' :
              #ifdef MOTOR0_REVERSE
                ShiftOut.bit.motor0 = FORWARD;
              #else
                ShiftOut.bit.motor0 = BACKWARD;
              #endif
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 1, stop
            case '#' :
                ShiftOut.bit.motor1 = OFF;
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 1, forward
            case '$' :
              #ifdef MOTOR1_REVERSE
                ShiftOut.bit.motor1 = BACKWARD;
              #else
                ShiftOut.bit.motor1 = FORWARD;
              #endif
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 1, backward
            case '%' :
              #ifdef MOTOR1_REVERSE
                ShiftOut.bit.motor1 = FORWARD;
              #else
                ShiftOut.bit.motor1 = BACKWARD;
              #endif
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 2, stop
            case '&' :
                ShiftOut.bit.motor2 = OFF;
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 2, forward
            case '\'' :
              #ifdef MOTOR2_REVERSE
                ShiftOut.bit.motor2 = BACKWARD;
              #else
                ShiftOut.bit.motor2 = FORWARD;
              #endif
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 2, backward
            case '(':
              #ifdef MOTOR2_REVERSE
                ShiftOut.bit.motor2 = FORWARD;
              #else
                ShiftOut.bit.motor2 = BACKWARD;
              #endif
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 3, stop
            case ')' :
                ShiftOut.bit.motor3 = OFF;
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 3, forward
            case '*' :
              #ifdef MOTOR3_REVERSE
                ShiftOut.bit.motor3 = BACKWARD;
              #else
                ShiftOut.bit.motor3 = FORWARD;
              #endif
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 3, backward
            case '+' :
              #ifdef MOTOR3_REVERSE
                ShiftOut.bit.motor3 = FORWARD;
              #else
                ShiftOut.bit.motor3 = BACKWARD;
              #endif
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 4, stop
            case ',' :
                ShiftOut.bit.motor4 = OFF;
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 4, forward
            case '-' :
              #ifdef MOTOR4_REVERSE
                ShiftOut.bit.motor4 = BACKWARD;
              #else
                ShiftOut.bit.motor4 = FORWARD;
              #endif
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Motor 4, backward
            case '.' :
              #ifdef MOTOR4_REVERSE
                ShiftOut.bit.motor4 = FORWARD;
              #else
                ShiftOut.bit.motor4 = BACKWARD;
              #endif
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // All motors stop
            case '/' :
                ShiftOut.bit.motor0 = OFF;
                ShiftOut.bit.motor1 = OFF;
                ShiftOut.bit.motor2 = OFF;
                ShiftOut.bit.motor3 = OFF;
                ShiftOut.bit.motor4 = OFF;
                
                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Toggle SW 0
            case '0' :
                ShiftOut.bit.switch0 = !(ShiftOut.bit.switch0);

                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Toggle SW 1
            case '1' :
                ShiftOut.bit.switch1 = !(ShiftOut.bit.switch1);

                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Toggle SW 2
            case '2' :
                ShiftOut.bit.switch2 = !(ShiftOut.bit.switch2);

                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Toggle SW 3
            case '3' :
                ShiftOut.bit.switch3 = !(ShiftOut.bit.switch3);

                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Toggle SW 4
            case '4' :
                ShiftOut.bit.switch4 = !(ShiftOut.bit.switch4);

                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // Toggle SW 5
            case '5' :
                ShiftOut.bit.switch5 = !(ShiftOut.bit.switch5);

                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
                break;

            // All switches off
            case '6' :
                ShiftOut.bit.switch0 = OFF;
                ShiftOut.bit.switch1 = OFF;
                ShiftOut.bit.switch2 = OFF;
                ShiftOut.bit.switch3 = OFF;
                ShiftOut.bit.switch4 = OFF;
                ShiftOut.bit.switch5 = OFF;

                updateShift();

                uartTxWord((uint16_t) TRUE); // ACK after command
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
                break;
        }
    }
}

void sendTemps(void) {
    int16_t temps[3];
    uint8_t i;

    cli(); // Disable interrupts

    // Convert analog values to temp array
    for (i = 5; i <= 7; i++) {
        temps[i-5] = adcFormatTemp(AnalogValues[i]);
    }

    // Send temp array
    for (i = 0; i <= 3; i++) {
        uartTxWord(temps[i]);
    }

    sei(); // Reenable interrupts
}

void sendCurrents(void) {
    int16_t currents[5];
    uint8_t i;

    cli(); // Disable interrupts

    // Convert analog values to currents array
    for (i = 0; i <= 4; i++) {
        currents[i] = adcFormatVoltage(AnalogValues[i]);
    }

    // Send currents array
    for (i = 0; i <= 4; i++) {
        uartTxWord(currents[i]);
    }

    sei(); // Reenable interrupts
}

void updateShift(void) {
    cli(); // Disable interrupts

    spiPeripheralSelect(0); // Disable all peripherals

    // Transmit new shift register data
    (void) spiByte((uint8_t) ShiftOut.uart);
    (void) spiByte((uint8_t) (ShiftOut.uart>>8));

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

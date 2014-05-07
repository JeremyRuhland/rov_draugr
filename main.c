/*******************************************************************************
* ROV Waterside firmware                                                       *
* 2014 WWU Marine Technology Club                                              *
* http://wwumarinetech.org/winter2014/Software/ROVWaterside                    *
* Jeremy Ruhland                                                               *
*                                                                              *
*******************************************************************************/

#include "includes.h"

// Function prototypes
void initGpio(void);
void runCommand(void);
void updateShift(void);
void sendTemps(void);
void sendCurrents(void);
void sendMotorStatus(void);
void allStop(void);

#ifdef DEAD_RECON_DEMO
void deadReconDemo(void);
#endif

// Global variables
volatile uint8_t ReceivedCommand; // Buffer for received command
volatile ShiftRegister16Bit ShiftOut; // Buffer for shift register
volatile uint16_t AnalogValues[8]; // Buffer for ADC results

int main(void) {
    // Initialize all functions
    initGpio();

    spiInit();
    updateShift(); // Zero shift registers

    uartInit();
    adcInit();

    //sei();

    #ifdef DEAD_RECON_DEMO
    deadReconDemo();
    for (;;) {}
    #else

    // Main control loop
    for(;;) {
        ReceivedCommand = uartRx(); // Check buffer for waiting command
        runCommand(); // Parse and run command
    }
    #endif
}

void initGpio(void) {
    // Set all chip select lines high to disable peripherals on startup
    PORTD |= (1<<PD7);
    DDRD |= ((1<<PD1) | (1<<PD7));

    PORTC |= (1<<PC5);
    DDRC |= (1<<PC5);

    PORTB |= ((0<<PB0) | (1<<PB1)); // Set PB0 back to output after testing
    DDRB |= ((0<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB5)); // Set PB0 back to output after testing
}

void runCommand(void) {
    static uint8_t oldCommand = 0x00;

    // Only run if new command has been received
    if (oldCommand != ReceivedCommand) {
        oldCommand = ReceivedCommand;
        ReceivedCommand = 0x00; // Reset received command to null

        // Parse and execute received commands
        switch (oldCommand) {
           #ifdef DIRECTIONAL_CONTROLLS // Directional motor control protocol
            // Forward
            case CASE_FORWARD :
                allStop();

              #ifdef MOTOR0_REVERSE
                ShiftOut.bit.motor0 = BACKWARD;
              #else
                ShiftOut.bit.motor0 = FORWARD;
              #endif

              #ifdef MOTOR1_REVERSE
                ShiftOut.bit.motor1 = BACKWARD;
              #else
                ShiftOut.bit.motor1 = FORWARD;
              #endif

                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Backward
            case CASE_BACKWARD :
                allStop();

              #ifdef MOTOR0_REVERSE
                ShiftOut.bit.motor0 = FORWARD;
              #else
                ShiftOut.bit.motor0 = BACKWARD;
              #endif

              #ifdef MOTOR1_REVERSE
                ShiftOut.bit.motor1 = FORWARD;
              #else
                ShiftOut.bit.motor1 = BACKWARD;
              #endif

                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Left
            case CASE_LEFT :
                allStop();

              #ifdef MOTOR4_REVERSE
                ShiftOut.bit.motor4 = BACKWARD;
              #else
                ShiftOut.bit.motor4 = FORWARD;
              #endif

                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Right
            case CASE_RIGHT :
                allStop();

              #ifdef MOTOR4_REVERSE
                ShiftOut.bit.motor4 = FORWARD;
              #else
                ShiftOut.bit.motor4 = BACKWARD;
              #endif

                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Up
            case CASE_UP :
                allStop();

              #ifdef MOTOR2_REVERSE
                ShiftOut.bit.motor2 = BACKWARD;
              #else
                ShiftOut.bit.motor2 = FORWARD;
              #endif

              #ifdef MOTOR3_REVERSE
                ShiftOut.bit.motor3 = BACKWARD;
              #else
                ShiftOut.bit.motor3 = FORWARD;
              #endif

                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Down
            case CASE_DOWN :
                allStop();

              #ifdef MOTOR2_REVERSE
                ShiftOut.bit.motor2 = FORWARD;
              #else
                ShiftOut.bit.motor2 = BACKWARD;
              #endif

              #ifdef MOTOR3_REVERSE
                ShiftOut.bit.motor3 = FORWARD;
              #else
                ShiftOut.bit.motor3 = BACKWARD;
              #endif

                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Clockwise
            case CASE_CLOCKWISE :
                allStop();

              #ifdef MOTOR0_REVERSE
                ShiftOut.bit.motor0 = BACKWARD;
              #else
                ShiftOut.bit.motor0 = FORWARD;
              #endif

              #ifdef MOTOR1_REVERSE
                ShiftOut.bit.motor1 = FORWARD;
              #else
                ShiftOut.bit.motor1 = BACKWARD;
              #endif

                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Anticlockwise
            case CASE_ANTICLOCKWISE :
                allStop();

              #ifdef MOTOR0_REVERSE
                ShiftOut.bit.motor0 = FORWARD;
              #else
                ShiftOut.bit.motor0 = BACKWARD;
              #endif

              #ifdef MOTOR1_REVERSE
                ShiftOut.bit.motor1 = BACKWARD;
              #else
                ShiftOut.bit.motor1 = FORWARD;
              #endif

                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // All Stop
            case CASE_ALL_STOP :
                allStop();

                uartTxByte(ACK); // ACK after command
                break;

           #else // Individual motor control protocol

            // Motor 0, stop
            case CASE_MOTOR_0_STOP :
                ShiftOut.bit.motor0 = OFF;
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 0, forward
            case CASE_MOTOR_0_FORWARD :
              #ifdef MOTOR0_REVERSE
                ShiftOut.bit.motor0 = BACKWARD;
              #else
                ShiftOut.bit.motor0 = FORWARD;
              #endif
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 0, backward
            case CASE_MOTOR_0_BACKWARD :
              #ifdef MOTOR0_REVERSE
                ShiftOut.bit.motor0 = FORWARD;
              #else
                ShiftOut.bit.motor0 = BACKWARD;
              #endif
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 1, stop
            case CASE_MOTOR_1_STOP :
                ShiftOut.bit.motor1 = OFF;
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 1, forward
            case CASE_MOTOR_1_FORWARD :
              #ifdef MOTOR1_REVERSE
                ShiftOut.bit.motor1 = BACKWARD;
              #else
                ShiftOut.bit.motor1 = FORWARD;
              #endif
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 1, backward
            case CASE_MOTOR_1_BACKWARD :
              #ifdef MOTOR1_REVERSE
                ShiftOut.bit.motor1 = FORWARD;
              #else
                ShiftOut.bit.motor1 = BACKWARD;
              #endif
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 2, stop
            case CASE_MOTOR_2_STOP :
                ShiftOut.bit.motor2 = OFF;
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 2, forward
            case CASE_MOTOR_2_FORWARD :
              #ifdef MOTOR2_REVERSE
                ShiftOut.bit.motor2 = BACKWARD;
              #else
                ShiftOut.bit.motor2 = FORWARD;
              #endif
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 2, backward
            case CASE_MOTOR_2_BACKWARD :
              #ifdef MOTOR2_REVERSE
                ShiftOut.bit.motor2 = FORWARD;
              #else
                ShiftOut.bit.motor2 = BACKWARD;
              #endif
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 3, stop
            case CASE_MOTOR_3_STOP :
                ShiftOut.bit.motor3 = OFF;
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 3, forward
            case CASE_MOTOR_3_FORWARD :
              #ifdef MOTOR3_REVERSE
                ShiftOut.bit.motor3 = BACKWARD;
              #else
                ShiftOut.bit.motor3 = FORWARD;
              #endif
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 3, backward
            case CASE_MOTOR_3_BACKWARD :
              #ifdef MOTOR3_REVERSE
                ShiftOut.bit.motor3 = FORWARD;
              #else
                ShiftOut.bit.motor3 = BACKWARD;
              #endif
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 4, stop
            case CASE_MOTOR_4_STOP :
                ShiftOut.bit.motor4 = OFF;
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 4, forward
            case CASE_MOTOR_4_FORWARD :
              #ifdef MOTOR4_REVERSE
                ShiftOut.bit.motor4 = BACKWARD;
              #else
                ShiftOut.bit.motor4 = FORWARD;
              #endif
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Motor 4, backward
            case CASE_MOTOR_4_BACKWARD :
              #ifdef MOTOR4_REVERSE
                ShiftOut.bit.motor4 = FORWARD;
              #else
                ShiftOut.bit.motor4 = BACKWARD;
              #endif
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // All motors stop
            case CASE_ALL_MOTORS_STOP :
                ShiftOut.bit.motor0 = OFF;
                ShiftOut.bit.motor1 = OFF;
                ShiftOut.bit.motor2 = OFF;
                ShiftOut.bit.motor3 = OFF;
                ShiftOut.bit.motor4 = OFF;
                
                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Toggle SW 0
            case CASE_TOGGLE_SW_0 :
                ShiftOut.bit.switch0 = !(ShiftOut.bit.switch0);

                updateShift();

                if (ShiftOut.bit.switch0) {
                    uartTxByte(ACK); // ACK if on
                } else {
                    uartTxWord((uint16_t) NAK); // NAK if off
                }
                break;

            // Toggle SW 1
            case CASE_TOGGLE_SW_1 :
                ShiftOut.bit.switch1 = !(ShiftOut.bit.switch1);

                updateShift();

                if (ShiftOut.bit.switch1) {
                    uartTxByte(ACK); // ACK if on
                } else {
                    uartTxWord((uint16_t) NAK); // NAK if off
                }
                break;

            // Toggle SW 2
            case CASE_TOGGLE_SW_2 :
                ShiftOut.bit.switch2 = !(ShiftOut.bit.switch2);

                updateShift();

                if (ShiftOut.bit.switch2) {
                    uartTxByte(ACK); // ACK if on
                } else {
                    uartTxWord((uint16_t) NAK); // NAK if off
                }
                break;

            // Toggle SW 3
            case CASE_TOGGLE_SW_3 :
                ShiftOut.bit.switch3 = !(ShiftOut.bit.switch3);

                updateShift();

                if (ShiftOut.bit.switch3) {
                    uartTxByte(ACK); // ACK if on
                } else {
                    uartTxWord((uint16_t) NAK); // NAK if off
                }
                break;

            // Toggle SW 4
            case CASE_TOGGLE_SW_4 :
                ShiftOut.bit.switch4 = !(ShiftOut.bit.switch4);

                updateShift();

                if (ShiftOut.bit.switch4) {
                    uartTxByte(ACK); // ACK if on
                } else {
                    uartTxWord((uint16_t) NAK); // NAK if off
                }
                break;

            // Toggle SW 5
            case CASE_TOGGLE_SW_5 :
                ShiftOut.bit.switch5 = !(ShiftOut.bit.switch5);

                updateShift();

                if (ShiftOut.bit.switch5) {
                    uartTxByte(ACK); // ACK if on
                } else {
                    uartTxWord((uint16_t) NAK); // NAK if off
                }
                break;

            // All switches off
            case CASE_ALL_SWITCHES_OFF :
                ShiftOut.bit.switch0 = OFF;
                ShiftOut.bit.switch1 = OFF;
                ShiftOut.bit.switch2 = OFF;
                ShiftOut.bit.switch3 = OFF;
                ShiftOut.bit.switch4 = OFF;
                ShiftOut.bit.switch5 = OFF;

                updateShift();

                uartTxByte(ACK); // ACK after command
                break;

            // Get all motor status
            case CASE_MOTOR_STATUS :
                sendMotorStatus();
                break;

            // Get all temps
            case CASE_TEMPS :
                sendTemps();
                break;

            // Get all motor currents
            case CASE_CURRENTS :
                sendCurrents();
                break;

            // Get water conductivity measurement
            case CASE_CONDUCTIVITY :

                break;

            // Get sonar distance measurement
            case CASE_SONAR_DISTANCE :

                break;

            // Get compass heading
            case CASE_COMPASS :

                break;

            default:
                break;
           #endif
        }

       #ifdef TEXT_DEBUG
        // Debug routine to print human readable state information
        switch (oldCommand) {
            // Motor 0, stop
            case CASE_MOTOR_0_STOP :
                uartTxStrg("\n\rMotor 0 Stop");
                break;

            // Motor 0, forward
            case CASE_MOTOR_0_FORWARD :
                uartTxStrg("\n\rMotor 0 Forward");
                break;

            // Motor 0, backward
            case CASE_MOTOR_0_BACKWARD :
                uartTxStrg("\n\rMotor 0 Backward");
                break;

            // Motor 1, stop
            case CASE_MOTOR_1_STOP :
                uartTxStrg("\n\rMotor 1 Stop");
                break;

            // Motor 1, forward
            case CASE_MOTOR_1_FORWARD :
                uartTxStrg("\n\rMotor 1 Forward");
                break;

            // Motor 1, backward
            case CASE_MOTOR_1_BACKWARD :
                uartTxStrg("\n\rMotor 1 Backward");
                break;

            // Motor 2, stop
            case CASE_MOTOR_2_STOP :
                uartTxStrg("\n\rMotor 2 Stop");
                break;

            // Motor 2, forward
            case CASE_MOTOR_2_FORWARD :
                uartTxStrg("\n\rMotor 2 Forward");
                break;

            // Motor 2, backward
            case CASE_MOTOR_2_BACKWARD:
                uartTxStrg("\n\rMotor 2 Backward");
                break;

            // Motor 3, stop
            case CASE_MOTOR_3_STOP :
                uartTxStrg("\n\rMotor 3 Stop");
                break;

            // Motor 3, forward
            case CASE_MOTOR_3_FORWARD :
                uartTxStrg("\n\rMotor 3 Forward");
                break;

            // Motor 3, backward
            case CASE_MOTOR_3_BACKWARD :
                uartTxStrg("\n\rMotor 3 Backward");
                break;

            // Motor 4, stop
            case CASE_MOTOR_4_STOP :
                uartTxStrg("\n\rMotor 4 Stop");
                break;

            // Motor 4, forward
            case CASE_MOTOR_4_FORWARD :
                uartTxStrg("\n\rMotor 4 Forward");
                break;

            // Motor 4, backward
            case CASE_MOTOR_4_BACKWARD :
                uartTxStrg("\n\rMotor 4 Backward");
                break;

            // All motors stop
            case CASE_ALL_MOTORS_STOP :
                uartTxStrg("\n\rAll Motors Stop");
                break;

            default:
                uartTxStrg("\n\rUnknown Command");
                break;
        }
       #endif
    }
}

void sendMotorStatus(void) {
}

void sendTemps(void) {
    int16_t temp;
    uint8_t i;

    // Convert analog values and send
    for (i = 0; i <= 2; i++) {
        temp = adcFormatTemp(AnalogValues[i+5]);
        uartTxWord(temp);
    }
}

void sendCurrents(void) {
    uint16_t currents[5];
    uint8_t i;

    // Convert analog values to currents array
    for (i = 0; i <= 4; i++) {
        currents[i] = adcFormatVoltage(AnalogValues[i]);
    }

    // Send currents array
    for (i = 0; i <= 4; i++) {
        uartTxWord(currents[i]);
    }
}

void updateShift(void) {
    spiPeripheralSelect(0); // Disable all peripherals

    // Transmit new shift register data
    (void) spiByte((uint8_t) ShiftOut.uart);
    (void) spiByte((uint8_t) (ShiftOut.uart>>8));

    _delay_us(0.025); // Delay 25nS for setup time of shift register

    // Rising edge on latch, delay and drop
    PORTB |= (1<<PB2);
    _delay_us(0.025); // Delay 25nS for setup time of shift register
    PORTB &= ~(1<<PB2);
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

void allStop(void) {
    // Turn off all motors
    ShiftOut.uart = (uint16_t) OFF;
    updateShift();
    
    _delay_ms(1); // Let bridges settle
}

#ifdef DEAD_RECON_DEMO
void deadReconDemo(void) {
#if 0
    // This is uart debug code
    for(;;) {
        uartTxByte('q');

        uartTxStrg("");
        _delay_ms(100);
    }
#endif
#if 0
    // This is spi output code to test speeds
    uint8_t i = 0;

    for (;;) {
        //_delay_ms(1);
        spiPeripheralSelect(4);
        _delay_us(20);
        spiByte(i);
        spiPeripheralSelect(0);
        i++;
        //_delay_us(200);
    }
#endif
#if 0
    // This is motor test code to verify things run properly
    _delay_ms(400);
    ShiftOut.bit.motor0 = FORWARD;

    updateShift();

    _delay_ms(400);
    ShiftOut.bit.motor0 = OFF;
    ShiftOut.bit.motor1 = FORWARD;
    updateShift();

    _delay_ms(400);
    ShiftOut.bit.motor1 = OFF;
    ShiftOut.bit.motor2 = FORWARD;
    updateShift();

    _delay_ms(400);
    ShiftOut.bit.motor2 = OFF;
    ShiftOut.bit.motor3 = FORWARD;
    updateShift();

    _delay_ms(400);
    ShiftOut.bit.motor3 = OFF;
    ShiftOut.bit.motor4 = FORWARD;
    updateShift();

    _delay_ms(400);
    // Turn off all motors
    ShiftOut.bit.motor0 = OFF;
    ShiftOut.bit.motor1 = OFF;
    ShiftOut.bit.motor2 = OFF;
    ShiftOut.bit.motor3 = OFF;
    ShiftOut.bit.motor4 = OFF;
    updateShift();
#endif
#if 0
    // This is the dead reconning diving demo for the pool
    // Wait 6 sec
    _delay_ms(600);

    // Forward 6 sec total
    ShiftOut.bit.motor0 = FORWARD;
    ShiftOut.bit.motor1 = FORWARD;
    updateShift();

    _delay_ms(400);

    // Submurge
    ShiftOut.bit.motor2 = BACKWARD;
    ShiftOut.bit.motor3 = BACKWARD;
    updateShift();

    _delay_ms(200);

    // Wait 2 sec
    ShiftOut.bit.motor0 = OFF;
    ShiftOut.bit.motor1 = OFF;
    ShiftOut.bit.motor2 = OFF;
    ShiftOut.bit.motor3 = OFF;
    updateShift();

    _delay_ms(200);

    // Sideways 4 sec
    ShiftOut.bit.motor4 = FORWARD;
    updateShift();

    _delay_ms(400);

    // Wait 2 sec
    ShiftOut.bit.motor4 = OFF;
    updateShift();

    _delay_ms(200);

    // Forward 4 sec
    ShiftOut.bit.motor0 = FORWARD;
    ShiftOut.bit.motor1 = FORWARD;
    updateShift();

    _delay_ms(400);

    // Rotate 4 sec

    ShiftOut.bit.motor1 = BACKWARD;
    updateShift();

    _delay_ms(400);

    // Forward 10 sec total
    ShiftOut.bit.motor1 = FORWARD;
    updateShift();

    _delay_ms(400);

    // Surface
    ShiftOut.bit.motor2 = FORWARD;
    ShiftOut.bit.motor3 = FORWARD;
    updateShift();

    _delay_ms(200);

    ShiftOut.bit.motor2 = OFF;
    ShiftOut.bit.motor3 = OFF;
    updateShift();

    _delay_ms(400);

    // Turn off all motors
    ShiftOut.bit.motor0 = OFF;
    ShiftOut.bit.motor1 = OFF;
    ShiftOut.bit.motor2 = OFF;
    ShiftOut.bit.motor3 = OFF;
    ShiftOut.bit.motor4 = OFF;
    
    updateShift();
#endif
}
#endif

/*******************************************************************************
* Main Includes File
*******************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "spi.h"
#include "uart.h"
#include "adc.h"

#define FORWARD  0x01
#define BACKWARD 0x03
#define ON       0x01
#define OFF      0x00

#define UART_FOSC 16000000
#define UART_BAUD 9600
#define UART_BAUD_CODE (UART_FOSC/(16/(UART_BAUD-1)))

typedef struct {
    uint8_t motor0  : 2;
    uint8_t motor1  : 2;
    uint8_t motor2  : 2;
    uint8_t motor3  : 2;
    uint8_t motor4  : 2;
    uint8_t switch0 : 1;
    uint8_t switch1 : 1;
    uint8_t switch2 : 1;
    uint8_t switch3 : 1;
    uint8_t switch4 : 1;
    uint8_t switch5 : 1;
} ShiftRegister16Bit;
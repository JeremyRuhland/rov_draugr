/*******************************************************************************
* Main Includes File                                                           *
*                                                                              *
* Program setting defines:                                                     *
* Uncomments these lines to enable features. If motors run backwards, reverse  *
* them here. If shift register receives data in reverse order, enable          *
* bitfield order reverse mode.                                                 *
*                                                                              *
*******************************************************************************/

// #define MOTOR0_REVERSE
// #define MOTOR1_REVERSE
// #define MOTOR2_REVERSE
// #define MOTOR3_REVERSE
// #define MOTOR4_REVERSE

// #define BITFIELD_ORDER_REVERSE

/*****************************************************************************/

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
#define TRUE     0x01
#define FALSE    0x00

#ifdef BITFIELD_ORDER_REVERSE
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
} bitfield;
#else
typedef struct {
    uint8_t switch5 : 1;
    uint8_t switch4 : 1;
    uint8_t switch3 : 1;
    uint8_t switch2 : 1;
    uint8_t switch1 : 1;
    uint8_t switch0 : 1;
    uint8_t motor4  : 2;
    uint8_t motor3  : 2;
    uint8_t motor2  : 2;
    uint8_t motor1  : 2;
    uint8_t motor0  : 2;
} bitfield;
#endif

typedef union ShiftRegister16Bit {
    uint16_t uart;
    bitfield bit;
} ShiftRegister16Bit;


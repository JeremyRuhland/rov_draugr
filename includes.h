/*******************************************************************************
* Main Includes File                                                           *
*                                                                              *
* Program setting defines:                                                     *
* Uncomments these lines to enable features. If motors run backwards, reverse  *
* them here. If shift register receives data in reverse order, enable          *
* bitfield order reverse mode.                                                 *
* Uncommenting text debug mode will cause the ROV to print serial strings      *
* describing its current state.                                                *
*                                                                              *
*******************************************************************************/

// #define MOTOR0_REVERSE
// #define MOTOR1_REVERSE
// #define MOTOR2_REVERSE
// #define MOTOR3_REVERSE
// #define MOTOR4_REVERSE

// #define BITFIELD_ORDER_REVERSE

#define TEXT_DEBUG

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

#define CASE_MOTOR_0_STOP     (uint8_t) '`'
#define CASE_MOTOR_0_FORWARD  (uint8_t) '1'
#define CASE_MOTOR_0_BACKWARD (uint8_t) '2'
#define CASE_MOTOR_1_STOP     (uint8_t) '3'
#define CASE_MOTOR_1_FORWARD  (uint8_t) '4'
#define CASE_MOTOR_1_BACKWARD (uint8_t) '5'
#define CASE_MOTOR_2_STOP     (uint8_t) '6'
#define CASE_MOTOR_2_FORWARD  (uint8_t) '7'
#define CASE_MOTOR_2_BACKWARD (uint8_t) '8'
#define CASE_MOTOR_3_STOP     (uint8_t) '9'
#define CASE_MOTOR_3_FORWARD  (uint8_t) '0'
#define CASE_MOTOR_3_BACKWARD (uint8_t) '-'
#define CASE_MOTOR_4_STOP     (uint8_t) '='
#define CASE_MOTOR_4_FORWARD  (uint8_t) 'q'
#define CASE_MOTOR_4_BACKWARD (uint8_t) 'w'
#define CASE_ALL_MOTORS_STOP  (uint8_t) 'e'
#define CASE_TOGGLE_SW_0      (uint8_t) 'r'
#define CASE_TOGGLE_SW_1      (uint8_t) 't'
#define CASE_TOGGLE_SW_2      (uint8_t) 'y'
#define CASE_TOGGLE_SW_3      (uint8_t) 'u'
#define CASE_TOGGLE_SW_4      (uint8_t) 'i'
#define CASE_TOGGLE_SW_5      (uint8_t) 'o'
#define CASE_ALL_SWITCHES_OFF (uint8_t) 'p'
#define CASE_MOTOR_STATUS     (uint8_t) '['
#define CASE_TEMPS            (uint8_t) ']'
#define CASE_CURRENTS         (uint8_t) '\\'
#define CASE_CONDUCTIVITY     (uint8_t) 'a'
#define CASE_SONAR_DISTANCE   (uint8_t) 's'
#define CASE_COMPASS          (uint8_t) 'd'

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


/*******************************************************************************
* ROV Waterside firmware
* 2014 WWU Marine Technology Club
* http://wwumarinetech.org/
* Jeremy Ruhland
*
*******************************************************************************/

#include <avr/io.h>
#include <util/delay.h>

void initGpio(void);
void initUart(void);
void initAdc(void);

int main(void) {
    initGpio();
    initUart();
    initAdc();
}

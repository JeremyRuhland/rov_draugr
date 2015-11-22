#include "includes.h"

void adcInit(void) {
    ADMUX = (1<<REFS0); // AVcc reference

    // Enable adc, enable interrupt, 128 prescaler
    ADCSRA = ((1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));

    // Begin first conversion
    ADCSRA |= (1<<ADSC);
}

uint16_t adcFormatVoltage(uint16_t v) {
    return (((v*50)/1024)*100);
}

int16_t adcFormatTemp(uint16_t t) {
    return (((int16_t) (adcFormatVoltage(t))-500)/10);
}

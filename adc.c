#include "includes.h"

void adcInit(void) {
    ADMUX = (1<<REFS0); // AVcc reference

    // Enable adc, enable interrupt, 128 prescaler
    ADCSRA = ((1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
}
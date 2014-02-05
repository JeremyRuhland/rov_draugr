#include "includes.h"

void adcInit(void) {
    ADMUX = (1<<REFS0); // AVcc reference

    // Enable adc, enable interrupt, 128 prescaler
    ADCSRA = ((1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
}

ISR(ADC_vect) {
    static uint8_t adcMux = 0;

    if (adcMux <= 4) { // Motor 0-4
        AnalogValues[adcMux] = (ADCH | ADCL);

        adcMux++;

        ADMUX = ((ADMUX & ~(0x0F)) | adcMux); // Switch to next mux

        // Start new conversion
    } else if (adcMux == 5) { // Skip EN1
        adcMux++;

        // Start new conversion
    } else if (adcMux <= 7) { // Temp 0-1
        AnalogValues[adcMux+1] = (ADCH | ADCL);

        adcMux++;

        ADMUX = ((ADMUX & ~(0x0F)) | (adcMux+1)); // Switch to next mux

        // Start new conversion
    } else { // Internal Temp
        AnalogValues[adcMux+1] = (ADCH | ADCL);

        adcMux = 0; // Reset cycle

        ADMUX = ((ADMUX & ~(0x0F)) | (adcMux+1)); // Switch to next mux

        // Start new conversion
    }
}

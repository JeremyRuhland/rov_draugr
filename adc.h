extern uint16_t AnalogValues;

extern void adcInit(void);
extern uint16_t adcFormatTemp(uint16_t value);

#define adcFormatCurrent(x) (uint16_t) ((uint16_t) x*50)/1024)*100)
//#define adcFormatTemp(x) (uint16_t) (adcFormatCurrent(x)-50) //  = 10/(voltage-500mV)

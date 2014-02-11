/******************************************************************************
* Analog to Digital Conversion Module                                         *
*                                                                             *
* Contains functions and definitions required for 2014 ROV motherboard        *
*                                                                             *
* (void) adcInit(void)                     Initializes ADC to 10-bit single   *
*                                          shot mode, external AVcc,          *
*                                          interrupt on conversion completion *
*                                          Function must be called before     *
*                                          following functions may be called. *
* (uint16_t) adcFormatVoltage(uint16_t)    Function to convert raw ADC        *
*                                          byte value to unsigned 16-bit      *
*                                          millivolts.                        *
* (int16_t) adcFormatTemp(uint16_t)        Function to convert raw Adc        *
*                                          byte value to signed 16-bit        *
*                                          centigrade.                        *
* (uint16_t) adcResult(void)               Macro define to accumulate high    *
*                                          and low ADC result register.       *
*                                          Returns unsigned 16-bit value.     *
*                                                                             *
******************************************************************************/

extern void adcInit(void);
extern uint16_t adcFormatVoltage(uint16_t v);
extern int16_t adcFormatTemp(uint16_t t);

#define adcResult() ((uint16_t) ((((uint16_t) ADCH)<<8) | ((uint16_t) ADCL)))

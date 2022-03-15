#ifndef CUSTOM_PINS_CPP
#define CUSTOM_PINS_CPP __DATE__ ", " __TIME__

#include "Custom_pins.h"

#include "wiring_analog.c"

namespace yh {
    namespace rec {
        namespace custom_pins {
            namespace {
                static volatile uint16_t adc_data [NUM_ANALOG_INPUTS];
            }
        }
    }
}

void yh::rec::custom_pins::begin_analog_read () {
    // ADEN: ADC enable
	// ADSC: ADC start new conversion
    // ADIF: ADC interrupt flag
    // ADIE: ADC interrupt enable
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t yh::rec::custom_pins::analog_read (uint8_t pin_or_channel) {
    if (pin_or_channel >= A0) pin_or_channel -= A0;
    uint8_t oldSREG = SREG;
    noInterrupts();
    const uint16_t temp = adc_data[pin_or_channel];
    SREG = oldSREG;
    return temp;
}

ISR (ADC_vect) {
    static uint8_t channel = 0;
    #ifdef ADCL
	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
    yh::rec::custom_pins::adc_data[channel++] = ADCL | (ADCH << 8);
    if (channel >= NUM_ANALOG_INPUTS) channel = 0;
    #endif
    #if defined(ADCSRB) && defined(MUX5)
	// the MUX5 bit of ADCSRB selects whether we're reading from channels
	// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((channel >> 3) & 0x01) << MUX5);
    #endif
  
	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 (the default).
    #if defined(ADMUX)
        #if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	ADMUX = (analog_reference << 4) | (channel & 0x07);
        #else
	ADMUX = (analog_reference << 6) | (channel & 0x07);
        #endif
    #endif
	// start new conversion
    ADCSRA |= (1 << ADSC);

}

#endif // #ifndef CUSTOM_PINS_CPP
#ifndef CUSTOM_PINS_CPP
#define CUSTOM_PINS_CPP __DATE__ ", " __TIME__

#if 1

#include "Custom_pins.h"

#include "wiring_analog.c"

namespace yh {
    namespace rec {
        namespace custom_pins {
            namespace {
                #if defined(__AVR_ATmega32U4__)
                static volatile uint16_t adc_data [16];
                #else
                static volatile uint16_t adc_data [NUM_ANALOG_INPUTS];
                #endif
            }
        }
        uint8_t Custom_pin::auto_object_counter = 0; // initialize the counter to 0
        Custom_pin pins [NUM_DIGITAL_PINS];
    }
}

void select_adc_channel (const uint8_t channel) __attribute__((__always_inline__));
void select_adc_channel (const uint8_t channel) {
    ADMUX = (ADMUX & ~((1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0))) | ((channel & 0b111) << MUX0);
#if defined(ADCSRB) && defined(MUX5)
	// the MUX5 bit of ADCSRB selects whether we're reading from channels
	// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((channel >> 3) & 0x01) << MUX5);
#endif
}

void begin_adc () __attribute__((__always_inline__));
void begin_adc () {
    ADCSRA |= (1 << ADIF); // clear the interrupt flag first
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = (0 << MUX5) | (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
    ADMUX = (0 << REFS1) | (0 << REFS0) | (0 << ADLAR) | (0 << MUX4) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
}

void select_adc_prescaler (const uint8_t prescaler) {
    ADCSRA = (ADCSRA & (~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)))) | ((prescaler & 0b111) << ADPS0);
}

void start_new_adc () __attribute__((__always_inline__));
void start_new_adc () {
    ADCSRA |= (1 << ADSC);
}

void wait_for_adc_complete () __attribute__((__always_inline__));
void wait_for_adc_complete () {
    while (ADCSRA & (1 << ADSC)) {}
}

uint16_t read_adc_result () __attribute__((__always_inline__));
uint16_t read_adc_result () {
    const uint8_t temp = ADCL;
    if (ADMUX & (1 << ADLAR))
        return (temp >> 6) | (ADCH << 2);
    return temp | (ADCH << 8);
}

uint8_t read_8_bit_adc_result () __attribute__((__always_inline__));
uint8_t read_8_bit_adc_result () {
    if (ADMUX & (1 << ADLAR))
        return ADCH;
    const uint8_t temp = ADCL;
    return (temp >> 2) | (ADCH << 6);
}

ISR (ADC_vect) {
    static uint8_t channel = 0;
    yh::rec::custom_pins::adc_data[channel] = read_adc_result();
    channel++;
    if (channel == NUM_ANALOG_INPUTS)
        channel = 0;
    select_adc_channel(channel);
    start_new_adc();
}

uint16_t analog_read (const uint8_t channel) {
    if (!(ADCSRA & (1 << ADEN)))
        begin_adc();
    if ( (ADCSRA & (1 << ADIE)) && (SREG & (1 << SREG_I)) ) {
        uint8_t oldSREG = SREG;
        noInterrupts();
        const uint16_t temp = yh::rec::custom_pins::adc_data[channel];
        SREG = oldSREG;
        return temp;
    }
    #if defined(__AVR_ATmega32U4__)
    select_adc_channel(analogPinToChannel(channel >= NUM_ANALOG_INPUTS ? channel - A0 : channel));
    #else
    select_adc_channel(channel >= NUM_ANALOG_INPUTS ? channel - A0 : channel);
    #endif
    start_new_adc();
    wait_for_adc_complete();
    return read_adc_result();
}

#endif // #if 1

#endif // #ifndef CUSTOM_PINS_CPP
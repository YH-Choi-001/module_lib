#ifndef CUSTOM_PINS_CPP
#define CUSTOM_PINS_CPP __DATE__ ", " __TIME__

#include "Custom_pins.h"

namespace yh {
    namespace rec {
        // namespace custom_pins {
        //     namespace {
        //         #if defined(__AVR_ATmega32U4__)
        //         static volatile uint16_t adc_data [16];
        //         #else
        //         static volatile uint16_t adc_data [NUM_ANALOG_INPUTS];
        //         #endif
        //     }
        // }
        uint8_t Custom_pin::auto_object_counter = 0; // initialize the counter to 0
        Custom_pin pins [NUM_DIGITAL_PINS];
        namespace custom_pins {
            int analog_read (uint8_t pin) {
                #if defined(analogPinToChannel)
                #if defined(__AVR_ATmega32U4__)
                    if (pin >= 18) pin -= 18; // allow for channel or pin numbers
                #endif
                    pin = analogPinToChannel(pin);
                #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
                    if (pin >= 54) pin -= 54; // allow for channel or pin numbers
                #elif defined(__AVR_ATmega32U4__)
                    if (pin >= 18) pin -= 18; // allow for channel or pin numbers
                #elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
                    if (pin >= 24) pin -= 24; // allow for channel or pin numbers
                #else
                    if (pin >= 14) pin -= 14; // allow for channel or pin numbers
                #endif
                select_adc_channel(pin);
                start_new_ad_conversion();
                wait_for_ad_conversion_complete();
                return read_adc_result();
            }
        }
    }
}

#if 0

void begin_adc () __attribute__((__always_inline__));
void begin_adc () {
    ADCSRA |= (1 << ADIF); // clear the interrupt flag first
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = (0 << MUX5) | (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
    ADMUX = (0 << REFS1) | (0 << REFS0) | (0 << ADLAR) | (0 << MUX4) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
}

ISR (ADC_vect) {
    static uint8_t channel = 0;
    yh::rec::custom_pins::adc_data[channel] = yh::rec::custom_pins::read_adc_result();
    channel++;
    if (channel == NUM_ANALOG_INPUTS)
        channel = 0;
    yh::rec::custom_pins::select_adc_channel(channel);
    yh::rec::custom_pins::start_new_ad_conversion();
}

#endif // #if 0

#endif // #ifndef CUSTOM_PINS_CPP
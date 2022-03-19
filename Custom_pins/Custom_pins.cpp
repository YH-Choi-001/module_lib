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
        Custom_pin pins [NUM_DIGITAL_PINS];
        uint8_t Custom_pin::auto_object_counter = 0; // initialize the counter to 0
    }
}

uint16_t yh::rec::custom_pins::analog_read (uint8_t pin_or_channel) {
    static const uint8_t ADIE_mask = (1 << ADIE);
    // Controlling bits in ADCSRA:
        // ADEN: ADC enable
        // ADSC: ADC start new conversion
        // ADIF: ADC interrupt flag
        // ADIE: ADC interrupt enable
        // ADPS == 0b111: ADC prescaler factor = 128
#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
	if (pin_or_channel >= 18) pin_or_channel -= 18; // allow for channel or pin numbers
#endif
	pin_or_channel = analogPinToChannel(pin_or_channel);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (pin_or_channel >= 54) pin_or_channel -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
	if (pin_or_channel >= 18) pin_or_channel -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
	if (pin_or_channel >= 24) pin_or_channel -= 24; // allow for channel or pin numbers
#else
	if (pin_or_channel >= 14) pin_or_channel -= 14; // allow for channel or pin numbers
#endif

    uint8_t oldSREG = SREG;
    noInterrupts();
    if (!(ADCSRA & ADIE_mask)) {
        // tries the best to maintain backwards compatability,
        // so as to prevent using any begin() functions to perform an initial kickoff
        ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // kicks off the first conversion with prescaler 128 (it is by default)
    }
    const uint16_t temp = adc_data[pin_or_channel];
    SREG = oldSREG;
    return temp;
}

uint16_t yh::rec::custom_pins::analog_read_and_wait (uint8_t pin_or_channel) {
    static const uint8_t ADSC_mask = (1 << ADSC);
    // Controlling bits in ADCSRA:
        // ADEN: ADC enable
        // ADSC: ADC start new conversion
        // ADIF: ADC interrupt flag
        // ADIE: ADC interrupt enable
        // ADPS == 0b111: ADC prescaler factor = 128
#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
	if (pin_or_channel >= 18) pin_or_channel -= 18; // allow for channel or pin numbers
#endif
	pin_or_channel = analogPinToChannel(pin_or_channel);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (pin_or_channel >= 54) pin_or_channel -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
	if (pin_or_channel >= 18) pin_or_channel -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
	if (pin_or_channel >= 24) pin_or_channel -= 24; // allow for channel or pin numbers
#else
	if (pin_or_channel >= 14) pin_or_channel -= 14; // allow for channel or pin numbers
#endif

    uint8_t oldSREG = SREG;
    // disable interrupts
    noInterrupts();
    // save previous ADCSRA flags
    uint8_t oldADCSRA = ADCSRA;
    pause_automated_analog_read();

    // select channel
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
    ADCSRA |= ADSC_mask;
    // re-enable interrupts
    SREG = oldSREG;
    while (ADCSRA & ADSC_mask) {} // wait for the conversion (~ 104 us)
    const uint16_t temp = ADCL | (ADCH << 8);
    ADCSRA = oldADCSRA;
    return temp;
}

ISR (ADC_vect) {
    static uint8_t channel = 0;
    #ifdef ADCL
	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
    yh::rec::custom_pins::adc_data[channel] = ADCL | (ADCH << 8);
    const uint8_t current_channel = channel;

    #if defined(__AVR_ATmega32U4__)
    // only channels 0, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 are in use on micro and leonardo
    // 0b 00111111 11110011
    // 0x    3   f    f   3
    yh::rec::custom_pins::analog_read_enable_byte &= 0x3ff3;
    #endif

    do {
        channel++;
        if (channel >= NUM_ANALOG_INPUTS) channel = 0;
        // if (channel == current_channel) break; // prevent it from entering an infinite loop, and prevent it from selecting new channel, since all channels are disabled now
        if (channel == current_channel) {
            if ( yh::rec::custom_pins::analog_read_enable_byte & (0x01 << channel) ) {
                // only this channel is enabled
                // to prevent it from entering an infinite loop, just break
                break;
            } else {
                // all channels are disabled before this ISR is called
                // all operations should be stopped now
                // turn off ADC interrupt enable flag
                ADCSRA &= ~(1 << ADIE);
                return;
            }
        }
    } while ( !( yh::rec::custom_pins::analog_read_enable_byte & (0x01 << channel) ) ); // skip the disabled channels
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
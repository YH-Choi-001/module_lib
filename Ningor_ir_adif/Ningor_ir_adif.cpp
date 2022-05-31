#ifndef NINGOR_IR_ADIF_CPP
#define NINGOR_IR_ADIF_CPP __DATE__ ", " __TIME__

#include "Ningor_ir_adif.h"

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#define ADC_CHANNEL_FOR_IR 12
namespace yh {
    namespace rec {
        namespace ningor_ir_adifs {
            // 7    6    5    4    3    2    1    0
            // x MUX5 MUX2 MUX1 MUX0 log2 log1 log0
            static volatile uint8_t adc_data_counter;
            // buffered data
            volatile uint16_t adc_data [ADC_CHANNEL_FOR_IR][8];

        }
    }
}
void yh::rec::Ningor_ir_adif::begin () {
    ADCSRA |= (1 << ADIF); // clear the interrupt flag first
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = (0 << MUX5) | (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
    ADMUX = (0 << REFS1) | (0 << REFS0) | (0 << ADLAR) | (0 << MUX4) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
}

uint16_t yh::rec::Ningor_ir_adif::read_sun (const uint8_t channel) {
    uint16_t max = yh::rec::ningor_ir_adifs::adc_data[channel][0];
    for (uint8_t i = 1; i < 8; i++) {
        const uint8_t oldSREG = SREG;
        noInterrupts();
        const uint16_t val = yh::rec::ningor_ir_adifs::adc_data[channel][i];
        SREG = oldSREG;
        if (val > max)
            max = val;
    }
    return max;
}

uint16_t yh::rec::Ningor_ir_adif::read_ball (const uint8_t channel) {
    uint16_t
        min = yh::rec::ningor_ir_adifs::adc_data[channel][0],
        max = yh::rec::ningor_ir_adifs::adc_data[channel][0];
    for (uint8_t i = 1; i < 8; i++) {
        const uint8_t oldSREG = SREG;
        noInterrupts();
        const uint16_t val = yh::rec::ningor_ir_adifs::adc_data[channel][i];
        SREG = oldSREG;
        if (val > max)
            max = val;
        if (val < min)
            min = val;
    }
    return max - min;
}

ISR(ADC_vect) {
    const uint8_t temp_adc_data_counter = yh::rec::ningor_ir_adifs::adc_data_counter; // load the volatile value in first, to save time
    const uint8_t temp_adcl = ADCL; // get ADCL first to lock ADC result
    const uint8_t temp_adch = ADCH;
    const uint8_t nex_adc_data_counter = temp_adc_data_counter + 1;
    const uint8_t selected_new_channel = (((nex_adc_data_counter >> 3) & 0b111) << MUX0);
    const uint8_t prev_admux = ADMUX & ((1 << REFS1) | (1 << REFS0) | (1 << ADLAR));
    ADMUX = prev_admux | selected_new_channel;
#if defined(ADCSRB) && defined(MUX5)
    (nex_adc_data_counter & 0x40) ? ADCSRB |= (1 << MUX5) : ADCSRB &= ~(1 << MUX5);
#endif
    yh::rec::ningor_ir_adifs::adc_data[temp_adc_data_counter >> 3][temp_adc_data_counter & 0b111] = temp_adcl | (temp_adch << 8);
    yh::rec::ningor_ir_adifs::adc_data_counter = nex_adc_data_counter;
}

#else // #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#error Ningor_ir_adif library is only for Arduino Mega or Mega 2560
#endif // #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#endif // #ifndef NINGOR_IR_ADIF_CPP
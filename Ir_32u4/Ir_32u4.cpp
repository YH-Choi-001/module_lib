#ifndef IR_32U4_CPP
#define IR_32U4_CPP __DATE__ ", " __TIME__

#include "Ir_32u4.h"

namespace yh {
    namespace rec {
        namespace ir_32u4s {
            uint8_t ball_intensity [IR_CHANNEL];
        }
    }
}

volatile uint8_t raw_data [IR_CHANNEL][8];
volatile uint8_t selecting_idx; // CHANNEL_3 CHANNEL_2 CHANNEL_1 CHANNEL_0 LOOP_2 LOOP_1 LOOP_0 UNUSED_BIT

static const PROGMEM uint8_t channel_selecting_order [12] = {
    0, 1, 4, 5, 6, 7, 8, 10, 11, 12, 13, 9
};

void yh::rec::ir_32u4s::begin () {
    DDRF &= ~(_BV(0) | _BV(1) | _BV(4) | _BV(5) | _BV(6) | _BV(7));
    DDRD &= ~(_BV(4) | _BV(6) | _BV(7));
    DDRB &= ~(_BV(4) | _BV(5) | _BV(6));
    ADCSRA |= (1 << ADIF); // clear the interrupt flag first
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = (0 << MUX5) | (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
    ADMUX = (0 << REFS1) | (0 << REFS0) | (1 << ADLAR) | (0 << MUX4) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
}

void yh::rec::ir_32u4s::update_ball_intensity (uint8_t channel) {
    volatile uint8_t *cmp_ptr = &(raw_data[channel][0]);
    uint8_t max = (*cmp_ptr), min = max;
    for (uint8_t i = 1; i < 8; i++) {
        cmp_ptr++;
        const uint8_t curr_val = (*cmp_ptr);
        if (curr_val > max)
            max = curr_val;
        if (curr_val < min)
            min = curr_val;
    }
    ball_intensity[channel] = max - min;
}

uint8_t yh::rec::ir_32u4s::get_sun_intensity (uint8_t channel) {
    volatile uint8_t *cmp_ptr = &(raw_data[channel][0]);
    uint8_t max = (*cmp_ptr);
    for (uint8_t i = 1; i < 8; i++) {
        cmp_ptr++;
        const uint8_t curr_val = (*cmp_ptr);
        if (curr_val > max)
            max = curr_val;
    }
    return max;
}

ISR(ADC_vect) {
    #define SWAP(x) { __asm__ __volatile__ ("swap %0\n\t" :"+r"(x) :); } // for <<4 or >>4 operations
    uint8_t temp_selecting_idx = selecting_idx; // load the volatile variable to register
    (   *( &(raw_data[0][0]) + (temp_selecting_idx >> 1) )   ) = ADCH;
    temp_selecting_idx += static_cast<uint8_t>(1 << 1);
    if ((temp_selecting_idx & static_cast<uint8_t>(0x07 << 1)) == 0) {
        // select new channel
        if (temp_selecting_idx >= (IR_CHANNEL << 4)) {
            temp_selecting_idx = 0;
        }
        uint8_t temp_chan = temp_selecting_idx;
        SWAP(temp_chan);
        temp_chan &= 0x0f;
        const uint8_t selected_channel = pgm_read_byte_near(channel_selecting_order + temp_chan);
        ADMUX = (ADMUX & ~((1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0))) | (static_cast<uint8_t>(selected_channel & 0x07) << MUX0);
        static_cast<uint8_t>(selected_channel & 0x08) ? (ADCSRB = (1 << MUX5)) : (ADCSRB = 0);
    }
    // ADCSRA |= (1 << ADSC); // start new A/D conversion
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // start new A/D conversion
    selecting_idx = temp_selecting_idx;
}

#endif // #ifndef IR_32U4_CPP
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
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);
    pinMode(A8, INPUT);
    pinMode(A9, INPUT);
    pinMode(A10, INPUT);
    pinMode(A11, INPUT);
    ADCSRA |= (1 << ADIF); // clear the interrupt flag first
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = (0 << MUX5) | (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
    ADMUX = (0 << REFS1) | (0 << REFS0) | (0 << ADLAR) | (0 << MUX4) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
}

uint16_t yh::rec::Ningor_ir_adif::read_sun (const uint8_t channel) {
    const uint8_t oldSREG = SREG;
    noInterrupts();
    uint16_t max = yh::rec::ningor_ir_adifs::adc_data[channel][0];
    SREG = oldSREG;
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
    const uint8_t oldSREG = SREG;
    noInterrupts();
    uint16_t
        min = yh::rec::ningor_ir_adifs::adc_data[channel][0],
        max = yh::rec::ningor_ir_adifs::adc_data[channel][0];
    SREG = oldSREG;
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

uint16_t yh::rec::Ningor_ir_adif::get_ball_dir () {
    uint16_t balls [ADC_CHANNEL_FOR_IR];
    uint8_t max_eye = 0;
    for (uint8_t i = 0; i < ADC_CHANNEL_FOR_IR; i++) {
        balls[i] = read_ball(i);
        if (balls[i] > balls[max_eye])
            max_eye = i;
    }
    const int16_t
        l_eye = balls[max_eye ? (ADC_CHANNEL_FOR_IR - 1) : (max_eye - 1)],
        r_eye = balls[((ADC_CHANNEL_FOR_IR - 1) == max_eye) ? 0 : (max_eye + 1)];
    int16_t angle =
        static_cast<uint8_t>(360 / ADC_CHANNEL_FOR_IR) * max_eye + static_cast<int8_t>(360 / ADC_CHANNEL_FOR_IR / 2) * (r_eye - l_eye) / (balls[max_eye] - (l_eye < r_eye ? l_eye : r_eye));
    if (angle < 0)
        angle += 360;
    return angle;
}

uint16_t yh::rec::Ningor_ir_adif::get_ball_mag () {
    uint16_t balls [ADC_CHANNEL_FOR_IR];
    uint8_t max_eye = 0;
    uint16_t max_val = 0;
    for (uint8_t i = 0; i < ADC_CHANNEL_FOR_IR; i++) {
        const uint16_t temp_val = read_ball(i);
        balls[i] = temp_val;
        if (temp_val > max_val)
            max_eye = i;
            max_val = temp_val;
    }
    const uint16_t
        l_eye = balls[max_eye ? (ADC_CHANNEL_FOR_IR - 1) : (max_eye - 1)],
        r_eye = balls[((ADC_CHANNEL_FOR_IR - 1) == max_eye) ? 0 : (max_eye + 1)];
    // const uint16_t delta = balls[max_eye] - smaller_eye;
    // return
    //     balls[max_eye]
    //     + l_eye * (l_eye - smaller_eye) / delta
    //     + r_eye * (r_eye - smaller_eye) / delta;
    if (l_eye < r_eye) {
        return balls[max_eye] + r_eye * (r_eye - l_eye) / (balls[max_eye] - l_eye);
    } else {
        return balls[max_eye] + l_eye * (l_eye - r_eye) / (balls[max_eye] - r_eye);
    }
}

ISR(ADC_vect) {
    const uint8_t temp_adc_data_counter = yh::rec::ningor_ir_adifs::adc_data_counter; // load the volatile value in first, to save time
    const uint8_t temp_adcl = ADCL; // get ADCL first to lock ADC result
    const uint8_t temp_adch = ADCH;
    const uint8_t nex_adc_data_counter = (temp_adc_data_counter >= 0b1100000) ? 0 : temp_adc_data_counter + 1;
    const uint8_t selected_new_channel = (((nex_adc_data_counter >> 3) & 0b111) << MUX0);
    const uint8_t prev_admux = ADMUX & ((1 << REFS1) | (1 << REFS0) | (1 << ADLAR));
    ADMUX = prev_admux | selected_new_channel;
#if defined(ADCSRB) && defined(MUX5)
    (nex_adc_data_counter & 0x40) ? ADCSRB |= (1 << MUX5) : ADCSRB &= ~(1 << MUX5);
#endif
    ADCSRA |= (1 << ADSC); // start new conversion
    yh::rec::ningor_ir_adifs::adc_data[temp_adc_data_counter >> 3][temp_adc_data_counter & 0b111] = temp_adcl | (temp_adch << 8);
    yh::rec::ningor_ir_adifs::adc_data_counter = nex_adc_data_counter;
}

#else // #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#error Ningor_ir_adif library is only for Arduino Mega or Mega 2560
#endif // #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#endif // #ifndef NINGOR_IR_ADIF_CPP
// This library is proved to be working on 15 Mar 2022 at 13:37

#ifndef HC_SR04_CPP
#define HC_SR04_CPP __DATE__ ", " __TIME__

#include "Hc_sr04.h"

yh::rec::Hc_sr04::Hc_sr04 (const Hc_sr04 &init_obj) :
    trig_pin(init_obj.trig_pin), echo_pin(init_obj.echo_pin), max_waiting_time_in_us(23530)
{
    //
}

yh::rec::Hc_sr04::Hc_sr04 (const uint8_t init_trig_pin, const uint8_t init_echo_pin) :
    trig_pin(init_trig_pin), echo_pin(init_echo_pin), max_waiting_time_in_us(23530)
{
    //
}

void yh::rec::Hc_sr04::begin () {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    trig_pin_output_register = portOutputRegister(digitalPinToPort(trig_pin));
    echo_pin_input_register = portInputRegister(digitalPinToPort(echo_pin));
    trig_pin_mask = digitalPinToBitMask(trig_pin);
    echo_pin_mask = digitalPinToBitMask(echo_pin);
}


void yh::rec::Hc_sr04::set_max_waiting_time_in_us (const unsigned long assign_max_waiting_time_in_us) {
    max_waiting_time_in_us = assign_max_waiting_time_in_us > 23530 ? 23530 : assign_max_waiting_time_in_us; // 23529.41 us for detecting 400 cm obstacle
}

void yh::rec::Hc_sr04::set_max_range_in_mm (const double max_range_in_mm) {
    max_waiting_time_in_us = (max_range_in_mm > 4000.0 ? 4000.0 : max_range_in_mm) * 5.8823; // it takes 5.8823 us to detect a 1 mm obstable
}

void yh::rec::Hc_sr04::set_max_range_in_cm (const double max_range_in_cm) {
    max_waiting_time_in_us = (max_range_in_cm > 400.0 ? 400.0 : max_range_in_cm) * 58.823; // it takes 58.823 us to detect a 1 cm obstable
}

#define TRIG_WAVE() { \
    (*trig_pin_output_register) &= ~trig_pin_mask; \
    delayMicroseconds(2); \
    (*trig_pin_output_register) |= trig_pin_mask; \
    delayMicroseconds(10); \
    (*trig_pin_output_register) &= ~trig_pin_mask; \
} \

#define CUSTOM_PULSEIN() \
    const unsigned long width = \
        countPulseASM(echo_pin_input_register, echo_pin_mask, echo_pin_mask, \
            microsecondsToClockCycles(max_waiting_time_in_us) / 16);

uint16_t yh::rec::Hc_sr04::read_dist_mm () {
    TRIG_WAVE();
    CUSTOM_PULSEIN();
    return width ? (clockCyclesToMicroseconds(width * 16 + 16) * 0.17) : 8888;
    // const unsigned long duration = pulseIn(echo_pin, HIGH, (max_waiting_time_in_us > 23530 ? 23530 : max_waiting_time_in_us));
    // return duration ? duration * 0.17 : 8888;
}

uint16_t yh::rec::Hc_sr04::read_dist_cm () {
    TRIG_WAVE();
    CUSTOM_PULSEIN();
    return width ? (clockCyclesToMicroseconds(width * 16 + 16) * 0.017) : 888;
    // const unsigned long duration = pulseIn(echo_pin, HIGH, (max_waiting_time_in_us > 23530 ? 23530 : max_waiting_time_in_us));
    // return duration ? duration * 0.017 : 888;
}

#undef CUSTOM_PULSEIN
#undef TRIG_WAVE



yh::rec::Hc_sr04_timer_int::Hc_sr04_timer_int (const Hc_sr04_timer_int &init_obj) :
    trig_pin(init_obj.trig_pin), echo_pin(init_obj.echo_pin), ending_tick(0), prev_dist_read(0)
{
    //
}

yh::rec::Hc_sr04_timer_int::Hc_sr04_timer_int (const uint8_t init_trig_pin, const uint8_t init_echo_pin) :
    trig_pin(init_trig_pin), echo_pin(init_echo_pin), ending_tick(0), prev_dist_read(0)
{
    //
}

void yh::rec::Hc_sr04_timer_int::begin () {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    trig_pin_output_register = portOutputRegister(digitalPinToPort(trig_pin));
    echo_pin_input_register = portInputRegister(digitalPinToPort(echo_pin));
    trig_pin_mask = digitalPinToBitMask(trig_pin);
    echo_pin_mask = digitalPinToBitMask(echo_pin);

    // initial kick-off
    (*trig_pin_output_register) &= ~trig_pin_mask;
    delayMicroseconds(2);
    (*trig_pin_output_register) |= trig_pin_mask;
    delayMicroseconds(10);
    (*trig_pin_output_register) &= ~trig_pin_mask;
    // wait
    while ((*echo_pin_input_register) & echo_pin_mask) {} // while HIGH
    while (!((*echo_pin_input_register) & echo_pin_mask)) {} // while LOW // this is the key to success
    uint8_t oldSREG = SREG;
    noInterrupts();
    current_tick = 0;
    ending_tick = 0;
    SREG = oldSREG;
}



yh::rec::Hc_sr04_ext_int::Hc_sr04_ext_int (const Hc_sr04_ext_int &init_obj) :
    trig_pin(init_obj.trig_pin), echo_pin(init_obj.echo_pin), starting_time(0), ending_time(0), prev_dist_read(0)
{
    //
}

yh::rec::Hc_sr04_ext_int::Hc_sr04_ext_int (const uint8_t init_trig_pin, const uint8_t init_echo_pin) :
    trig_pin(init_trig_pin), echo_pin(init_echo_pin), starting_time(0), ending_time(0), prev_dist_read(0)
{
    //
}


void yh::rec::Hc_sr04_ext_int::begin () {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    trig_pin_output_register = portOutputRegister(digitalPinToPort(trig_pin));
    echo_pin_input_register = portInputRegister(digitalPinToPort(echo_pin));
    trig_pin_mask = digitalPinToBitMask(trig_pin);
    echo_pin_mask = digitalPinToBitMask(echo_pin);

    // initial kick-off
    (*trig_pin_output_register) &= ~trig_pin_mask;
    delayMicroseconds(2);
    (*trig_pin_output_register) |= trig_pin_mask;
    delayMicroseconds(10);
    (*trig_pin_output_register) &= ~trig_pin_mask;
    // wait
    while ((*echo_pin_input_register) & echo_pin_mask) {} // while HIGH
    while (!((*echo_pin_input_register) & echo_pin_mask)) {} // while LOW // this is the key to success
    uint8_t oldSREG = SREG;
    noInterrupts();
    starting_time = micros();
    ending_time = 0;
    SREG = oldSREG;
}

#endif //#ifndef HC_SR04_CPP
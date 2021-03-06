#ifdef HC_SR04_CPP

// This library is proved to be working on 15 Mar 2022 at 13:37

#ifndef HC_SR04_UNIVERSAL_CPP
#define HC_SR04_UNIVERSAL_CPP __DATE__ ", " __TIME__

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

uint16_t yh::rec::Hc_sr04::read_dist_mm () {
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    const unsigned long duration = pulseIn(echo_pin, HIGH, max_waiting_time_in_us);
    return duration ? duration * 0.17 : 8888;
}

uint16_t yh::rec::Hc_sr04::read_dist_cm () {
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    const unsigned long duration = pulseIn(echo_pin, HIGH, max_waiting_time_in_us);
    return duration ? duration * 0.017 : 888;
}



yh::rec::Hc_sr04_timer_int::Hc_sr04_timer_int (const Hc_sr04_timer_int &init_obj) :
    trig_pin(init_obj.trig_pin), echo_pin(init_obj.echo_pin), waiting_for_echo_rise(0), measurement_ended(0), prev_dist_read(0)
{
    //
}

yh::rec::Hc_sr04_timer_int::Hc_sr04_timer_int (const uint8_t init_trig_pin, const uint8_t init_echo_pin) :
    trig_pin(init_trig_pin), echo_pin(init_echo_pin), waiting_for_echo_rise(0), measurement_ended(0), prev_dist_read(0)
{
    //
}

void yh::rec::Hc_sr04_timer_int::begin () {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);

    // initial kick-off
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    // wait
    waiting_for_echo_rise = true;
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

    // initial kick-off
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    uint8_t oldSREG = SREG;
    noInterrupts();
    starting_time = 0;
    SREG = oldSREG;
}

#endif // #ifndef HC_SR04_UNIVERSAL_CPP

#endif // #ifdef HC_SR04_CPP
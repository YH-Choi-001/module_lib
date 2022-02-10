// This library is proved to be working on 6 Feb 2022 at 18:36

#ifndef HC_SR04_CPP
#define HC_SR04_CPP __DATE__ ", " __TIME__

#include "Hc_sr04.h"

#define CONFIG_MASK_N_PORTS() \
    trig_pin_mask=digitalPinToBitMask(trig_pin);trig_pin_output_register=portOutputRegister(digitalPinToPort(trig_pin)); \
    echo_pin_mask=digitalPinToBitMask(echo_pin);echo_pin_input_register=portInputRegister(digitalPinToPort(echo_pin));

yh::rec::Hc_sr04_fast::Hc_sr04_fast (const Hc_sr04_fast &init_obj) :
    trig_pin(init_obj.trig_pin), echo_pin(init_obj.echo_pin)
{
    CONFIG_MASK_N_PORTS()
}

yh::rec::Hc_sr04_fast::Hc_sr04_fast (const uint16_t init_trig_and_echo_pin) :
    trig_pin(init_trig_and_echo_pin >> 8), echo_pin(init_trig_and_echo_pin & 0xff)
{
    CONFIG_MASK_N_PORTS()
}

yh::rec::Hc_sr04_fast::Hc_sr04_fast (const uint8_t init_trig_pin, const uint8_t init_echo_pin) :
    trig_pin(init_trig_pin), echo_pin(init_echo_pin)
{
    CONFIG_MASK_N_PORTS()
}

#undef CONFIG_MASK_N_PORTS

void yh::rec::Hc_sr04_fast::begin () {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
}

inline void yh::rec::Hc_sr04_fast::trig_wave () {
    (*trig_pin_output_register) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    // send the pulse
    (*trig_pin_output_register) |= trig_pin_mask;  // this line replaces digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    (*trig_pin_output_register) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);
}

uint16_t yh::rec::Hc_sr04_fast::read_dist_mm (const unsigned long limiting_time_in_us) {
    trig_wave();
    const unsigned long width =
        countPulseASM(echo_pin_input_register, echo_pin_mask, echo_pin_mask,
            microsecondsToClockCycles( (limiting_time_in_us > 23530 ? 23530 : limiting_time_in_us) ) / 16);
    return width ? (clockCyclesToMicroseconds(width * 16 + 16) * 0.17) : 8888;
    // const unsigned long duration = pulseIn(echo_pin, HIGH, (limiting_time_in_us > 23530 ? 23530 : limiting_time_in_us));
    // return duration ? duration * 0.17 : 8888;
}

uint16_t yh::rec::Hc_sr04_fast::read_dist_cm (const unsigned long limiting_time_in_us) {
    trig_wave();
    const unsigned long width =
        countPulseASM(echo_pin_input_register, echo_pin_mask, echo_pin_mask,
            microsecondsToClockCycles( (limiting_time_in_us > 23530 ? 23530 : limiting_time_in_us) ) / 16);
    return width ? (clockCyclesToMicroseconds(width * 16 + 16) * 0.017) : 888;
    // const unsigned long duration = pulseIn(echo_pin, HIGH, (limiting_time_in_us > 23530 ? 23530 : limiting_time_in_us));
    // return duration ? duration * 0.017 : 888;
}

#define CONFIG_MASK_N_PORTS() \
    trig_pin_mask=digitalPinToBitMask(trig_pin);trig_pin_output_register=portOutputRegister(digitalPinToPort(trig_pin)); \
    echo_pin_mask=digitalPinToBitMask(echo_pin);echo_pin_input_register=portInputRegister(digitalPinToPort(echo_pin));

yh::rec::Hc_sr04::Hc_sr04 (const Hc_sr04 &init_obj) :
    Hc_sr04_fast(init_obj.trig_pin, init_obj.echo_pin)
{
    CONFIG_MASK_N_PORTS()
}

yh::rec::Hc_sr04::Hc_sr04 (const uint16_t init_trig_and_echo_pin) :
    Hc_sr04_fast(init_trig_and_echo_pin)
{
    CONFIG_MASK_N_PORTS()
}

yh::rec::Hc_sr04::Hc_sr04 (const uint8_t init_trig_pin, const uint8_t init_echo_pin) :
    Hc_sr04_fast(init_trig_pin, init_echo_pin)
{
    CONFIG_MASK_N_PORTS()
}

#undef CONFIG_MASK_N_PORTS

void yh::rec::Hc_sr04::custom_pulseIn (const unsigned long limiting_time_in_us) {
    // duration_in_us * 0.17 < 4000 mm
    // duration_in_us < 23529.4117
    const unsigned long width =
        countPulseASM(echo_pin_input_register, echo_pin_mask, echo_pin_mask,
            microsecondsToClockCycles( (limiting_time_in_us > 23530 ? 23530 : limiting_time_in_us) ) / 16);
    dist_read_mm = width ? (clockCyclesToMicroseconds(width * 16 + 16) * 0.17) : 8888;
    // const unsigned long duration = pulseIn(echo_pin, HIGH, (limiting_time_in_us > 23530 ? 23530 : limiting_time_in_us));
    // dist_read_mm = duration ? duration * 0.17 : 8888;
}

uint16_t yh::rec::Hc_sr04::read_dist_mm (const unsigned long limiting_time_in_us) {
    trig_wave();
    custom_pulseIn(limiting_time_in_us);
    return dist_read_mm;
}

uint16_t yh::rec::Hc_sr04::previous_dist_mm () {
    return dist_read_mm;
}

uint16_t yh::rec::Hc_sr04::read_dist_cm (const unsigned long limiting_time_in_us) {
    trig_wave();
    custom_pulseIn(limiting_time_in_us);
    return dist_read_mm / 10;
}

uint16_t yh::rec::Hc_sr04::previous_dist_cm () {
    return dist_read_mm / 10;
}

yh::rec::Hc_sr04_timer_int::Hc_sr04_timer_int (const Hc_sr04_timer_int &init_obj) :
    Hc_sr04_fast(init_obj.trig_pin, init_obj.echo_pin), dist_read_cm(0), limiting_time(13000), trig_time_micros(0)
{
    //
}

yh::rec::Hc_sr04_timer_int::Hc_sr04_timer_int (const uint16_t init_trig_and_echo_pin) :
    Hc_sr04_fast(init_trig_and_echo_pin), dist_read_cm(0), limiting_time(13000), trig_time_micros(0)
{
    //
}

yh::rec::Hc_sr04_timer_int::Hc_sr04_timer_int (const uint8_t init_trig_pin, const uint8_t init_echo_pin) :
    Hc_sr04_fast(init_trig_pin, init_echo_pin), dist_read_cm(0), limiting_time(13000), trig_time_micros(0)
{
    //
}

void yh::rec::Hc_sr04_timer_int::begin () {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    trig_wave();
}

void yh::rec::Hc_sr04_timer_int::set_limiting_dist (const uint16_t request_limiting_dist) {
    set_limiting_time(request_limiting_dist/0.017);
}

void yh::rec::Hc_sr04_timer_int::set_limiting_time (const unsigned long request_limiting_time) {
    limiting_time = request_limiting_time < 23530 ? request_limiting_time : 23530;
}

#endif //#ifndef HC_SR04_CPP
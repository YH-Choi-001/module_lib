#ifndef HC_SR04_CPP
#define HC_SR04_CPP __DATE__ ", " __TIME__

#include "Hc_sr04.h"

#define CONFIG_MASK_N_PORTS() \
    trig_pin_mask=digitalPinToBitMask(trig_pin);trig_pin_output_port=portOutputRegister(digitalPinToPort(trig_pin)); \
    echo_pin_mask=digitalPinToBitMask(echo_pin);echo_pin_output_port=portInputRegister(digitalPinToPort(echo_pin));

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

inline void yh::rec::Hc_sr04_fast::begin () {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
}

void yh::rec::Hc_sr04_fast::trig_wave () {
    (*trig_pin_output_port) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    // send the pulse
    (*trig_pin_output_port) |= trig_pin_mask;  // this line replaces digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    (*trig_pin_output_port) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);
}

uint16_t yh::rec::Hc_sr04_fast::read_dist_mm (const unsigned long limiting_time_in_us) {
    trig_wave();
    // duration_in_us * 0.17 < 3036 mm
    // duration_in_us < 17858.823
    unsigned long width =
        countPulseASM(echo_pin_input_port, echo_pin_mask, echo_pin_mask,
            microsecondsToClockCycles( (limiting_time_in_us > 17858 ? 17858 : limiting_time_in_us) ) / 16);
    return width ? (clockCyclesToMicroseconds(width * 16 + 16) * 0.17) : 8888;
}

uint16_t yh::rec::Hc_sr04_fast::read_dist_cm (const unsigned long limiting_time_in_us) {
    trig_wave();
    // duration_in_us * 0.017 < 303.6 cm
    // duration_in_us < 17858.823
    unsigned long width =
        countPulseASM(echo_pin_input_port, echo_pin_mask, echo_pin_mask,
            microsecondsToClockCycles( (limiting_time_in_us > 17858 ? 17858 : limiting_time_in_us) ) / 16);
    return width ? (clockCyclesToMicroseconds(width * 16 + 16) * 0.017) : 8888;
}

#define CONFIG_MASK_N_PORTS() \
    trig_pin_mask=digitalPinToBitMask(trig_pin);trig_pin_output_port=portOutputRegister(digitalPinToPort(trig_pin)); \
    echo_pin_mask=digitalPinToBitMask(echo_pin);echo_pin_output_port=portInputRegister(digitalPinToPort(echo_pin));

yh::rec::Hc_sr04::Hc_sr04 (const Hc_sr04 &init_obj) :
    trig_pin(init_obj.trig_pin), echo_pin(init_obj.echo_pin)
{
    CONFIG_MASK_N_PORTS()
}

yh::rec::Hc_sr04::Hc_sr04 (const uint16_t init_trig_and_echo_pin) :
    trig_pin(init_trig_and_echo_pin >> 8), echo_pin(init_trig_and_echo_pin & 0xff)
{
    CONFIG_MASK_N_PORTS()
}

yh::rec::Hc_sr04::Hc_sr04 (const uint8_t init_trig_pin, const uint8_t init_echo_pin) :
    trig_pin(init_trig_pin), echo_pin(init_echo_pin)
{
    CONFIG_MASK_N_PORTS()
}

#undef CONFIG_MASK_N_PORTS

void yh::rec::Hc_sr04::custom_pulseIn (const unsigned long limiting_time_in_us) {
    // duration_in_us * 0.17 < 3036 mm
    // duration_in_us < 17858.823
    unsigned long width =
        countPulseASM(echo_pin_input_port, echo_pin_mask, echo_pin_mask,
            microsecondsToClockCycles( (limiting_time_in_us > 17858 ? 17858 : limiting_time_in_us) ) / 16);
    dist_read_mm = width ? (clockCyclesToMicroseconds(width * 16 + 16) * 0.17) : 8888;
}

uint16_t yh::rec::Hc_sr04::read_dist_mm (const unsigned long limiting_time_in_us) {
    trig_wave();
    custom_pulseIn();
    return dist_read_mm;
}

uint16_t yh::rec::Hc_sr04::previous_dist_mm () {
    return dist_read_mm;
}

uint16_t yh::rec::Hc_sr04::read_dist_cm (const unsigned long limiting_time_in_us) {
    trig_wave();
    custom_pulseIn();
    return dist_read_mm / 10;
}

uint16_t yh::rec::Hc_sr04::previous_dist_cm () {
    return dist_read_mm / 10;
}

#endif //#ifndef HC_SR04_CPP
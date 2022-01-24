#ifndef HC_SR04_CPP
#define HC_SR04_CPP __DATE__ ", " __TIME__

#include "Hc_sr04.h"

yh::rec::Hc_sr04::Hc_sr04 (const Hc_sr04 &init_obj) :
    trig_pin(init_obj.trig_pin), echo_pin(init_obj.echo_pin)
{
    trig_pin_mask = (digitalPinToBitMask(trig_pin));
    trig_pin_output_port = (portOutputRegister(digitalPinToPort(trig_pin)));
}

yh::rec::Hc_sr04::Hc_sr04 (const uint16_t init_trig_and_echo_pin) :
    trig_pin(init_trig_and_echo_pin >> 8), echo_pin(init_trig_and_echo_pin & 0xff)
{
    trig_pin_mask = (digitalPinToBitMask(trig_pin));
    trig_pin_output_port = (portOutputRegister(digitalPinToPort(trig_pin)));
}

yh::rec::Hc_sr04::Hc_sr04 (const uint8_t init_trig_pin, const uint8_t init_echo_pin) :
    trig_pin(init_trig_pin), echo_pin(init_echo_pin)
{
    trig_pin_mask = (digitalPinToBitMask(trig_pin));
    trig_pin_output_port = (portOutputRegister(digitalPinToPort(trig_pin)));
}

inline void yh::rec::Hc_sr04::begin () {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
}

void yh::rec::Hc_sr04::trig_wave () {
    // HC-SR04 usage:
    // set trig_pin low for 2 ms
    // set trig_pin high for 10 ms
    // set trig_pin low
    // read the length of high pulse on echo pin with arduino built-in function pulseIn()
    (*trig_pin_output_port) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    // send the pulse
    (*trig_pin_output_port) |= trig_pin_mask;  // this line replaces digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    (*trig_pin_output_port) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);

}

uint16_t yh::rec::Hc_sr04::raw_uts_by_pulseIn (const unsigned long limiting_time_in_us) {
    // HC-SR04 usage:
    // set trig_pin low for 2 ms
    // set trig_pin high for 10 ms
    // set trig_pin low
    // read the length of high pulse on echo pin with arduino built-in function pulseIn()
    // duration_in_us * 0.17 < 3036 mm
    // duration_in_us < 17858.823
    const unsigned long duration_in_us = pulseIn(echo_pin, HIGH, limiting_time_in_us > 17858 ? 17858 : limiting_time_in_us);
    dist_read_mm = (duration_in_us ? duration_in_us * 0.17 : 8888); // duration_in_us* 0.17 is from duration_in_us * 0.034 / 2, then because using mm, * 10
    if (dist_read_mm > 4000) dist_read_mm = 8888;
    return dist_read_mm;
}

uint16_t yh::rec::Hc_sr04::read_dist_mm (const bool refresh, const unsigned long limiting_time_in_us) {
    if (refresh) {
        trig_wave();
        raw_uts_by_pulseIn(limiting_time_in_us);
    }
    return dist_read_mm;
}

uint16_t yh::rec::Hc_sr04::read_dist_cm (const bool refresh, const unsigned long limiting_time_in_us) {
    if (refresh) {
        trig_wave();
        raw_uts_by_pulseIn(limiting_time_in_us);
    }
    return dist_read_mm / 10U;
}

#endif //#ifndef HC_SR04_CPP
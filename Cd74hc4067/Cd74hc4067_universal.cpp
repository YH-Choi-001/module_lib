#ifdef CD74HC4067_CPP

#ifndef CD74HC4067_UNIVERSAL_CPP
#define CD74HC4067_UNIVERSAL_CPP  __DATE__ ", " __TIME__

#include "Cd74hc4067.h"

yh::rec::Cd74hc4067::Cd74hc4067 (const Cd74hc4067 &init_obj) :
    pin_s0(init_obj.pin_s0),
    pin_s1(init_obj.pin_s1),
    pin_s2(init_obj.pin_s2),
    pin_s3(init_obj.pin_s3),
    pin_en(init_obj.pin_en)
{
    //
}

yh::rec::Cd74hc4067::Cd74hc4067
(
    const uint8_t init_pin_s0,
    const uint8_t init_pin_s1,
    const uint8_t init_pin_s2,
    const uint8_t init_pin_s3,
    const uint8_t init_pin_en
) :
    pin_s0(init_pin_s0),
    pin_s1(init_pin_s1),
    pin_s2(init_pin_s2),
    pin_s3(init_pin_s3),
    pin_en(init_pin_en)
{
    //
}

void yh::rec::Cd74hc4067::begin () {
    pinMode(pin_s0, OUTPUT);
    pinMode(pin_s1, OUTPUT);
    pinMode(pin_s2, OUTPUT);
    pinMode(pin_s3, OUTPUT);
    if (pin_en != NC_PINNO) {
        pinMode(pin_en, OUTPUT);
        digitalWrite(pin_en, HIGH); // write en pin to HIGH to disable the single ic
    }
    digitalWrite(pin_s0, LOW);
    digitalWrite(pin_s1, LOW);
    digitalWrite(pin_s2, LOW);
    digitalWrite(pin_s3, LOW);
    if (pin_en != NC_PINNO)
        digitalWrite(pin_en, LOW); // write en pin to LOW to enable the single ic
}

void yh::rec::Cd74hc4067::select_channel (const uint8_t channel) {
    digitalWrite(pin_s3, ((channel & (1 << 3)) ? HIGH : LOW));
    digitalWrite(pin_s2, ((channel & (1 << 2)) ? HIGH : LOW));
    digitalWrite(pin_s1, ((channel & (1 << 1)) ? HIGH : LOW));
    digitalWrite(pin_s0, ((channel & (1 << 0)) ? HIGH : LOW));
}

void yh::rec::Cd74hc4067::enable () {
    if (pin_en != NC_PINNO)
        digitalWrite(pin_en, LOW); // write en pin to LOW to enable the single ic
}

void yh::rec::Cd74hc4067::disable () {
    if (pin_en != NC_PINNO)
    digitalWrite(pin_en, HIGH); // write en pin to HIGH to disable the single ic
}

#endif // #ifndef CD74HC4067_UNIVERSAL_CPP

#endif // #ifdef CD74HC4067_CPP
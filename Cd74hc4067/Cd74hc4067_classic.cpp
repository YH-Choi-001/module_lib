#ifdef CD74HC4067_CPP

#ifndef CD74HC4067_CLASSIC_CPP
#define CD74HC4067_CLASSIC_CPP  __DATE__ ", " __TIME__

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
    if (pin_en != NC_PINNO)
        pinMode(pin_en, OUTPUT);
    pin_s0_out_reg = portOutputRegister(digitalPinToPort(pin_s0));
    pin_s1_out_reg = portOutputRegister(digitalPinToPort(pin_s1));
    pin_s2_out_reg = portOutputRegister(digitalPinToPort(pin_s2));
    pin_s3_out_reg = portOutputRegister(digitalPinToPort(pin_s3));
    if (pin_en == NC_PINNO)
        pin_en_out_reg = portOutputRegister(digitalPinToPort(0));
    else
        pin_en_out_reg = portOutputRegister(digitalPinToPort(pin_en));
    pin_s0_mask = digitalPinToBitMask(pin_s0);
    pin_s1_mask = digitalPinToBitMask(pin_s1);
    pin_s2_mask = digitalPinToBitMask(pin_s2);
    pin_s3_mask = digitalPinToBitMask(pin_s3);
    if (pin_en == NC_PINNO)
        pin_en_mask = 0;
    else
        pin_en_mask = digitalPinToBitMask(pin_en);
    (*pin_en_out_reg) |= pin_en_mask; // write en pin to HIGH to disable the single ic
    (*pin_s0_out_reg) &= (~pin_s0_mask);
    (*pin_s1_out_reg) &= (~pin_s1_mask);
    (*pin_s2_out_reg) &= (~pin_s2_mask);
    (*pin_s3_out_reg) &= (~pin_s3_mask);
    (*pin_en_out_reg) &= (~pin_en_mask); // write en pin to LOW to enable the single ic
}

void yh::rec::Cd74hc4067::select_channel (const uint8_t channel) {
    (channel & (1 << 3)) ? ((*pin_s3_out_reg) |= pin_s3_mask) : ((*pin_s3_out_reg) &= (~pin_s3_mask));
    (channel & (1 << 2)) ? ((*pin_s2_out_reg) |= pin_s2_mask) : ((*pin_s2_out_reg) &= (~pin_s2_mask));
    (channel & (1 << 1)) ? ((*pin_s1_out_reg) |= pin_s1_mask) : ((*pin_s1_out_reg) &= (~pin_s1_mask));
    (channel & (1 << 0)) ? ((*pin_s0_out_reg) |= pin_s0_mask) : ((*pin_s0_out_reg) &= (~pin_s0_mask));
}

void yh::rec::Cd74hc4067::enable () {
    (*pin_en_out_reg) &= (~pin_en_mask); // write en pin to LOW to enable the single ic
}

void yh::rec::Cd74hc4067::disable () {
    (*pin_en_out_reg) |= pin_en_mask; // write en pin to HIGH to disable the single ic
}

#endif // #ifndef CD74HC4067_CLASSIC_CPP

#endif // #ifdef CD74HC4067_CPP
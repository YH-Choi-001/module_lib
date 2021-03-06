// This library is proved to be working on 5 Feb 2022 at 18:40

#ifndef US_016_CPP
#define US_016_CPP __DATE__ ", " __TIME__

#include "Us_016.h"

yh::rec::Us_016::Us_016 (const Us_016 &init_obj) :
    range_pin(init_obj.range_pin),
    out_pin(init_obj.out_pin),
    is_resolution_3mm(init_obj.is_resolution_3mm)
{
    //
}

yh::rec::Us_016::Us_016 (const uint8_t init_range_pin, const uint8_t init_out_pin, const uint8_t init_resolution) :
    range_pin(init_range_pin), out_pin(init_out_pin), is_resolution_3mm(init_resolution != 1)
{
    //
}

void yh::rec::Us_016::begin () {
    if (range_pin == NC_PINNO) {
        range_pin_mask = 0;
        range_pin_output_port = (portOutputRegister(digitalPinToPort(0)));
    } else {
        range_pin_mask = (digitalPinToBitMask(range_pin));
        range_pin_output_port = (portOutputRegister(digitalPinToPort(range_pin)));
    }
    pinMode(range_pin, OUTPUT);
    pinMode(out_pin, INPUT);
    if (is_resolution_3mm)
        (*range_pin_output_port) |= range_pin_mask; // this line replaces digitalWrite(range_pin, HIGH);
    else
        (*range_pin_output_port) &= ~range_pin_mask; // this line replaces digitalWrite(range_pin, LOW);
}

uint16_t yh::rec::Us_016::read_dist_mm () {
    return is_resolution_3mm ? (analogRead(out_pin) * 3) : analogRead(out_pin);
}

uint16_t yh::rec::Us_016::read_dist_cm () {
    return (is_resolution_3mm ? (analogRead(out_pin) * 3) : analogRead(out_pin)) / 10;
}

void yh::rec::Us_016::set_resolution (const uint8_t resolution) {
    if ((is_resolution_3mm = (resolution != 1)))
        (*range_pin_output_port) |= range_pin_mask; // this line replaces digitalWrite(range_pin, HIGH);
    else
        (*range_pin_output_port) &= ~range_pin_mask; // this line replaces digitalWrite(range_pin, LOW);
}

#endif //#ifndef US_016_CPP
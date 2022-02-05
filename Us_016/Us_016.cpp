#ifndef US_016_CPP
#define US_016_CPP __DATE__ ", " __TIME__

#include "Us_016.h"

yh::rec::Us_016_fast::Us_016_fast (const Us_016_fast &init_obj) :
    range_pin(init_obj.range_pin),
    out_pin(init_obj.out_pin),
    is_resolution_3m(init_obj.is_resolution_3m)
{
    range_pin_mask = (digitalPinToBitMask(range_pin));
    range_pin_output_port = (portOutputRegister(digitalPinToPort(range_pin)));
}

yh::rec::Us_016_fast::Us_016_fast (const uint32_t init_range_and_out_pin_and_resolution) :
    range_pin((init_range_and_out_pin_and_resolution >> 16) & 0xff),
    out_pin((init_range_and_out_pin_and_resolution >> 8) & 0xff),
    is_resolution_3m((init_range_and_out_pin_and_resolution & 0xff) != 1)
{
    range_pin_mask = (digitalPinToBitMask(range_pin));
    range_pin_output_port = (portOutputRegister(digitalPinToPort(range_pin)));
}

yh::rec::Us_016_fast::Us_016_fast (const uint8_t init_range_pin, const uint8_t init_out_pin, const uint8_t init_resolution) :
    range_pin(init_range_pin), out_pin(init_out_pin), is_resolution_3m(init_resolution != 1)
{
    range_pin_mask = (digitalPinToBitMask(range_pin));
    range_pin_output_port = (portOutputRegister(digitalPinToPort(range_pin)));
}

void yh::rec::Us_016_fast::begin () {
    pinMode(range_pin, OUTPUT);
    pinMode(out_pin, INPUT);
    if (is_resolution_3m)
        (*range_pin_output_port) |= range_pin_mask; // this line replaces digitalWrite(range_pin, HIGH);
    else
        (*range_pin_output_port) &= ~range_pin_mask; // this line replaces digitalWrite(range_pin, LOW);
}

uint16_t yh::rec::Us_016_fast::read_dist_mm () {
    return is_resolution_3m ? (analogRead(out_pin) * 3) : analogRead(out_pin);
}

uint16_t yh::rec::Us_016_fast::read_dist_cm () {
    return (is_resolution_3m ? (analogRead(out_pin) * 3) : analogRead(out_pin)) / 10;
}

void yh::rec::Us_016_fast::set_resolution (const uint8_t resolution) {
    if ((is_resolution_3m = (resolution != 1)))
        (*range_pin_output_port) |= range_pin_mask; // this line replaces digitalWrite(range_pin, HIGH);
    else
        (*range_pin_output_port) &= ~range_pin_mask; // this line replaces digitalWrite(range_pin, LOW);
}

yh::rec::Us_016::Us_016 (const yh::rec::Us_016 &init_obj) :
    Us_016_fast(init_obj.range_pin, init_obj.out_pin, init_obj.is_resolution_3m), raw_dist_read(0)
{
    //
}

yh::rec::Us_016::Us_016 (const uint32_t init_range_and_out_pin_and_resolution) :
    Us_016_fast(init_range_and_out_pin_and_resolution), raw_dist_read(0)
{
    //
}

yh::rec::Us_016::Us_016 (const uint8_t init_range_pin, const uint8_t init_out_pin, const uint8_t init_resolution) :
    Us_016_fast(init_range_pin, init_out_pin, init_resolution), raw_dist_read(0)
{
    //
}

uint16_t yh::rec::Us_016::read_dist_mm () {
    raw_dist_read = analogRead(out_pin);
    return is_resolution_3m ? (raw_dist_read * 3) : raw_dist_read;
}

uint16_t yh::rec::Us_016::read_dist_mm (const bool refresh) {
    if (refresh) raw_dist_read = analogRead(out_pin);
    return is_resolution_3m ? (raw_dist_read * 3) : raw_dist_read;
}

uint16_t yh::rec::Us_016::read_dist_cm () {
    raw_dist_read = analogRead(out_pin);
    return (is_resolution_3m ? (raw_dist_read * 3) : raw_dist_read) / 10;
}

uint16_t yh::rec::Us_016::read_dist_cm (const bool refresh) {
    if (refresh) raw_dist_read = analogRead(out_pin);
    return (is_resolution_3m ? (raw_dist_read * 3) : raw_dist_read) / 10;
}

#endif //#ifndef US_016_CPP
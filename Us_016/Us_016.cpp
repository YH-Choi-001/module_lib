#ifndef US_016_CPP
#define US_016_CPP __DATE__ ", " __TIME__

#include "Us_016.h"

yh::rec::Us_016::Us_016 (const Us_016 &init_obj) :
    range_pin(init_obj.range_pin),
    out_pin(init_obj.out_pin),
    is_resolution_3m(init_obj.is_resolution_3m),
    dist_read_mm(0)
{
    range_pin_mask = (digitalPinToBitMask(range_pin));
    range_pin_output_port = (portOutputRegister(digitalPinToPort(range_pin)));
}

yh::rec::Us_016::Us_016 (const uint32_t init_range_and_out_pin_and_resolution) :
    range_pin((init_range_and_out_pin_and_resolution >> 16) & 0xff),
    out_pin((init_range_and_out_pin_and_resolution >> 8) & 0xff),
    is_resolution_3m((init_range_and_out_pin_and_resolution & 0xff) != 1),
    dist_read_mm(0)
{
    range_pin_mask = (digitalPinToBitMask(range_pin));
    range_pin_output_port = (portOutputRegister(digitalPinToPort(range_pin)));
}

yh::rec::Us_016::Us_016 (const uint8_t init_range_pin, const uint8_t init_out_pin, const uint8_t init_resolution) :
    range_pin(init_range_pin), out_pin(init_out_pin), is_resolution_3m(init_resolution != 1), dist_read_mm(0)
{
    range_pin_mask = (digitalPinToBitMask(range_pin));
    range_pin_output_port = (portOutputRegister(digitalPinToPort(range_pin)));
}

uint16_t yh::rec::Us_016::raw_uts () {
    dist_read_mm = analogRead(out_pin);
    if (is_resolution_3m) dist_read_mm *= 3;
    return dist_read_mm;
}

inline void yh::rec::Us_016::refresh_range_pin () {
    if (is_resolution_3m)
        (*range_pin_output_port) |= range_pin_mask; // this line replaces digitalWrite(range_pin, HIGH);
    else
        (*range_pin_output_port) &= ~range_pin_mask; // this line replaces digitalWrite(range_pin, LOW);
}

inline void yh::rec::Us_016::begin () {
    pinMode(range_pin, OUTPUT);
    pinMode(out_pin, INPUT);
    refresh_range_pin();
}

uint16_t yh::rec::Us_016::read_dist_mm (const bool refresh) {
    if (refresh) raw_uts();
    return dist_read_mm;
}

uint16_t yh::rec::Us_016::read_dist_cm (const bool refresh) {
    if (refresh) raw_uts();
    return dist_read_mm / 10;
}

bool yh::rec::Us_016::set_resolution (const uint8_t resolution) {
    is_resolution_3m = (resolution != 1);
    refresh_range_pin();
    return is_resolution_3m;
}

#endif //#ifndef US_016_CPP
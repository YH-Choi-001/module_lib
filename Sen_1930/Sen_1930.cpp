// This library is proved to be working on 13 Feb 2022 at 16:53

#ifndef SEN_1930_CPP
#define SEN_1930_CPP __DATE__ ", " __TIME__

#include "Sen_1930.h"

yh::rec::Sen_1930::Sen_1930 (const uint8_t init_read_gryscl_pin) :
read_gryscl_pin(init_read_gryscl_pin), bnw_threshold(512)
{
    //
}

void yh::rec::Sen_1930::begin () {
    pinMode(read_gryscl_pin, INPUT);
}

void yh::rec::Sen_1930::set_threshold (const uint16_t input_threshold) {
    bnw_threshold = input_threshold > 1023 ? 1023 : input_threshold;
}

uint16_t yh::rec::Sen_1930::raw_read_gry_value () {
    return analogRead(read_gryscl_pin);
}

bool yh::rec::Sen_1930::touch_black () {
    return analogRead(read_gryscl_pin) < bnw_threshold;
}

bool yh::rec::Sen_1930::touch_white () {
    return analogRead(read_gryscl_pin) > bnw_threshold;
}

#endif //#ifndef SEN_1930_CPP
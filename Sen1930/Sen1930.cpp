#ifndef SEN1930_CPP
#define SEN1930_CPP __DATE__ ", " __TIME__

#include "Sen1930.h"

yh::rec::Sen1930::Sen1930 (const uint8_t init_read_gryscl_pin) :
read_gryscl_pin(init_read_gryscl_pin)
{
}

inline void yh::rec::Sen1930::begin () {
    pinMode(read_gryscl_pin, INPUT);
}

void yh::rec::Sen1930::set_lower_limit_of_white (const uint16_t input_lower_limit_of_white) {
    lower_limit_of_white = input_lower_limit_of_white > 1023 ? 1023 : input_lower_limit_of_white;
}

uint16_t yh::rec::Sen1930::raw_read_gry_value () {
    return gryscl_read = analogRead(read_gryscl_pin);
}

bool yh::rec::Sen1930::touch_white (const bool refresh) {
    if (refresh) raw_read_gry_value();
    return gryscl_read > lower_limit_of_white;
}

#endif //#ifndef SEN1930_CPP
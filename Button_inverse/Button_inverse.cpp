// This library is proved to be working on 6 Feb 2022 at 19:24

#ifndef BUTTON_INVERSE_CPP
#define BUTTON_INVERSE_CPP __DATE__ ", " __TIME__

#include "button_inverse.h"

yh::rec::Button_inverse::Button_inverse (const uint8_t init_read_button_pin) :
    read_button_pin(init_read_button_pin)
{
    read_button_pin_mask = digitalPinToBitMask(read_button_pin);
    read_button_pin_input_register = (portInputRegister(digitalPinToPort(read_button_pin)));
}

#endif // #ifndef BUTTON_INVERSE_CPP
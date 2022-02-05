#ifndef BUTTON_INVERSE_CPP
#define BUTTON_INVERSE_CPP __DATE__ ", " __TIME__

#include "button_inverse.h"

yh::rec::Button_inverse_fast::Button_inverse_fast (const uint8_t init_read_button_pin) :
    read_button_pin(init_read_button_pin)
{
    read_button_pin_mask = digitalPinToBitMask(read_button_pin);
    read_button_pin_input_port = (portInputRegister(digitalPinToPort(read_button_pin)));
}

void yh::rec::Button_inverse_fast::begin () {
    pinMode(read_button_pin, INPUT);
}

bool yh::rec::Button_inverse_fast::pressed_down () {
    return !((*read_button_pin_input_port) & read_button_pin_mask); // this line replaces return !digitalRead(read_button_pin);
}

yh::rec::Button_inverse::Button_inverse (const uint8_t init_read_button_pin) :
    Button_inverse_fast(init_read_button_pin)
{
}

bool yh::rec::Button_inverse::pressed_down () {
    return button_state = !((*read_button_pin_input_port) & read_button_pin_mask); // this line replaces button_state = !digitalRead(read_button_pin);
}

bool yh::rec::Button_inverse::pressed_down (const bool refresh) {
    if (refresh)
        button_state = !((*read_button_pin_input_port) & read_button_pin_mask); // this line replaces button_state = !digitalRead(read_button_pin);
    return button_state;
}

#endif // #ifndef BUTTON_INVERSE_CPP
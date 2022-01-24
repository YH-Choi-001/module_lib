#ifndef LED_W_CPP
#define LED_W_CPP __DATE__ ", " __TIME__

#include "Led_w.h"

yh::rec::Led_w_fast::Led_w_fast (const uint8_t init_led_w_pin) :
led_w_pin(init_led_w_pin)
{
    led_w_pin_mask = digitalPinToBitMask(led_w_pin);
    led_w_pin_output_port = portOutputRegister(digitalPinToPort(led_w_pin));
}

void yh::rec::Led_w_fast::begin () {
    pinMode(led_w_pin, OUTPUT);
}

void yh::rec::Led_w_fast::led_on () {
    (*led_w_pin_output_port) |= led_w_pin_mask; // this line replaces digitalWrite(led_w_pin, HIGH);
}

void yh::rec::Led_w_fast::led_off () {
    (*led_w_pin_output_port) &= ~led_w_pin_mask; // this line replaces digitalWrite(led_w_pin, LOW);
}

void yh::rec::Led_w_fast::set_led (const bool assign_led_state) {
    // this line below replaces digitalWrite(led_w_pin, assign_led_state);
    assign_led_state ? ((*led_w_pin_output_port) |= led_w_pin_mask) : ((*led_w_pin_output_port) &= ~led_w_pin_mask);
}

yh::rec::Led_w::Led_w (const uint8_t init_led_w_pin) :
Led_w_fast(init_led_w_pin)
{
    //
}

void yh::rec::Led_w::led_on () {
    if (!((*led_w_pin_output_port) & led_w_pin_mask))
        (*led_w_pin_output_port) |= led_w_pin_mask; // this line replaces digitalWrite(led_w_pin, HIGH);
}

void yh::rec::Led_w::led_off () {
    if ((*led_w_pin_output_port) & led_w_pin_mask)
    (*led_w_pin_output_port) &= ~led_w_pin_mask; // this line replaces digitalWrite(led_w_pin, LOW);
}

void yh::rec::Led_w::set_led (const bool assign_led_state) {
    // this line below replaces digitalWrite(led_w_pin, assign_led_state);
    // orig, new
    //    0    0 => nothing
    //    0    1 => turn_on
    //    1    0 => turn_off
    //    1    1 => nothing
    const bool curr_state = ((*led_w_pin_output_port) & led_w_pin_mask)?1:0;
    if ((assign_led_state?1:0) ^ curr_state)
        curr_state ? ((*led_w_pin_output_port) &= ~led_w_pin_mask) : ((*led_w_pin_output_port) |= led_w_pin_mask);
}

void yh::rec::Led_w::toggle_led () {
    ((*led_w_pin_output_port) & led_w_pin_mask) ? ((*led_w_pin_output_port) &= ~led_w_pin_mask) : ((*led_w_pin_output_port) |= led_w_pin_mask);
}

#endif // #ifndef LED_W_CPP
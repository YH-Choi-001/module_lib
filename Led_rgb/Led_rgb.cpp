// This library is proved to be working on 6 Feb 2022 at 19:19

#ifndef LED_RGB_CPP
#define LED_RGB_CPP __DATE__ ", " __TIME__

#include "Led_rgb.h"

#define LED_ON(x) ((*led_##x##_pin_output_register)|=led_##x##_pin_mask)
#define LED_OFF(x) ((*led_##x##_pin_output_register)&=(~led_##x##_pin_mask))
#define LED_TOGGLE(x) ((*led_##x##_pin_output_register)^=led_##x##_pin_mask)
#define LED_STATE(x) ((*led_##x##_pin_output_register)&led_##x##_pin_mask)

#define CONFIG_MASK_N_PORT(x) led_##x##_pin_mask=digitalPinToBitMask(led_##x##_pin);led_##x##_pin_output_register=portOutputRegister(digitalPinToPort(led_##x##_pin));

yh::rec::Led_rgb_fast::Led_rgb_fast (const Led_rgb_fast &init_obj) :
    led_r_pin(init_obj.led_r_pin), led_g_pin(init_obj.led_g_pin), led_b_pin(init_obj.led_b_pin)
{
    CONFIG_MASK_N_PORT(r)CONFIG_MASK_N_PORT(g)CONFIG_MASK_N_PORT(b)
}

yh::rec::Led_rgb_fast::Led_rgb_fast (const uint32_t init_led_r_g_b_pin) :
    led_r_pin((init_led_r_g_b_pin >> 16) & 0xff), led_g_pin((init_led_r_g_b_pin >> 8) & 0xff), led_b_pin(init_led_r_g_b_pin & 0xff)
{
    CONFIG_MASK_N_PORT(r)CONFIG_MASK_N_PORT(g)CONFIG_MASK_N_PORT(b)
}

yh::rec::Led_rgb_fast::Led_rgb_fast (const uint8_t init_led_r_pin, const uint8_t init_led_g_pin, const uint8_t init_led_b_pin) :
    led_r_pin(init_led_r_pin), led_g_pin(init_led_g_pin), led_b_pin(init_led_b_pin)
{
    CONFIG_MASK_N_PORT(r)CONFIG_MASK_N_PORT(g)CONFIG_MASK_N_PORT(b)
}

#undef CONFIG_MASK_N_PORT

void yh::rec::Led_rgb_fast::begin () {
    #define CONFIG_PINMODE(x) pinMode(led_##x##_pin,OUTPUT);
    CONFIG_PINMODE(r) CONFIG_PINMODE(g) CONFIG_PINMODE(b)
    #undef CONFIG_PINMODE
}

#define ON_OFF(x) void yh::rec::Led_rgb_fast::x##_on() {LED_ON(x);} void yh::rec::Led_rgb_fast::x##_off() {LED_OFF(x);}
ON_OFF(r) ON_OFF(g) ON_OFF(b)
#undef ON_OFF

#define SET(x) void yh::rec::Led_rgb_fast::set_##x(const bool assign_##x##_state) {assign_##x##_state?LED_ON(x):LED_OFF(x);}
SET(r) SET(g) SET(b)
#undef SET

#define TOGGLE(x) void yh::rec::Led_rgb_fast::toggle_##x() {LED_TOGGLE(x);}
TOGGLE(r) TOGGLE(g) TOGGLE(b)
#undef TOGGLE

#define STATE(x) bool yh::rec::Led_rgb_fast::x##_state() {return LED_STATE(x)?1:0;}
STATE(r) STATE(g) STATE(b)
#undef STATE

void yh::rec::Led_rgb_fast::set_rgb (const uint8_t xxxxxrgb) {
    (xxxxxrgb & 0x04) ? LED_ON(r) : LED_OFF(r);
    (xxxxxrgb & 0x02) ? LED_ON(g) : LED_OFF(g);
    (xxxxxrgb & 0x01) ? LED_ON(b) : LED_OFF(b);
}

void yh::rec::Led_rgb_fast::set_rgb (const bool r, const bool g, const bool b) {
    r ? LED_ON(r) : LED_OFF(r);
    g ? LED_ON(g) : LED_OFF(g);
    b ? LED_ON(b) : LED_OFF(b);
}

#undef LED_ON
#undef LED_OFF
#undef LED_STATE

yh::rec::Led_rgb_analog::Led_rgb_analog (const Led_rgb_analog &init_obj) :
    led_r_pin(init_obj.led_r_pin), led_g_pin(init_obj.led_g_pin), led_b_pin(init_obj.led_b_pin)
{
    //
}

yh::rec::Led_rgb_analog::Led_rgb_analog (const uint32_t init_led_r_g_b_pin) :
    led_r_pin((init_led_r_g_b_pin >> 16) & 0xff), led_g_pin((init_led_r_g_b_pin >> 8) & 0xff), led_b_pin(init_led_r_g_b_pin & 0xff)
{
    //
}

yh::rec::Led_rgb_analog::Led_rgb_analog (const uint8_t init_led_r_pin, const uint8_t init_led_g_pin, const uint8_t init_led_b_pin) :
    led_r_pin(init_led_r_pin), led_g_pin(init_led_g_pin), led_b_pin(init_led_b_pin)
{
    //
}

void yh::rec::Led_rgb_analog::begin () {
    #define CONFIG_PINMODE(x) pinMode(led_##x##_pin,OUTPUT);
    CONFIG_PINMODE(r) CONFIG_PINMODE(g) CONFIG_PINMODE(b)
    #undef CONFIG_PINMODE
}

#define SET(x) void yh::rec::Led_rgb_analog::set_##x(const uint8_t assign_##x##_brightness) {analogWrite(led_##x##_pin,x##_brightness=assign_##x##_brightness);}
SET(r) SET(g) SET(b)
#undef SET

#define GET_BRIGHTNESS(x) uint8_t yh::rec::Led_rgb_analog::get_##x##_brightness() {return x##_brightness;}
GET_BRIGHTNESS(r) GET_BRIGHTNESS(g) GET_BRIGHTNESS(b)
#undef GET_BRIGHTNESS

#endif // #ifndef LED_RGB_CPP
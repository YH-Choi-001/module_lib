// This library is proved to be working on 6 Feb 2022 at 19:19

#ifndef LED_RGB_H
#define LED_RGB_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        class Led_rgb_fast {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pin connected to the red led
                const uint8_t led_r_pin;
                uint8_t led_r_pin_mask;
                volatile uint8_t *led_r_pin_output_register;
                // the pin connected to the green led
                const uint8_t led_g_pin;
                uint8_t led_g_pin_mask;
                volatile uint8_t *led_g_pin_output_register;
                // the pin connected to the blue led
                const uint8_t led_b_pin;
                uint8_t led_b_pin_mask;
                volatile uint8_t *led_b_pin_output_register;
            public:
                Led_rgb_fast (const Led_rgb_fast &init_obj);
                // inits the red, green, blue led pins to the object
                Led_rgb_fast (const uint8_t init_led_r_pin, const uint8_t init_led_g_pin, const uint8_t init_led_b_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // turn on red led
                virtual void r_on ();
                // turn off red led
                virtual void r_off ();
                // turn on green led
                virtual void g_on ();
                // turn off green led
                virtual void g_off ();
                // turn on blue led
                virtual void b_on ();
                // turn off blue led
                virtual void b_off ();
                // set red led on or off
                virtual void set_r (const bool assign_r_state);
                // set green led on or off
                virtual void set_g (const bool assign_g_state);
                // set blue led on or off
                virtual void set_b (const bool assign_b_state);
                // toggle the red led to its inverse state
                virtual void toggle_r ();
                // toggle the green led to its inverse state
                virtual void toggle_g ();
                // toggle the blue led to its inverse state
                virtual void toggle_b ();
                // returns the state of red led
                bool r_state ();
                // returns the state of green led
                bool g_state ();
                // returns the state of blue led
                bool b_state ();
                // set red, green, blue led on or off together
                virtual void set_rgb (const uint8_t xxxxxrgb);
                // set red, green, blue led on or off together
                virtual void set_rgb (const bool r, const bool g, const bool b);
        };
        // This class uses analogWrite(pin, val) function,
        // which allows varying brightness of led of each color.
        // note: only pwm pins are able to use this class
        class Led_rgb_analog {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pin connected to the red led
                const uint8_t led_r_pin;
                // the pin connected to the green led
                const uint8_t led_g_pin;
                // the pin connected to the blue led
                const uint8_t led_b_pin;
                // the brightness of the red led
                uint8_t r_brightness;
                // the brightness of the green led
                uint8_t g_brightness;
                // the brightness of the blue led
                uint8_t b_brightness;
            public:
                Led_rgb_analog (const Led_rgb_analog &init_obj);
                // inits the red, green, blue led pins to the object
                Led_rgb_analog (const uint8_t init_led_r_pin, const uint8_t init_led_g_pin, const uint8_t init_led_b_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // set the brightness of the red led [0:255]
                void set_r (const uint8_t assign_r_brightness);
                // set the brightness of the green led [0:255]
                void set_g (const uint8_t assign_g_brightness);
                // set the brightness of the blue led [0:255]
                void set_b (const uint8_t assign_b_brightness);
                // returns the brightness of the red led
                uint8_t get_r_brightness ();
                // returns the brightness of the green led
                uint8_t get_g_brightness ();
                // returns the brightness of the blue led
                uint8_t get_b_brightness ();
        };
    }
}

#endif // #ifndef LED_RGB_H
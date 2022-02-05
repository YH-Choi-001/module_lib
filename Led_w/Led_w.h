#ifndef LED_W_H
#define LED_W_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        class Led_w_fast {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pin connected to the led
                const uint8_t led_w_pin;
                uint8_t led_w_pin_mask;
                volatile uint8_t *led_w_pin_output_port;
            public:
                // inits the white led pin to the object
                Led_w_fast (const uint8_t init_led_w_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // turn on the led
                virtual void led_on ();
                // turn off the led
                virtual void led_off ();
                // set the led on or off
                virtual void set_led (const bool assign_led_state);
                // toggle the led to the inverse state
                void toggle_led ();
                // returns the state of the led
                bool led_state ();
        };
        class Led_w : public Led_w_fast {
            private:
                //
            protected:
                //
            public:
                // inits the white led pin to the object
                Led_w (const uint8_t init_led_w_pin);
                // turn on the led
                // to override Led_w_fast::led_on()
                void led_on ();
                // turn off the led
                // to override Led_w_fast::led_off()
                void led_off ();
                // set the led on or off
                // to override Led_w_fast::set_led()
                void set_led (const bool assign_led_state);
        };
        // This class uses analogWrite(pin, val) function,
        // which allows varying brightness of the led.
        // note 1: only analog pins and pwm pins are able to use this class
        // note 2: analogWrite(...) is very slow
        class Led_w_analog {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pin connected to the led
                const uint8_t led_w_pin;
                // the brightness of the led
                uint8_t led_brightness;
            public:
                // inits the white led pin to the object
                Led_w_analog (const uint8_t init_led_w_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // set the brightness of the led [0:255]
                void set_led (const uint8_t assign_led_brightness);
                // returns the brightness of the led
                uint8_t get_led_brightness ();
        };
    }
}

#endif // #ifndef LED_W_H
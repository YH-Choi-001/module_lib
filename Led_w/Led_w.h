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
                const uint8_t led_w_pin;
                uint8_t led_w_pin_mask;
                volatile uint8_t *led_w_pin_output_port;
            public:
                // inits the white led pin to the argument
                Led_w_fast (const uint8_t init_led_w_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                inline void begin ();
                // turn on the led
                virtual void led_on ();
                // turn off the led
                virtual void led_off ();
                // set the led on or off
                virtual void set_led (const bool assign_led_state);
        };
        class Led_w : public Led_w_fast {
            private:
                //
            protected:
                bool led_state;
            public:
                // inits the white led pin to the argument
                Led_w (const uint8_t init_led_w_pin);
                // turn on the led
                void led_on ();
                // turn off the led
                void led_off ();
                // set the led on or off
                void set_led (const bool assign_led_state);
                // toggle the led to the inverse state
                void toggle_led ();
        };
    }
}

#endif // #ifndef LED_W_H
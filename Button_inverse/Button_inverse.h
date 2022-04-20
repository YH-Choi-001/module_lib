// This library is proved to be working on 6 Feb 2022 at 19:24

#ifndef BUTTON_INVERSE_H
#define BUTTON_INVERSE_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        class Button_inverse {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pin to read the state of inverse button
                const uint8_t button_signal_pin;
                uint8_t button_signal_mask;
                volatile uint8_t *button_signal_reg;
            public:
                // inits the button pin to this object
                Button_inverse (const uint8_t init_button_signal_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                inline void begin () __attribute__((__always_inline__)) {
                    button_signal_reg = (portInputRegister(digitalPinToPort(button_signal_pin)));
                    button_signal_mask = digitalPinToBitMask(button_signal_pin);
                    pinMode(button_signal_pin, INPUT);
                }
                // checks if the button is pressed
                inline bool pressed_down () __attribute__((__always_inline__)) {
                    return !((*button_signal_reg) & button_signal_mask);
                }
        };
    }
}

#endif // #ifndef BUTTON_INVERSE_H
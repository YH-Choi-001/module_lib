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
                const uint8_t read_button_pin;
                uint8_t read_button_pin_mask;
                volatile uint8_t *read_button_pin_input_register;
            public:
                // inits the button pin to this object
                Button_inverse (const uint8_t init_read_button_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // checks if the button is pressed
                virtual bool pressed_down ();
        };
    }
}

#endif // #ifndef BUTTON_INVERSE_H
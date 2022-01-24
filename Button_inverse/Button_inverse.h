#ifndef BUTTON_INVERSE_H
#define BUTTON_INVERSE_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        class Button_inverse_fast {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pin to read the state of inverse button
                const uint8_t read_button_pin;
                uint8_t read_button_pin_mask;
                volatile uint8_t *read_button_pin_input_port;
            public:
                // inits the button pin to this object
                Button_inverse_fast (const uint8_t init_read_button_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                inline void begin ();
                // checks if the button is pressed
                virtual bool pressed_down ();
        };
        class Button_inverse : public Button_inverse_fast {
            private:
                //
            protected:
                // the bool that saves the previous button state
                bool button_state;
            public:
                // inits the button pin to this object
                Button_inverse (const uint8_t init_read_button_pin);
                // checks if the button is pressed
                // to override Button_inverse_fast::pressed_down()
                bool pressed_down ();
                // checks if the button is pressed
                bool pressed_down (const bool refresh);
        };
    }
}

#endif // #ifndef BUTTON_INVERSE_H
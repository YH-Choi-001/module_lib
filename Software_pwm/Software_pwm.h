#ifndef SOFTWARE_PWM_H
#define SOFTWARE_PWM_H

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        class Software_pwm {
            private:
                //
            protected:
                // the array to save the registered pins
                uint8_t *pins;
                // the amount of pins registered
                uint8_t count_of_pins;
                // the port output registers of the pins
                volatile uint8_t **port_output_regs;
                // the bit masks of the pins
                uint8_t *masks;
                // the values of the pins
                uint8_t *values;
                // the flag that locks the attach_pin(...) method after begin() method has been called
                bool begin_locking;
            public:
                //
                Software_pwm ();
                // theoretically I need to add this destructor to free the memory after the object is destroyed
                ~Software_pwm ();
                //
                void attach_pin (const uint8_t pin);
                //
                void begin ();
                //
                int8_t analog_write (const uint8_t pin, const uint8_t val);
                // you may call me in a compare match interrupt or overflow interrupt of a timer
                inline void timer_update_isr (void);
        };
    }
}

#ifndef SOFTWARE_PWM_CPP
yh::rec::Software_pwm software_pwm;
#endif // #ifndef SOFTWARE_PWM_CPP

#endif // #ifndef SOFTWARE_PWM_H
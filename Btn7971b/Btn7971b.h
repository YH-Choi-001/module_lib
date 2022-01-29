#ifndef BTN7971B_H
#define BTN7971B_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        class Btn7971b {
            private:
                //
            protected:
                // pins that cannot be changed:
                // pwm 1
                const uint8_t pwm_1_pin;
                uint8_t pwm_1_pin_mask;
                volatile uint8_t *pwm_1_pin_output_port;
                // pwm 2
                const uint8_t pwm_2_pin;
                uint8_t pwm_2_pin_mask;
                volatile uint8_t *pwm_2_pin_output_port;
                // the current speed of the motor
                // (actually is the voltage supplied to the motor)
                int16_t speed;
                // the limits on the spd of the motor
                const int16_t
                    slowest_spd,
                    fastest_spd;
            public:
                // inits the arguments into pwm pins 1 and 2
                Btn7971b (const uint8_t init_pwm_1_pin, const uint8_t init_pwm_2_pin);
                // inits the arguments into pwm pins 1 and 2
                // syntax should be Btn7971b(pwm_1_pin << 8 | pwm_2_pin)
                Btn7971b (const uint16_t init_pwm_1_and_2_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                inline void begin ();
                // set the speed of the motor [-slowest_spd:fastest_spd]
                inline void set_spd (const int16_t input_spd);
                // stops the motor
                inline void stop_motor ();
                // returns the speed inputed for the motor
                inline int16_t get_spd ();
                // adds the increase to the current speed of the motor
                // and returns the newest speed
                inline int16_t operator += (const int16_t increase);
                // subtracts the decrease from the current speed of the motor
                // and returns the newest speed
                inline int16_t operator -= (const int16_t decrease);
                // increases the motor speed by 1
                inline int16_t operator ++ ();
                // decreases the motor speed by 1
                inline int16_t operator -- ();
        };
    }
}

#endif // #ifndef BTN7971B_H
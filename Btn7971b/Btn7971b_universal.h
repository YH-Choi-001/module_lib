#ifdef BTN7971B_H
// This library is proved to be working on 24 Mar 2022 with bottleSumo robot

// written by YH Choi
// look for the latest version on https://github.com/YH-Choi-001/module_lib/blob/main/Btn7971b/Btn7971b.h

#ifndef BTN7971B_UNIVERSAL_H
#define BTN7971B_UNIVERSAL_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

// // for function turnOffPWM
// #include "wiring_digital.c"

namespace yh {
    namespace rec {
        // warning: BTN7971b uses H-bridge design,
        // which does not support DAC input, but PWM input only.
        // Never use a DAC on any IN pins on BTN7971b
        //    IN1 : IN2   
        //    LOW : LOW    => stop
        //    LOW : HIGH   => clockwise
        //   HIGH : LOW    => anti-clockwise
        //   HIGH : HIGH   => stop
        // now let pwm be IN1, dir be IN2
        class Btn7971b {
            private:
                //
            protected:
                // pins that cannot be changed:
                // pwm pin
                const uint8_t pwm_pin;
                // dir pin
                const uint8_t dir_pin;
                // the current speed of the motor
                // (actually is the period of the pwm cycle over 255)
                int16_t speed;
                // the limits on the spd of the motor
                const int16_t
                    slowest_spd,
                    fastest_spd;
                // writes the pwm value
                inline void analog_write_pwm_pin (const uint8_t val);
            public:
                Btn7971b (const Btn7971b &init_obj);
                // inits the arguments into pwm pin and dir pin
                Btn7971b (const uint8_t init_pwm_pin, const uint8_t init_dir_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // set the speed of the motor
                // @param input_spd ranges from [-slowest_spd:fastest_spd]
                void set_spd (const int16_t input_spd);
                // stops the motor
                void stop_motor ();
                // full speed forwards
                void full_spd_forwards ();
                // full speed backwards
                void full_spd_backwards ();
                // returns the speed inputed for the motor
                int16_t get_spd ();
                // adds the increase to the current speed of the motor
                // and returns the newest speed
                int16_t operator += (const int16_t increase);
                // subtracts the decrease from the current speed of the motor
                // and returns the newest speed
                int16_t operator -= (const int16_t decrease);
                // increases the motor speed by 1
                int16_t operator ++ ();
                // decreases the motor speed by 1
                int16_t operator -- ();
        };
    }
}

#endif // #ifndef BTN7971B_UNIVERSAL_H

#endif // #ifdef BTN7971B_H
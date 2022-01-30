#ifndef HC_SR04_H
#define HC_SR04_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)



namespace yh {
    namespace rec {
        // note that the implementation has set restrictions on the farest distance it can read
        // since the soccer field in RCJ soccer has a limited area
        class Hc_sr04_fast {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pin to trigger a sound wave
                const uint8_t trig_pin;
                uint8_t trig_pin_mask;
                volatile uint8_t *trig_pin_output_port;
                // the pin to read the soundwave sensor
                const uint8_t echo_pin;
                uint8_t echo_pin_mask;
                volatile uint8_t *echo_pin_input_port;
                // ask the ultrasound to pulse a sound wave
                void trig_wave ();
            public:
                Hc_sr04_fast (const Hc_sr04_fast &init_obj);
                // inits both trigger pin and echo pin with one argument only
                // you should combine 2 pins like this: (trig_pin << 8) | echo_pin
                Hc_sr04_fast (const uint16_t init_trig_and_echo_pin);
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04_fast (const uint8_t init_trig_pin, const uint8_t init_echo_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                inline void begin ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                // returns 888 when the returning sound wave is undetectable
                // has better performance under noInterrupts() environment
                virtual uint16_t read_dist_mm (const unsigned long limiting_time_in_us = 13000U);
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                // returns 888 when the returning sound wave is undetectable
                virtual uint16_t read_dist_cm (const unsigned long limiting_time_in_us = 13000U);
        };
        class Hc_sr04 : public Hc_sr04_fast {
            private:
                //
            protected:
                // the current ultrasound reading (unit is mm)
                uint16_t dist_read_mm;
                // does pulseIn on the echo pin and saves the distance read to dist_read_mm
                // !! call trig_wave() method before calling me !!
                void custom_pulseIn (const unsigned long limiting_time_in_us);
            public:
                Hc_sr04 (const Hc_sr04 &init_obj);
                // inits both trigger pin and echo pin with one argument only
                // you should combine 2 pins like this: (trig_pin << 8) | echo_pin
                Hc_sr04 (const uint16_t init_trig_and_echo_pin);
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04 (const uint8_t init_trig_pin, const uint8_t init_echo_pin);
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                // returns 888 when the returning sound wave is undetectable
                // has better performance under noInterrupts() environment
                uint16_t read_dist_mm (const unsigned long limiting_time_in_us = 13000U);
                // returns the saved reading of the last update (unit is mm)
                uint16_t previous_dist_mm ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                // returns 888 when the returning sound wave is undetectable
                uint16_t read_dist_cm (const unsigned long limiting_time_in_us = 13000U);
                // returns the saved reading of the last update (unit is cm)
                uint16_t previous_dist_cm ();
        };
    }
}

#endif //#ifndef HC_SR04_H
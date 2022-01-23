#ifndef HC_SR04_H
#define HC_SR04_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)



namespace yh {
    namespace rec {
        class Hc_sr04 {
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
                // the current ultrasound reading (unit is mm)
                uint16_t dist_read_mm;
                // ask the ultrasound to pulse a sound wave
                void trig_wave ();
                // ask the ultrasound to pulse a sound wave and wait for echo pin to rise
                // suitable for using interrupt to collect data
                // void trig_wave_and_wait ();
                // functions that load raw data for other functions to further process
                // !! call trig_wave() method before calling me !!
                uint16_t raw_uts_by_pulseIn (const unsigned long limiting_time_in_us = 13000U);
                // interrupt to record and process the time
                // !! read-only !!
                // do not use this function directly as it is completely wrong from the beginning
                // void int_2_coll_echo_data_template ();
                // functions that load raw data for other functions to further process
                // attaches an interrupt to the echo pin, to allow processing other data
                // while waiting for the returning sound wave to come back
                // !! only available for pins that are declared able to run hardware interrupts !!
                // inline void attach_data_collecting_method_to_interrupt ();
                // detaches the ISR that has been attached in attach_data_collecting_method_to_interrupt()
                // !! only available for pins that are declared able to run hardware interrupts !!
                // inline void detach_data_collecting_method_to_interrupt ();
            public:
                Hc_sr04 (const Hc_sr04 &init_obj);
                // inits both trigger pin and echo pin with one argument only
                // you should combine 2 pins like this: (trig_pin << 8) | echo_pin
                Hc_sr04 (const uint16_t init_trig_and_echo_pin);
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04 (const uint8_t init_trig_pin, const uint8_t init_echo_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                inline void begin ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                // returns 888 when the returning sound wave is undetectable
                uint16_t read_dist_mm (const bool refresh = true, const unsigned long limiting_time_in_us = 13000U);
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                // returns 888 when the returning sound wave is undetectable
                uint16_t read_dist_cm (const bool refresh = true, const unsigned long limiting_time_in_us = 13000U);
        };
    }
}

#endif //#ifndef HC_SR04_H
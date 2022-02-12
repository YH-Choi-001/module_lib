// This library is proved to be working on 6 Feb 2022 at 18:36

#ifndef HC_SR04_H
#define HC_SR04_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

// pulseIn is simplified to countPulseASM, so the following libraries are included
#include "wiring_private.h"
#include "pins_arduino.h"

namespace yh {
    namespace rec {
        class Hc_sr04_fast {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pin to trigger a sound wave
                const uint8_t trig_pin;
                uint8_t trig_pin_mask;
                volatile uint8_t *trig_pin_output_register;
                // the pin to read the soundwave sensor
                const uint8_t echo_pin;
                uint8_t echo_pin_mask;
                volatile uint8_t *echo_pin_input_register;
                // ask the ultrasound to pulse a sound wave
                virtual inline void trig_wave ();
            public:
                Hc_sr04_fast (const Hc_sr04_fast &init_obj);
                // inits both trigger pin and echo pin with one argument only
                // you should combine 2 pins like this: (trig_pin << 8) | echo_pin
                Hc_sr04_fast (const uint16_t init_trig_and_echo_pin);
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04_fast (const uint8_t init_trig_pin, const uint8_t init_echo_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                virtual void begin ();
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
        /* Using Hc_sr04 with timer interrupts to scan the status of echo pins is tested and failed
           where the reason is still unknown.
           Using Hc_sr04 with hardware interrupts to get the time interval is tested and failed
           where the reason is still unknown.




        // This class does not use pulseIn, but instead
        // relies on programmer-defined timer interrupts.
        // It is suggested to set the timer interrupt between every 58 - 59 us
        // if the required precision is 1cm.
        class Hc_sr04_timer_int : protected Hc_sr04_fast {
            private:
                //
            protected:
                // different from other classes that counts time based on precise pulseIn()
                // the current ultrasound reading (unit is cm)
                volatile uint16_t dist_read_cm;
                // maximum time to be waited before declaring the distance is undetectable
                unsigned long limiting_time;
                // the time that the wave was triggered (unit is microsecond)
                volatile unsigned long trig_time_micros;
                // the flag to indicate whether the returning pulse is received
                volatile bool pulse_received;
                // the ticks counted in the ISR
                volatile unsigned long timer_tick;
                //
                volatile unsigned long test_time;
                // // the flag to indicate whether the ISR should continue update the sensor
                // bool autoscan;
            public:
                // constructor that calls inputs another constructor
                Hc_sr04_timer_int (const Hc_sr04_timer_int &init_obj);
                // inits both trigger pin and echo pin with one argument only
                // you should combine 2 pins like this: (trig_pin << 8) | echo_pin
                Hc_sr04_timer_int (const uint16_t init_trig_and_echo_pin);
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04_timer_int (const uint8_t init_trig_pin, const uint8_t init_echo_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                // also auto calls trig_wave() one time to kick off the interrupt
                void begin ();
                // sets the maximum distance to be measured before declaring the distance is undetectable
                void set_limiting_dist (const uint16_t request_limiting_dist);
                // sets the maximum time to be waited before declaring the distance is undetectable
                void set_limiting_time (const unsigned long request_limiting_time);
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                // returns 888 when the returning sound wave is undetectable
                // for compatibility with original Hc_sr04 series above
                inline uint16_t read_dist_mm () { return dist_read_cm * 10; }
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                // returns 888 when the returning sound wave is undetectable
                // for compatibility with original Hc_sr04 series above
                inline uint16_t read_dist_cm () { return dist_read_cm; }
                // // turn on the auto scan feature to start reading the distance
                // inline void turn_on_autoscan () { autoscan = true; trig_wave(); }
                // // turn off the auto scan feature and stop reading the distance
                // inline void turn_off_autoscan () { autoscan = false; dist_read_cm = 0; }

                // ISR-function

                // writes the trig_pin to HIGH
                inline void write_trig_pin_high () { (*trig_pin_output_register) |= trig_pin_mask; }
                // writes the trig_pin to LOW
                inline void write_trig_pin_low () { (*trig_pin_output_register) &= ~trig_pin_mask; }

                // triggers the ultrasound to pulse a sound wave
                inline void trig_wave () {
                    if (pulse_received) {
                        pulse_received = false;
                        (*trig_pin_output_register) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);
                        delayMicroseconds(2);
                        (*trig_pin_output_register) |= trig_pin_mask;  // this line replaces digitalWrite(trig_pin, HIGH);
                        delayMicroseconds(10);
                        (*trig_pin_output_register) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);
                        #if defined(HC_SR04_USE_MICROS)
                        trig_time_micros = micros();
                        #endif // #if defined(HC_SR04_USE_MICROS)
                        #if defined(HC_SR04_USE_COUNTER_TICKS)
                        timer_tick = 0;
                        #endif // #if defined(HC_SR04_USE_COUNTER_TICKS)
                    }
                }

                #if defined(HC_SR04_USE_TEST_TIMER)
                inline void polling_refresh () {
                    if (test_time) {
                        (*trig_pin_output_register) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);
                        delayMicroseconds(2);
                        (*trig_pin_output_register) |= trig_pin_mask;  // this line replaces digitalWrite(trig_pin, HIGH);
                        delayMicroseconds(10);
                        (*trig_pin_output_register) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);
                        dist_read_cm = (test_time - trig_time_micros) * 0.017;
                        trig_time_micros = micros();
                        test_time = 0;
                    }
                }
                #endif // #if defined(HC_SR04_USE_TEST_TIMER)

                #if defined(HC_SR04_USE_TEST_TIMER)
                inline void timer_int_isr_update () {
                    if (!( ((*echo_pin_input_register) & echo_pin_mask) || test_time)) { // echo pin is LOW and test_time is empty
                        test_time = micros();
                    }
                }
                #else // #elif defined(HC_SR04_USE_TEST_TIMER)
                // ISR-function
                // call me in the ISR to update
                inline void timer_int_isr_update () {
                    // if (autoscan) {
                    if (!pulse_received) {
                    #if defined(HC_SR04_USE_MICROS)
                    static bool prev_echo_pin_state = ((*echo_pin_input_register) & echo_pin_mask);
                    const bool curr_echo_pin_state = ((*echo_pin_input_register) & echo_pin_mask);
                    if (!curr_echo_pin_state && prev_echo_pin_state) { // echo pin falls
                        dist_read_cm = (micros() - trig_time_micros) * 0.017; // calculate the distance to obstacles
                        pulse_received = true;
                        // trig_wave(); // triggers another wave again
                    } else if (micros() - trig_time_micros > limiting_time) { // checks if the time length exceeds the limit
                        dist_read_cm = 888; // error distance
                        pulse_received = true;
                        // trig_wave(); // re-trigger the wave
                    }
                    prev_echo_pin_state = curr_echo_pin_state;
                    #elif defined(HC_SR04_USE_COUNTER_TICKS)
                    static bool prev_echo_pin_state = ((*echo_pin_input_register) & echo_pin_mask);
                    const bool curr_echo_pin_state = ((*echo_pin_input_register) & echo_pin_mask);
                    if (!curr_echo_pin_state && prev_echo_pin_state) { // echo pin falls
                        dist_read_cm = timer_tick;
                        pulse_received = true;
                        // trig_wave(); // triggers another wave again
                    } else if (timer_tick > 221) { // checks if the dist length exceeds 400 cm
                        dist_read_cm = 888; // error distance
                        pulse_received = true;
                        // trig_wave(); // re-trigger the wave
                    } else {
                        timer_tick++;
                    }
                    prev_echo_pin_state = curr_echo_pin_state;
                    #endif // #elif defined(HC_SR04_USE_COUNTER_TICKS)
                    }
                }
                #endif // line 178 #else // #elif defined(HC_SR04_USE_TEST_TIMER)

        };
        class Hc_sr04_timer_int_beta : protected Hc_sr04_fast {
            // start clearup process
            private:
                //
            protected:
                // different from other classes that counts time based on precise pulseIn()
                // the current ultrasound reading (unit is cm)
                volatile uint16_t dist_read_cm;
                // the time that the wave was triggered (unit is microsecond)
                volatile unsigned long trig_time_micros;
                // the time that the sensor receives the returning wave (unit is microsecond)
                volatile unsigned long wave_returning_time_micros;
                // // the flag to indicate whether the ISR should continue update the sensor
                // bool autoscan;
            public:
                // constructor that calls inputs another constructor
                Hc_sr04_timer_int_beta (const Hc_sr04_timer_int_beta &init_obj) :
    Hc_sr04_fast(init_obj.trig_pin, init_obj.echo_pin), dist_read_cm(0), trig_time_micros(0), wave_returning_time_micros(1) {}
                // inits both trigger pin and echo pin with one argument only
                // you should combine 2 pins like this: (trig_pin << 8) | echo_pin
                Hc_sr04_timer_int_beta (const uint16_t init_trig_and_echo_pin) :
    Hc_sr04_fast(init_trig_and_echo_pin), dist_read_cm(0), trig_time_micros(0), wave_returning_time_micros(1) {}
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04_timer_int_beta (const uint8_t init_trig_pin, const uint8_t init_echo_pin) :
    Hc_sr04_fast(init_trig_pin, init_echo_pin), dist_read_cm(999), trig_time_micros(0), wave_returning_time_micros(1) {}
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                // also auto calls trig_wave() one time to kick off the interrupt
                void begin () {
                    pinMode(trig_pin, OUTPUT);
                    pinMode(echo_pin, INPUT);
                }
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                // returns 888 when the returning sound wave is undetectable
                // for compatibility with original Hc_sr04 series above
                inline uint16_t read_dist_mm () { return dist_read_cm * 10; }
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                // returns 888 when the returning sound wave is undetectable
                // for compatibility with original Hc_sr04 series above
                inline uint16_t read_dist_cm () { return dist_read_cm; }
                // // turn on the auto scan feature to start reading the distance
                // inline void turn_on_autoscan () { autoscan = true; trig_wave(); }
                // // turn off the auto scan feature and stop reading the distance
                // inline void turn_off_autoscan () { autoscan = false; dist_read_cm = 0; }

                // ISR-function

                inline void polling_refresh () {
                    if (wave_returning_time_micros) {
                        (*trig_pin_output_register) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);
                        delayMicroseconds(2);
                        (*trig_pin_output_register) |= trig_pin_mask;  // this line replaces digitalWrite(trig_pin, HIGH);
                        delayMicroseconds(10);
                        (*trig_pin_output_register) &= ~trig_pin_mask; // this line replaces digitalWrite(trig_pin, LOW);
                        const uint16_t temp_dist_read_cm = (wave_returning_time_micros - trig_time_micros) * 0.017;
                        if (temp_dist_read_cm) dist_read_cm = temp_dist_read_cm; // locking mechanism to prevent always 0-reporting
                        trig_time_micros = micros();
                        wave_returning_time_micros = 0;
                    }
                }
                inline void timer_int_isr_update () {
                    if (!( ((*echo_pin_input_register) & echo_pin_mask) || wave_returning_time_micros)) { // echo pin is LOW and test_time is empty
                        wave_returning_time_micros = micros();
                    }
                }
        };
        */
    }
}

#endif //#ifndef HC_SR04_H
// This library is proved to be working on 6 Feb 2022 at 18:36

#ifndef HC_SR04_H
#define HC_SR04_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

// pulseIn is simplified to countPulseASM, so the following libraries are included
#include "wiring_private.h"

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
                // the max waiting time for the sound pulse to return (unit is microseconds)
                unsigned long max_waiting_time_in_us;
            public:
                Hc_sr04_fast (const Hc_sr04_fast &init_obj);
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04_fast (const uint8_t init_trig_pin, const uint8_t init_echo_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                virtual void begin ();
                // sets the maximum time the program will pause and wait for the sound pulse to return (unit is microseconds)
                void set_max_waiting_time_in_us (const unsigned long assign_max_waiting_time_in_us);
                // sets the maximum range of the sensor (unit is mm)
                void set_max_range_in_mm (const double max_range_in_mm);
                // sets the maximum range of the sensor (unit is cm)
                void set_max_range_in_cm (const double max_range_in_cm);
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                // returns 888 when the returning sound wave is undetectable
                // has better performance under noInterrupts() environment
                virtual uint16_t read_dist_mm ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                // returns 888 when the returning sound wave is undetectable
                // has better performance under noInterrupts() environment
                virtual uint16_t read_dist_cm ();
        };
        class Hc_sr04 : public Hc_sr04_fast {
            private:
                //
            protected:
                // the current ultrasound reading (unit is mm)
                uint16_t dist_read_mm;
            public:
                Hc_sr04 (const Hc_sr04 &init_obj);
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04 (const uint8_t init_trig_pin, const uint8_t init_echo_pin);
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                // returns 888 when the returning sound wave is undetectable
                // has better performance under noInterrupts() environment
                uint16_t read_dist_mm ();
                // gets the previous distance read between this ultrasound sensor and the obstacle in front of it (unit is mm)
                uint16_t get_previous_dist_mm ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                // returns 888 when the returning sound wave is undetectable
                // has better performance under noInterrupts() environment
                uint16_t read_dist_cm ();
                // gets the previous distance read between this ultrasound sensor and the obstacle in front of it (unit is cm)
                uint16_t get_previous_dist_cm ();
        };
        class Hc_sr04_test_timer_int {
            private:
                //
            protected:
            // public: // for debugging use only
                // pins that cannot be changed:
                // the pin to trigger a sound wave
                const uint8_t trig_pin;
                uint8_t trig_pin_mask;
                volatile uint8_t *trig_pin_output_register;
                // the pin to read the soundwave sensor
                const uint8_t echo_pin;
                uint8_t echo_pin_mask;
                volatile uint8_t *echo_pin_input_register;
                //
                volatile unsigned long current_tick;
                // also acts as a flag to indicate whether the measurement has ended
                volatile unsigned long ending_tick;
                // saves the distance read from the previous measurement when the current measurement is still in progress
                double prev_dist_read;
                // the time interval between each ISR being run
                double us_per_tick;
            public:
                // #define RAISE_TRIG_PIN() ((*trig_pin_output_register) |= trig_pin_mask)
                // #define TOGGLE_TRIG_PIN() ((*trig_pin_output_register) ^= trig_pin_mask)
                // #define LOWER_TRIG_PIN() ((*trig_pin_output_register) &= ~trig_pin_mask)
                // #define IS_ECHO_PIN_HIGH() ((*echo_pin_input_register) & echo_pin_mask)
                Hc_sr04_test_timer_int (const Hc_sr04_test_timer_int &init_obj);
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04_test_timer_int (const uint8_t init_trig_pin, const uint8_t init_echo_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                virtual void begin ();
                // tells the object the time distance between each isr_individual_sensor_routine() called
                inline void set_time_interval_between_interrupts (const double time_interval_between_interrupts) { us_per_tick = time_interval_between_interrupts; }
                // read the distance of the previous measurement
                // you can also call me when the current measurement is still in progress
                inline double read_previous_dist_cm () {
                    if (ending_tick) { // no need to disable interrupts before reading ending_tick, bc i just treat it as a flag in this line
                        uint8_t oldSREG = SREG;
                        noInterrupts();
                        prev_dist_read = ending_tick * us_per_tick / 58.823; // prevent ending_tick being updated while we are reading it
                        SREG = oldSREG;
                        if (prev_dist_read > 400) prev_dist_read = 888;
                    }
                    return prev_dist_read;
                }
                // only call me by polling in the void loop () after you have get the previous measurement from read_previous_dist_cm()
                // triggers a new sound pulse if the previous measurement has ended
                inline void simple_update () {
                    // if (echo_pin is HIGH) or (ending_tick == 0), just exit the function
                    if (   ((*echo_pin_input_register) & echo_pin_mask) || (!ending_tick)   ) return;

                    (*trig_pin_output_register) &= ~trig_pin_mask; // write trig_pin to LOW
                    delayMicroseconds(2);
                    (*trig_pin_output_register) |= trig_pin_mask; // write trig_pin to HIGH
                    delayMicroseconds(10);
                    (*trig_pin_output_register) &= ~trig_pin_mask; // write trig_pin to LOW
                    //
                    uint8_t oldSREG = SREG;
                    noInterrupts();
                    current_tick = 0;
                    ending_tick = 0;
                    SREG = oldSREG;
                }
                // call me by polling in void loop ()
                inline double polling_update () {
                    read_previous_dist_cm();
                    simple_update();
                    return prev_dist_read;
                }

                // only call me in an ISR for each sensor
                inline void isr_individual_sensor_routine () {
                    // if (echo_pin == HIGH) && (ending_tick == 0)
                        // measurement is in progress
                        // increment to current_tick
                    // elif (echo_pin == LOW) && (ending_tick == 0)
                        // record the value of current_tick to ending_tick
                    // elif (echo_pin == LOW) && (ending_tick != 0)
                        // the echo_pin has lowered but the polling function in void loop hasn't called for a new measurement
                        // nothing to do
                    // elif (echo_pin == HIGH) && (ending_tick != 0)
                        // isr is being ran in simple_update()
                        // noting to do
                    if (!ending_tick) {
                        if ((*echo_pin_input_register) & echo_pin_mask) // {
                            current_tick++;
                        // } else {
                        else
                            ending_tick = current_tick;
                        // }
                    }
                }
        };
    }
}

#endif //#ifndef HC_SR04_H
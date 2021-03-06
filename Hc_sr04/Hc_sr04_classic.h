#ifdef HC_SR04_H

// This library is proved to be working on 15 Mar 2022 at 13:37

#ifndef HC_SR04_CLASSIC_H
#define HC_SR04_CLASSIC_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

// pulseIn is simplified to countPulseASM, so the following libraries are included
#include "wiring_private.h"

namespace yh {
    namespace rec {
        // you may use me when you choose to get distance measured with classic pulseIn(...) function
        class Hc_sr04 {
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
                Hc_sr04 (const Hc_sr04 &init_obj);
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04 (const uint8_t init_trig_pin, const uint8_t init_echo_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // sets the maximum time the program will pause and wait for the sound pulse to return (unit is microseconds)
                void set_max_waiting_time_in_us (const unsigned long assign_max_waiting_time_in_us);
                // sets the maximum range of the sensor (unit is mm)
                void set_max_range_in_mm (const double max_range_in_mm);
                // sets the maximum range of the sensor (unit is cm)
                void set_max_range_in_cm (const double max_range_in_cm);
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                // returns 8888 when the returning sound wave is undetectable
                // has better performance under noInterrupts() environment
                uint16_t read_dist_mm ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                // returns 888 when the returning sound wave is undetectable
                // has better performance under noInterrupts() environment
                uint16_t read_dist_cm ();
        };
        // you may use me when you choose to get distance measured with frequent scans on echo pin with timer interrupts
        class Hc_sr04_timer_int {
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
                // the counter to be incremented every time the ISR is run
                volatile unsigned long ticks;
                // // also acts as a flag to indicate whether the measurement has ended
                // volatile unsigned long ending_tick;
                // the flag to indicate whether the LOW state of echo pin is before or after a measurement
                // also the flag to indicate whether the ISR should save the ticks to ending_tick as an end of a measurement
                volatile uint8_t waiting_for_echo_rise : 1;
                //
                volatile uint8_t measurement_ended : 1;
                // saves the distance read from the previous measurement when the current measurement is still in progress
                double prev_dist_read;
                // the time interval between each ISR being run
                double us_per_tick;
            public:
                // #define RAISE_TRIG_PIN() ((*trig_pin_output_register) |= trig_pin_mask)
                // #define TOGGLE_TRIG_PIN() ((*trig_pin_output_register) ^= trig_pin_mask)
                // #define LOWER_TRIG_PIN() ((*trig_pin_output_register) &= ~trig_pin_mask)
                // #define IS_ECHO_PIN_HIGH() ((*echo_pin_input_register) & echo_pin_mask)
                Hc_sr04_timer_int (const Hc_sr04_timer_int &init_obj);
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04_timer_int (const uint8_t init_trig_pin, const uint8_t init_echo_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // tells the object the time distance between each isr_individual_sensor_routine() called
                inline void set_time_interval_between_interrupts (const double time_interval_between_interrupts) { us_per_tick = time_interval_between_interrupts; }
                // checks if new data is ready @return true when data is ready
                inline bool is_data_ready () { return measurement_ended ? true : false; }
                // checks if new data is not ready @return true when data is not ready
                inline bool is_data_not_ready () { return !measurement_ended; }
                // read the distance of the previous measurement
                // you can also call me when the current measurement is still in progress
                inline double read_previous_dist_cm () {
                    if (measurement_ended) { // no need to disable interrupts before reading measurement_ended, bc i just treat it as a flag in this line
                        uint8_t oldSREG = SREG;
                        noInterrupts();
                        prev_dist_read = ticks * us_per_tick / 58.823; // prevent ticks being updated while we are reading it
                        SREG = oldSREG;
                        if (prev_dist_read > 400) prev_dist_read = 888;
                    }
                    return prev_dist_read;
                }
                // only call me by polling in the void loop () after you have get the previous measurement from read_previous_dist_cm()
                // triggers a new sound pulse if the previous measurement has ended
                inline void simple_update () {
                    // if (echo_pin is HIGH) or (measurement_ended == 0), just exit the function
                    if (   ((*echo_pin_input_register) & echo_pin_mask) || (!measurement_ended)   ) return;

                    (*trig_pin_output_register) &= ~trig_pin_mask; // write trig_pin to LOW
                    delayMicroseconds(2);
                    (*trig_pin_output_register) |= trig_pin_mask; // write trig_pin to HIGH
                    delayMicroseconds(10);
                    (*trig_pin_output_register) &= ~trig_pin_mask; // write trig_pin to LOW
                    //
                    // while (!((*echo_pin_input_register) & echo_pin_mask)) {} // while LOW
                    waiting_for_echo_rise = true; // leave it for the ISR to check the echo pin status
                }
                // call me by polling in void loop ()
                inline double polling_update () {
                    read_previous_dist_cm();
                    simple_update();
                    return prev_dist_read;
                }

                // only call me in an ISR for each sensor
                inline void isr_individual_sensor_routine () __attribute__((__always_inline__)) {
                    if (waiting_for_echo_rise) {
                        if ((*echo_pin_input_register) & echo_pin_mask) {
                            // echo pin has risen
                            // clear the flag
                            waiting_for_echo_rise = false;
                            // kicks off a new measurement
                            ticks = 0;
                            measurement_ended = false;
                        }
                    } else {
                        // if (echo_pin == HIGH) && (measurement_ended == false)
                            // measurement is in progress
                            // increment to ticks
                        // elif (echo_pin == LOW) && (measurement_ended == false)
                            // set the measurement_ended flag on
                        // elif (echo_pin == LOW) && (measurement_ended == true)
                            // the echo_pin has lowered but the polling function in void loop hasn't called for a new measurement
                            // nothing to do
                        // elif (echo_pin == HIGH) && (measurement_ended == true)
                            // isr is being ran in simple_update()
                            // noting to do
                        if (!measurement_ended) {
                            if ((*echo_pin_input_register) & echo_pin_mask) // {
                                ticks++;
                            // } else {
                            else
                                measurement_ended = true;
                            // }
                        }
                    }
                }
        };
        // you may use me when you choose to get distance measured with hardware interrupts or pin change interrupts
        class Hc_sr04_ext_int {
            private:
                //
            protected:
            // public: // for debugging use only

            // flags:
                // starting_time == 0:
                    // new measurement is waiting to start
                // starting_time != 0:
                    // measurement is in progress or finished
                // ending_time == 0:
                    // current measurement is waiting to be finished
                // ending_time != 0:
                    // latest measurement is finished
            // situations:
                // trig_pin triggered, waiting measurement to start     // waiting mode
                // starting_time == 0, ending_time != 0
                // new measurement starts, and is measuring now         // measuring mode
                // starting_time != 0, ending_time == 0
                // measurement finishes                                 // finished mode
                // starting_time != 0, ending_time != 0

                // pins that cannot be changed:
                // the pin to trigger a sound wave
                const uint8_t trig_pin;
                uint8_t trig_pin_mask;
                volatile uint8_t *trig_pin_output_register;
                // the pin to read the soundwave sensor
                const uint8_t echo_pin;
                uint8_t echo_pin_mask;
                volatile uint8_t *echo_pin_input_register;
                // the time the measurement starts
                unsigned long starting_time;
                // the time the pulse returns
                // also acts as a flag to indicate whether the measurement has ended
                volatile unsigned long ending_time;
                // saves the distance read from the previous measurement when the current measurement is still in progress
                double prev_dist_read;
            public:
                // #define RAISE_TRIG_PIN() ((*trig_pin_output_register) |= trig_pin_mask)
                // #define TOGGLE_TRIG_PIN() ((*trig_pin_output_register) ^= trig_pin_mask)
                // #define LOWER_TRIG_PIN() ((*trig_pin_output_register) &= ~trig_pin_mask)
                // #define IS_ECHO_PIN_HIGH() ((*echo_pin_input_register) & echo_pin_mask)
                Hc_sr04_ext_int (const Hc_sr04_ext_int &init_obj);
                // inits the trigger pin number and echo pin number to this Uts object
                Hc_sr04_ext_int (const uint8_t init_trig_pin, const uint8_t init_echo_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                virtual void begin ();

                // checks if new data is ready @return true when data is ready
                inline bool is_data_ready () { return ending_time ? true : false; }

                // checks if new data is not ready @return true when data is not ready
                inline bool is_data_not_ready () { return !ending_time; }

                // read the distance of the previous measurement
                // you can also call me when the current measurement is still in progress
                inline double read_previous_dist_mm () {
                    if (starting_time && ending_time) { // no need to disable interrupts before reading ending_tick, bc i just treat it as a flag in this line
                        uint8_t oldSREG = SREG;
                        noInterrupts();
                        prev_dist_read = (ending_time - starting_time) * 0.17; // prevent ending_time being updated while we are reading it
                        SREG = oldSREG;
                        if (prev_dist_read > 4000) prev_dist_read = 8888;
                    }
                    return prev_dist_read;
                }

                // read the distance of the previous measurement
                // you can also call me when the current measurement is still in progress
                inline double read_previous_dist_cm () {
                    if (starting_time && ending_time) { // no need to disable interrupts before reading ending_tick, bc i just treat it as a flag in this line
                        uint8_t oldSREG = SREG;
                        noInterrupts();
                        prev_dist_read = (ending_time - starting_time) * 0.017; // prevent ending_time being updated while we are reading it
                        SREG = oldSREG;
                        if (prev_dist_read > 400) prev_dist_read = 888;
                    }
                    return prev_dist_read;
                }

                // only call me by polling in the void loop () after you have get the previous measurement from read_previous_dist_cm()
                // triggers a new sound pulse if the previous measurement has ended
                inline void simple_update () {
                    // if (echo_pin is HIGH) or (measurement not finished), just exit the function
                    if (   ((*echo_pin_input_register) & echo_pin_mask) || (!(starting_time && ending_time))   ) return;

                    (*trig_pin_output_register) &= ~trig_pin_mask; // write trig_pin to LOW
                    delayMicroseconds(2);
                    (*trig_pin_output_register) |= trig_pin_mask; // write trig_pin to HIGH
                    delayMicroseconds(10);
                    (*trig_pin_output_register) &= ~trig_pin_mask; // write trig_pin to LOW
                    uint8_t oldSREG = SREG;
                    noInterrupts();
                    starting_time = 0; // configures the flag into waiting mode
                    SREG = oldSREG;
                }

                // call me by polling in void loop ()
                inline double polling_update () {
                    read_previous_dist_cm();
                    simple_update();
                    return prev_dist_read;
                }

                // only call me in an ISR for each sensor
                // This method should be used in CHANGE mode of the interrupt (or PCINTs).
                // This method does not check if the echo pin has really changed,
                // but treats the current state of echo pin as the state of echo pin after it is changed.
                inline void isr_individual_sensor_routine () __attribute__((__always_inline__)) {
                    if ((*echo_pin_input_register) & echo_pin_mask) {
                        // pin rises
                        if (!starting_time) { // configures the flag into measuring mode
                            starting_time = micros();
                            ending_time = 0;
                        }
                    } else {
                        // pin falls
                        if (starting_time && (!ending_time)) ending_time = micros(); // logs the falling time, which also configures the flag into finished mode
                    }
                }

                // // only call me in an ISR for each sensor
                // // this method also checks the status of echo pin, which is suitable for PCINTs
                // inline void isr_individual_sensor_routine_with_checking_echo_status () __attribute__((__always_inline__)) {
                //     if (!ending_time && (!((*echo_pin_input_register) & echo_pin_mask)) ) ending_time = micros();
                // }
        };
    }
}

#endif // #ifndef HC_SR04_CLASSIC_H

#endif // #ifdef HC_SR04_H
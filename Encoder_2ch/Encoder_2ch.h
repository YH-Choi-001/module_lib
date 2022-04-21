#ifndef ENCODER_2CH_H
#define ENCODER_2CH_H

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

//                      <-- anti-clockwise                      clockwise -->

// 1-channel encoders
// able to tell at rest or in motion
//   ___     ___     ___     ___     ___     ___     ___     ___     ___     ___     ___     ___     ___     ___
// _|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___

// 2-channel encoders
// able to absolutely tell -1, 0, +1 units of displacement
//       _____       _____       _____       _____       _____       _____       _____       _____       _____
// A:  _|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____
//          _____       _____       _____       _____       _____       _____       _____       _____       _____
// B:  ____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |__


// 3-channel encoders
// able to absolutely tell -2, -1, 0, +1, +2 units of displacement
//       _____       _____       _____       _____       _____       _____       _____       _____       _____
// A:  _|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____
//         _____       _____       _____       _____       _____       _____       _____       _____       _____
// B:  ___|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |___
//           _____       _____       _____       _____       _____       _____       _____       _____       _____
// C:  _____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_

// reminder:



// Motions:

// equations of motions:
// s = (u + v)t / 2
// v = u + at
// s = ut + (1/2)a(t^2)
// v^2 = (u^2) + 2as
// where s is displacement, u is initial velocity, v is final velocity, a is acceleration, t is time
// the 4 equations above only apply when acceleration of the whole process is constant (or linear approximations like what I did in gy521 library)

// By the observation of Mr. William Ng,
// to find 1 unknown, at least 3 values must be known.
// They might be hidden in the question. Be awared.



// Newton's first law: (inertia)
// An object is always at rest or in uniform motion, unless a force is applied.

// Newton's second law: (force)
// Force applied is proportional to mass of object
// Force applied is proportional to acceleration of object
// F = ma
// where m is mass of object, a is acceleration of object

// Newton's third law: (F = -f)
// When a force is applied to an object, the applier of force experiences a force of the same magnitude but opposite direction.
// These 2 forces form an Action and reaction pair.



// Moment:

// equations of moment:
// consider the diagram below:
//
//           \ 
//            \ 
//             \ 
//              \ 
//               \ 
//                \ 
//                _\|  applied force F
//   O-----------------
//   |--------------|
//      distance d
//
//
//                  |
//                  |
//                  |
//                  |
//                  |
//                  |
//                 \|/ perpendicular vector of applied force
//
//                 \ 
//           ------- parallel vector of applied force
//                 /
//   O-----------------
//   |--------------|
//      distance d
//
// moment = perpendicular F * d = F * perpendicular d

// Definition of an object at rest:
// An object is defined as at rest when it's net velocity is 0, and its net moments is 0.

// Net moment of an object = sum of clockwise moments + sum of anti-clockwise moments

// Energy:

// Conservation of energy:
// The sum of energy in the universe is a constant. Energy can change from 1 form to another, but cannot be created nor destroyed.

// equations of energy:
// PE = mg∆h
// KE = Fs = (1/2)m(v^2)
// where m is mass of object, g is acceleration due to gravity (9.81 ms^-2), ∆h is change of height,
// F is force applied on object, s is displacement of object while force is applied, v is velocity of object

// power:
// P = E / t = Fv
// where P is power, E is energy transferred (or work done), t is time taken, F is force applied on object, v is constant velocity of object



// Momentum (completely unrelated to moment):

// Conservation of momentum:
// The total momentum of a system is conserved, provided that no net external force is acting on it.

// equations of momentum:
// ma ua + mb ub = ma va + mb vb
// where ma is mass of object A, mb is mass of object B,
// ua is initial velocity of object A, ub is initial velocity of object B, va is final velocity of object A, vb is final velocity of object B

namespace yh {
    namespace rec {
        #define DISPLACEMENT_UNIT int32_t
        // for 2-channel rotary encoder use
        class Encoder_2ch_timer_int {
            private:
                //
            protected:
                // pins that cannot be changed:
                // signal A pin
                const uint8_t signal_A_pin;
                volatile uint8_t *signal_A_input_reg;
                uint8_t signal_A_mask;
                // signal B pin
                const uint8_t signal_B_pin;
                volatile uint8_t *signal_B_input_reg;
                uint8_t signal_B_mask;
                // stores the previous state of channel A and channel B
                uint8_t prev_A_state, prev_B_state;
                // stores the previous displacements for velocity calculations use
                DISPLACEMENT_UNIT *logged_displacements;
                // indicates the length of the logged_displacements array
                const uint16_t log_len;
                // the current index saving the displacements
                uint16_t log_idx;
                // stores the current displacement of the encoder
                DISPLACEMENT_UNIT current_displacement;

                // By the definition of velocity: v = ∆s / ∆t
                // the instantaneous velocity may be found using 2 ways:
                // v =  1 / ∆t
                // v = ∆s / 1

                // v =  1 / ∆t measures the difference of time between every unit of displacement moved
                // it will become less responsive and buggy if the encoder is spinning at a low speed
                // this method uses PCINT or hardware interrupts to calculate velocity

                // v = ∆s / 1 measures the difference of displacement between every fixed time period
                // it will not become less responsive and buggy, but it has a delay situation
                // this method uses timer interrupts to calculate velocity

                // stores the instantaneous velocity of the encoder
                int16_t instantaneous_velocity;
                // stores the instantaneous acceleration of the encoder
                int16_t instantaneous_acceleration;
            public:
                Encoder_2ch_timer_int (Encoder_2ch_timer_int &init_obj);
                // inits the arguments to the signal A and B pins, and the length of displacement stored
                Encoder_2ch_timer_int (const uint8_t init_signal_A_pin, const uint8_t init_signal_B_pin, const uint16_t request_log_len = 400);
                // frees the memory allocated
                ~Encoder_2ch_timer_int ();
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // gets the current displacement of the encoder
                inline int16_t get_displacement () __attribute__((__always_inline__)) { return current_displacement; }
                // gets the instantaneous velocity of the encoder
                // the value of instantaneous_velocity increases as request_log_len in constructor increases
                inline int16_t get_instantaneous_velocity () __attribute__((__always_inline__)) { return instantaneous_velocity; }
                // reset current displacement as 0
                void reset_displacement () {
                    // clear global interrupt bit to prevent isr_individual_sensor_routine() method from running
                    uint8_t oldSREG = SREG;
                    noInterrupts();
                    for (uint16_t i = 0; i < log_len; i++) {
                        logged_displacements[i] -= current_displacement;
                    }
                    current_displacement = 0;
                    // re-enable interrupts
                    SREG = oldSREG;
                }
                // only call me in an ISR for each sensor
                inline void isr_individual_sensor_routine () __attribute__((__always_inline__)) {
                    // stores the current state of channel A and channel B
                    const uint8_t curr_A_state = (*signal_A_input_reg & signal_A_mask);
                    const uint8_t curr_B_state = (*signal_B_input_reg & signal_B_mask);
                    // implementation

                    // if A_rise && B_is_low
                    // +1
                    // if B_rise && A_is_high
                    // +1
                    // if A_fall && B_is_high
                    // +1
                    // if B_fall && A_is_low
                    // +1

                    // if A_rise && B_is_high
                    // -1
                    // if B_rise && A_is_low
                    // -1
                    // if A_fall && B_is_low
                    // -1
                    // if B_fall && A_is_high
                    // -1


                    // re-arranged

                    // if A_is_changed
                    // if A_is_high_now && B_is_low
                    // +1
                    // if A_is_low_now && B_is_high
                    // +1
                    // if A_is_high_now && B_is_high
                    // -1
                    // if A_is_low_now && B_is_low
                    // -1

                    // if B_is_changed
                    // if B_is_high_now && A_is_high
                    // +1
                    // if B_is_low_now && A_is_low
                    // +1
                    // if B_is_high_now && A_is_low
                    // -1
                    // if B_is_low_now && A_is_high
                    // -1

                    if ( (prev_A_state ^ curr_A_state) && (prev_B_state ^ curr_B_state) ) {
                        // both A has B has changed
                        // crashed (either +2 or -2)
                        current_displacement += (instantaneous_velocity > 0) ? (+2) : (-2);
                    } else if (prev_A_state ^ curr_A_state) {
                        // A has changed
                        current_displacement += (curr_A_state ^ curr_B_state) ? (+1) : (-1);
                    } else if (prev_B_state ^ curr_B_state) {
                        // B has changed
                        current_displacement += (curr_A_state ^ curr_B_state) ? (-1) : (+1);
                    }
                    instantaneous_acceleration = -instantaneous_velocity;
                    instantaneous_velocity = current_displacement - logged_displacements[log_idx]; // v = ∆s / ∆t (let ∆t = 1), v = ∆s = s' - s
                    instantaneous_acceleration += instantaneous_velocity; // a = ∆v / ∆t (let ∆t = 1), a = ∆v = v - u
                    logged_displacements[log_idx++] = current_displacement;
                    if (log_idx == log_len) log_idx = 0;
                    prev_A_state = curr_A_state;
                    prev_B_state = curr_B_state;
                }
        };
        // for 2-channel rotary encoder use (memory saving)
        class Encoder_2ch_timer_int_light {
            private:
                //
            protected:
                // pins that cannot be changed:
                // signal A pin
                const uint8_t signal_A_pin;
                volatile uint8_t *signal_A_input_reg;
                uint8_t signal_A_mask;
                // signal B pin
                const uint8_t signal_B_pin;
                volatile uint8_t *signal_B_input_reg;
                uint8_t signal_B_mask;
                // stores the previous state of channel A and channel B
                uint8_t prev_A_state, prev_B_state;
                // indicates the length of the logged_displacements array
                const uint16_t log_len;
                // the current index saving the displacements
                uint16_t log_idx;
                // stores the current displacement of the encoder
                int16_t current_displacement;
                // stores the instantaneous velocity of the encoder
                int16_t instantaneous_velocity;
            public:
                Encoder_2ch_timer_int_light (Encoder_2ch_timer_int_light &init_obj);
                // inits the arguments to the signal A and B pins, and the length of displacement stored
                Encoder_2ch_timer_int_light (const uint8_t init_signal_A_pin, const uint8_t init_signal_B_pin, const uint16_t request_log_len = 400);
                // frees the memory allocated
                ~Encoder_2ch_timer_int_light ();
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // gets the instantaneous velocity of the encoder
                // the value of instantaneous_velocity increases as request_log_len in constructor increases
                inline int16_t get_instantaneous_velocity () __attribute__((__always_inline__)) { return instantaneous_velocity; }
                // only call me in an ISR for each sensor
                inline void isr_individual_sensor_routine () __attribute__((__always_inline__)) {
                    // stores the current state of channel A and channel B
                    const uint8_t curr_A_state = (*signal_A_input_reg & signal_A_mask);
                    const uint8_t curr_B_state = (*signal_B_input_reg & signal_B_mask);

                    // if A_is_changed
                    // if A_is_high_now && B_is_low
                    // +1
                    // if A_is_low_now && B_is_high
                    // +1
                    // if A_is_high_now && B_is_high
                    // -1
                    // if A_is_low_now && B_is_low
                    // -1

                    // if B_is_changed
                    // if B_is_high_now && A_is_high
                    // +1
                    // if B_is_low_now && A_is_low
                    // +1
                    // if B_is_high_now && A_is_low
                    // -1
                    // if B_is_low_now && A_is_high
                    // -1

                    if ( (prev_A_state ^ curr_A_state) && (prev_B_state ^ curr_B_state) ) {
                        // both A has B has changed
                        // crashed (either +2 or -2)
                        current_displacement += (instantaneous_velocity > 0) ? (+2) : (-2);
                    } else if (prev_A_state ^ curr_A_state) {
                        // A has changed
                        current_displacement += (curr_A_state ^ curr_B_state) ? (+1) : (-1);
                    } else if (prev_B_state ^ curr_B_state) {
                        // B has changed
                        current_displacement += (curr_A_state ^ curr_B_state) ? (-1) : (+1);
                    }
                    log_idx++;
                    if (log_idx == log_len) {
                        instantaneous_velocity = current_displacement;
                        log_idx = 0;
                    }
                    prev_A_state = curr_A_state;
                    prev_B_state = curr_B_state;
                }
        };
        class Encoder_2ch_ext_int {
            private:
                //
            protected:
                // pins that cannot be changed:
                // signal A pin
                const uint8_t signal_A_pin;
                volatile uint8_t *signal_A_input_reg;
                uint8_t signal_A_mask;
                // signal B pin
                const uint8_t signal_B_pin;
                volatile uint8_t *signal_B_input_reg;
                uint8_t signal_B_mask;
                // stores the previous state of channel A and channel B
                uint8_t prev_A_state, prev_B_state;
                //
                double prev_time;

                // stores the instantaneous velocity of the encoder
                double instantaneous_velocity;
                // stores the instantaneous acceleration of the encoder
                double instantaneous_acceleration;
            public:
                Encoder_2ch_ext_int (Encoder_2ch_ext_int &init_obj);
                // inits the arguments to the signal A and B pins, and the length of displacement stored
                Encoder_2ch_ext_int (const uint8_t init_signal_A_pin, const uint8_t init_signal_B_pin);
                // frees the memory allocated
                ~Encoder_2ch_ext_int ();
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // only call me in an ISR for each sensor
                inline void isr_individual_sensor_routine () __attribute__((__always_inline__)) {
                    // stores the current state of channel A and channel B
                    const uint8_t curr_A_state = (*signal_A_input_reg & signal_A_mask);
                    const uint8_t curr_B_state = (*signal_B_input_reg & signal_B_mask);
                    int8_t delta_displacement;
                    const double delta_time = micros() - prev_time;
                    if ( (prev_A_state ^ curr_A_state) && (prev_B_state ^ curr_B_state) ) {
                        // both A has B has changed
                        // crashed (either +2 or -2)
                        delta_displacement = (instantaneous_velocity > 0) ? (+2) : (-2);
                    } else if (prev_A_state ^ curr_A_state) {
                        // A has changed
                        delta_displacement = (curr_A_state ^ curr_B_state) ? (+1) : (-1);
                    } else if (prev_B_state ^ curr_B_state) {
                        // B has changed
                        delta_displacement = (curr_A_state ^ curr_B_state) ? (-1) : (+1);
                    }
                    const double new_instantaneous_velocity = delta_displacement / delta_time;
                    instantaneous_acceleration = (new_instantaneous_velocity - instantaneous_velocity) / delta_time;
                    instantaneous_velocity = new_instantaneous_velocity;
                    prev_time += delta_time;
                    prev_A_state = curr_A_state;
                    prev_B_state = curr_B_state;
                }
        };
    }
}

#endif // #ifndef ENCODER_2CH_H
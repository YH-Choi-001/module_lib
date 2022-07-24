#ifndef PID_H
#define PID_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        class Pid {
            private:
                //
            protected:
                //
                double prev_error;
                // p, i, d values
                double p, i, d;
                // weighting of p, i, d correction
                double p_weighting, i_weighting, d_weighting;
                //
                unsigned long prev_time;
            public:
                // default constructor to init 0 for basic values, and 1 for default weighting values
                Pid () : prev_error(0), p(0), i(0), d(0), p_weighting(1), i_weighting(1), d_weighting(1), prev_time(0) {}
                // inits the 3 arguments to the respective weighting values
                Pid (const double init_p_weight, const double init_i_weight, const double init_d_weight) :
                prev_error(0), p(0), i(0), d(0), p_weighting(init_p_weight), i_weighting(init_i_weight), d_weighting(init_d_weight), prev_time(0) {}
                // kicks off the prev_time
                void begin () {
                    prev_time = micros();
                }
                // sets the p_weighting value
                inline void set_p_weighting (const double request_p_weighting) __attribute__((__always_inline__)) {
                    p_weighting = (request_p_weighting < 0) ? 0 : request_p_weighting;
                }
                // sets the i_weighting value
                inline void set_i_weighting (const double request_i_weighting) __attribute__((__always_inline__)) {
                    i_weighting = (request_i_weighting < 0) ? 0 : request_i_weighting;
                }
                // sets the d_weighting value
                inline void set_d_weighting (const double request_d_weighting) __attribute__((__always_inline__)) {
                    d_weighting = (request_d_weighting < 0) ? 0 : request_d_weighting;
                }
                // gets the p correction value
                inline double get_p () __attribute__((__always_inline__)) { return p; }
                // gets the i correction value
                inline double get_i () __attribute__((__always_inline__)) { return i; }
                // gets the d correction value
                inline double get_d () __attribute__((__always_inline__)) { return d; }
                // gets the sum correction value
                inline double get_corr () __attribute__((__always_inline__)) { return p + i + d; }
                // updates the correction value
                double update_corr (const double target_val, const double actual_val, const double time_diff) {
                    p = target_val - actual_val;
                    i += p * time_diff * i_weighting;
                    d = (p - prev_error) / time_diff * d_weighting;
                    prev_error = p;
                    p *= p_weighting;
                    return p + i + d;
                }
                // updates the correction value with built-in time tracker
                double update_corr (const double target_val, const double actual_val) {
                    const unsigned long time_diff = micros() - prev_time;
                    p = target_val - actual_val;
                    i += p * time_diff * i_weighting;
                    d = (p - prev_error) / time_diff * d_weighting;
                    prev_error = p;
                    p *= p_weighting;
                    prev_time += time_diff;
                    return p + i + d;
                }
                // resets the i correction value to 0
                inline void reset_i () __attribute__((__always_inline__)) { i = 0; }
        };
    }
}

#endif // #ifndef PID_H
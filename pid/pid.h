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
            public:
                // default constructor to init 0 for basic values, and 1 for default weighting values
                Pid () : prev_error(0), p(0), i(0), d(0), p_weighting(1), i_weighting(1), d_weighting(1) {}
                // inits the 3 arguments to the respective weighting values
                Pid (const double init_p_weight, const double init_i_weight, const double init_d_weight) :
                prev_error(0), p(0), i(0), d(0), p_weighting(init_p_weight), i_weighting(init_i_weight), d_weighting(init_d_weight) {}
                // void update_pid_val (const double input_target_val, const double input_reality_val) {
                //     target_val = input_target_val;
                //     reality_val = input_reality_val;
                //     error = reality_val - target_val; // error is +ive when reality_val > target_val, it is -ive when reality < target_val
                //     p = -error; // definition of p: p == -error
                //     const double duration = micros() - previous_micros;
                //     i += (-error) * duration; // i == the area under the curve of the error graph 積分
                //     d = -(error - previous_error) / duration; // d == rate of change of error in the error graph 微分
                //     previous_micros += duration;
                //     previous_error = error;
                // }

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
                // resets the i correction value to 0
                inline void reset_i () __attribute__((__always_inline__)) { i = 0; }
        };
    }
}

#endif // #ifndef PID_H
#ifndef FF_PID_H
#define FF_PID_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        #error ALL ITEMS UNDER NAMESPACE FF ARE NOT ALLOWED TO USE UNTIL VERIFIED
        namespace ff {
            class Pid {
                private:
                    //
                protected:
                    //
                    double target_val;
                    double reality_val;
                    double error, previous_error;
                    //
                    unsigned long previous_micros;
                public:
                    // for user to access p, i, d values
                    double p, i, d;
                    // constructor to init 0 for the values
                    Pid () : previous_micros(0), target_val(0), reality_val(0), error(0), previous_error(0), p(0), i(0), d(0) {}
                    void update_pid_val (const double input_target_val, const double input_reality_val) {
                        target_val = input_target_val;
                        reality_val = input_reality_val;
                        error = reality_val - target_val; // error is +ive when reality_val > target_val, it is -ive when reality < target_val
                        p = -error; // definition of p: p == -error
                        const double duration = micros() - previous_micros;
                        i += (-error) * duration; // i == the area under the curve of the error graph 積分
                        d = -(error - previous_error) / duration; // d == rate of change of error in the error graph 微分
                        previous_micros += duration;
                        previous_error = error;
                    }
            };
        }
    }
}

#endif // #ifndef FF_PID_H
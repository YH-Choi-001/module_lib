#ifndef IR_32U4_H
#define IR_32U4_H __DATE__ ", " __TIME__

#include <Arduino.h>

#define IR_CHANNEL 12

namespace yh {
    namespace rec {
        namespace ir_32u4s {
            extern uint8_t ball_intensity [];
            // kicks off everything in the ADC
            void begin ();
            //
            void update_ball_intensity (uint8_t channel);
            //
            uint8_t get_sun_intensity (uint8_t channel);
        }
    }
}

#endif // #ifndef IR_32U4_H
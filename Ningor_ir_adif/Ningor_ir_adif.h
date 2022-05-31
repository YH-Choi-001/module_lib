#ifndef NINGOR_IR_ADIF_H
#define NINGOR_IR_ADIF_H __DATE__ ", " __TIME__

#include <Arduino.h>

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

namespace yh {
    namespace rec {
        class Ningor_ir_adif {
            private:
                //
            protected:
                //
            public:
                //
                uint16_t ball_dir;
                //
                void begin ();
                // read unfiltered IR signal
                uint16_t read_sun (const uint8_t channel);
                // read IR signal filtered for RCJ soccer ball
                uint16_t read_ball (const uint8_t channel);
                // get the direction of the ball in degrees
                uint16_t get_ball_dir ();
                // get the magnitude of the ball
                uint16_t get_ball_mag ();
        };
    }
}

#else // #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#error Ningor_ir_adif library is only for Arduino Mega or Mega 2560
#endif // #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#endif // #ifndef NINGOR_IR_ADIF_H
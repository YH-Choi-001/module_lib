#ifndef SEN1930_H
#define SEN1930_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        class Sen1930 {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pin to read the brightness sensor value
                const uint8_t read_gryscl_pin;
                // the value that decides if the sensor is touching white
                uint16_t lower_limit_of_white;
                // the current gryscl sensor reading
                uint16_t gryscl_read;
            public:
                // inits the pin number of the reading pin to this Gryscl object
                Sen1930 (const uint8_t init_read_gryscl_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                inline void begin ();
                // sets the value to decide whether the sensor is touching white
                void set_lower_limit_of_white (const uint16_t input_lower_limit_of_white);
                // functions that load raw data for other functions to further process
                uint16_t raw_read_gry_value ();
                // returns 1 if the sensor is determined touching white, else returns 0
                bool touch_white (const bool refresh = true);
        };
    }
}

#endif //#ifndef SEN1930_H
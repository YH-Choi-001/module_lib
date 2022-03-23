// This library is proved to be working on 13 Feb 2022 at 16:53

#ifndef SEN_1930_H
#define SEN_1930_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        class Sen_1930 {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pin to read the brightness sensor value
                const uint8_t read_gryscl_pin;
                // the value that decides if the sensor is touching black or white
                uint16_t bnw_threshold;
            public:
                // inits the pin number of the reading pin to this Gryscl object
                Sen_1930 (const uint8_t init_read_gryscl_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // sets the value to decide whether the sensor is touching white
                void set_threshold (const uint16_t input_bnw_threshold);
                // functions that load raw data for other functions to further process
                virtual uint16_t raw_read_gry_value ();
                // returns 1 if the sensor is determined touching black, else returns 0
                virtual bool touch_black ();
                // returns 1 if the sensor is determined touching white, else returns 0
                virtual bool touch_white ();
        };
    }
}

#endif //#ifndef SEN_1930_H
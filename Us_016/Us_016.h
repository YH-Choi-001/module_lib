// This library is proved to be working on 5 Feb 2022 at 18:40

#ifndef US_016_H
#define US_016_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#ifndef NC_PINNO
#define NC_PINNO 255 // pin number of a not-connected pin
#endif

namespace yh {
    namespace rec {
        class Us_016 {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pin to trigger a sound wave
                const uint8_t range_pin;
                uint8_t range_pin_mask;
                volatile uint8_t *range_pin_output_port;
                // the pin to read the soundwave sensor
                const uint8_t out_pin;
                // the flag to indicate if the resolution is 3 millimeters
                bool is_resolution_3mm;
            public:
                Us_016 (const Us_016 &init_obj);
                // inits the range pin number and out pin number to this Uts object
                Us_016 (const uint8_t init_range_pin, const uint8_t init_out_pin, const uint8_t init_resolution = 3);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                virtual uint16_t read_dist_mm ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                virtual uint16_t read_dist_cm ();
                // set the resolution to either 1 or 3 millimeters
                void set_resolution (const uint8_t resolution);
        };
    }
}

#endif //#ifndef US_016_H
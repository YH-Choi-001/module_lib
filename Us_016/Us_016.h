#ifndef US_016_H
#define US_016_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        class Us_016_fast {
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
                // the flag to indicate if the resolution is 3 meters
                bool is_resolution_3m;
            public:
                Us_016_fast (const Us_016_fast &init_obj);
                // inits both range pin and out pin with one argument only
                // you should combine 2 pins like this: (range_pin << 8) | out_pin
                Us_016_fast (const uint32_t init_range_and_out_pin_and_resolution);
                // inits the range pin number and out pin number to this Uts object
                Us_016_fast (const uint8_t init_range_pin, const uint8_t init_out_pin, const uint8_t init_resolution = 3);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                virtual uint16_t read_dist_mm ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                virtual uint16_t read_dist_cm ();
                // set the resolution to either 1 or 3 meters
                void set_resolution (const uint8_t resolution);
        };
        class Us_016 : public Us_016_fast {
            private:
                //
            protected:
                // the current ultrasound reading (unit is mm or 3mm)
                uint16_t raw_dist_read;
            public:
                Us_016 (const Us_016 &init_obj);
                // inits both range pin and out pin with one argument only
                // you should combine 2 pins like this: (range_pin << 8) | out_pin
                Us_016 (const uint32_t init_range_and_out_pin_and_resolution);
                // inits the range pin number and out pin number to this Uts object
                Us_016 (const uint8_t init_range_pin, const uint8_t init_out_pin, const uint8_t init_resolution = 3);
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                uint16_t read_dist_mm ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is mm)
                uint16_t read_dist_mm (const bool refresh);
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                uint16_t read_dist_cm ();
                // reads the distance between this ultrasound sensor and the obstacle in front of it (unit is cm)
                uint16_t read_dist_cm (const bool refresh);
        };
    }
}

#endif //#ifndef US_016_H
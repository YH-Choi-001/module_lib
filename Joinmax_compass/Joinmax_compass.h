#ifndef JOINMAX_COMPASS_H
#define JOINMAX_COMPASS_H __DATE__ ", " __TIME__

#include <Arduino.h>
#include <Wire.h>

namespace yh {
    namespace rec {
        class Compass {
            private:
                //
            protected:
                //
                const uint8_t i2c_address;
            public:
                Compass (const uint8_t init_i2c_address = 0x3B);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // configures the settings of the I2C bus and the chip
                virtual void begin ();
                // @return the heading of the compass relative to the north set by clicking its button
                uint16_t get_heading ();
        };
    }
}

#endif // #ifndef JOINMAX_COMPASS_H
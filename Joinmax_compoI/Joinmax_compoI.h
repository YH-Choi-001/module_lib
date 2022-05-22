#ifndef JOINMAX_COMPOI_H
#define JOINMAX_COMPOI_H __DATE__ ", " __TIME__

#include <Arduino.h>
#include <Wire.h>

namespace yh {
    namespace rec {
        class CompoI {
            private:
                //
            protected:
                //
                const uint8_t i2c_address;
                uint8_t command (const uint8_t cmd);
            public:
                CompoI (const uint8_t init_i2c_address = 0x01);
                //
                void begin ();
                //
                uint8_t get_channel_val (const uint8_t channel);
                //
                uint8_t get_max_idx ();
                //
                uint8_t get_max_val ();
                //
                uint8_t get_min_idx ();
                //
                uint8_t get_min_val ();
                //
                uint8_t get_avg_val ();
                //
                uint8_t set_filter_on ();
                //
                uint8_t set_filter_off ();
                //
                uint8_t calibrate ();
        };
    }
}

#endif // #ifndef JOINMAX_COMPOI_H
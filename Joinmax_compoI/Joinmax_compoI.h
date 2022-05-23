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
                virtual uint8_t command (const uint8_t cmd);
            public:
                CompoI (const uint8_t init_i2c_address = 0x01);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // configures the settings of the I2C bus and the chip
                virtual void begin ();
                // @param channel selects the channel of compound-eye to read its value
                // @return the IR intensity read by the photodiodes of the selected channel
                uint8_t get_channel_val (const uint8_t channel);
                // @return the no. of photodiode that reads the strongest IR intensity
                uint8_t get_max_idx ();
                // @return the strongest IR intensity among the readings of the photodiodes of all channels
                uint8_t get_max_val ();
                // @return the no. of photodiode that reads the weakest IR intensity
                uint8_t get_min_idx ();
                // @return the weakest IR intensity among the readings of the photodiodes of all channels
                uint8_t get_min_val ();
                // @return the mean average IR intensity among the readings of the photodiodes of all channels
                uint8_t get_avg_val ();
                // enters ball-filtering mode
                // environmental IR signals will be removed from readings of photodiodes
                uint8_t set_filter_on ();
                // quits ball-filtering mode
                // readings of photodiodes are unprocessed raw values
                uint8_t set_filter_off ();
                // initializes the hardware
                // completed by factory, no need for user to call this method
                uint8_t calibrate ();
        };
    }
}

#endif // #ifndef JOINMAX_COMPOI_H
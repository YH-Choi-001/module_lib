#ifndef Z4HC595_H
#define Z4HC595_H __DATE__ ", " __TIME__

#include <Arduino.h>
#include <SPI.h>

#ifndef NC_PINNO
#define NC_PINNO 255
#endif

namespace yh {
    namespace rec {
        class Z4hc595 {
            private:
                //
            protected:
                // pins that cannot be changed:
                const uint8_t
                    shcp_pin,
                    ds_pin,
                    stcp_pin;
                // SPI settings
                static const SPISettings default_spi_settings;
                // the flag to indicate if user uses hardware SPI ports or bitbang-SPI (which is shiftOut)
                unsigned int use_hardware_spi : 1;
            public:
                // use this constructor for hardware SPI ports
                // @param stcp_pin latch pin (slave select)
                Z4hc595 (const uint8_t init_ss_pin);
                // use this constructor for bitbang-SPI (which is shiftOut)
                // @param shcp_pin clock pin (serial clock)
                // @param ds_pin data pin    (SDI / MOSI)
                // @param stcp_pin latch pin (slave select)
                Z4hc595 (const uint8_t init_shcp_pin, const uint8_t init_ds_pin, const uint8_t init_stcp_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // left-shift the outputs by 1 bit, and input new bit value of q0 (0 or 1 only)
                void left_shift (const uint8_t q0_new_val);
                // write the status of all 8 ports
                void write (const uint8_t q7_to_q0_new_vals);
        };
    }
}

#endif // #ifndef Z4HC595_H
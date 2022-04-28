#ifndef SOFT_SPI_H
#define SOFT_SPI_H

#include <Arduino.h>

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

namespace yh {
    namespace rec {
        class Soft_SPI {
            private:
                //
            protected:
                // SCK pin
                volatile uint8_t *const sck_port;
                const uint8_t sck_mask;
                // MOSI pin
                volatile uint8_t *const mosi_port;
                const uint8_t mosi_mask;
                const uint8_t inverted_mosi_mask;
                // MISO pin
                volatile uint8_t *const miso_port;
                const uint8_t miso_mask;
                // 1 if LSBFIRST, 0 if MSBFIRST
                unsigned int is_LSB_first : 1;
                // CPOL << 1 | CPHA
                unsigned int data_mode : 2;
                // clock divider
                unsigned int clock_div : 3;
            public:
                // (PORTB, 0x01, PORTB, 0x02, PINB, 0x04)
                Soft_SPI
                (
                    volatile uint8_t *const init_sck_port,
                    const uint8_t init_sck_mask,
                    volatile uint8_t *const init_mosi_port,
                    const uint8_t init_mosi_mask,
                    volatile uint8_t *const init_miso_port,
                    const uint8_t init_miso_mask
                )
                __attribute__((__always_inline__))
                :   // initializer list
                    sck_port (init_sck_port) ,
                    sck_mask (init_sck_mask) ,
                    mosi_port(init_mosi_port),
                    mosi_mask(init_mosi_mask),
                    miso_port(init_miso_port),
                    miso_mask(init_miso_mask),
                    inverted_mosi_mask(~init_mosi_mask),
                    is_LSB_first(0),
                    data_mode(0),
                    clock_div(0)
                {
                    //
                }
                // configures the state of the pins
                // you should call the pinMode() function on every pin used before calling me
                void begin () {
                    // sets the default state of SCK pin
                    if (data_mode & 0x02) // CPOL == 1
                        (*sck_port) |= sck_mask;
                    // else // CPOL == 0
                    if (!(data_mode & 0x02)) // CPOL == 0
                        (*sck_port) &= ~sck_mask;
                }
                //
                void end () {
                    //
                }
                //
                inline uint8_t transfer (const uint8_t passing_data) {
                    // by default MSBFIRST
                    uint8_t data_mask = 0x80;
                    uint8_t data_in = 0;
                    uint8_t data_out = 0;
                    if (is_LSB_first) {
                        // swap the location of bits
                        for (uint8_t i = 0; i < 8; i++) {
                            if ( passing_data & (0x01 << i) )
                                data_out |= (0x80 >> i);
                        }
                    }
                    else
                        // direct copy
                        data_out = passing_data;
                    if (data_mode & 0x01) { // CPHA == 1
                        while (data_mask) {

                            // toggle SCK
                            // set MOSI state
                            // (delays)
                            // toggle SCK
                            // read MISO state
                            // (delays)

                            (*sck_port) ^= sck_mask;

                            if (data_out & data_mask)
                                (*mosi_port) |= mosi_mask;
                            else
                                (*mosi_port) &= inverted_mosi_mask;

                            delayMicroseconds(clock_div);

                            (*sck_port) ^= sck_mask;

                            if ((*miso_port) & miso_mask)
                                data_in |= data_mask;

                            data_mask >>= 1;

                            delayMicroseconds(clock_div);

                        }
                    } else { // CPHA == 0

                        if (data_out & data_mask)
                            (*mosi_port) |= mosi_mask;
                        else
                            (*mosi_port) &= inverted_mosi_mask;

                        delayMicroseconds(clock_div);

                        while (data_mask) {

                            // toggle SCK
                            // read MISO state
                            // (delays)
                            // toggle SCK
                            // set MOSI state
                            // (delays)

                            (*sck_port) ^= sck_mask;

                            if ((*miso_port) & miso_mask)
                                data_in |= data_mask;

                            delayMicroseconds(clock_div);

                            (*sck_port) ^= sck_mask;

                            if (data_out & data_mask)
                                (*mosi_port) |= mosi_mask;
                            else
                                (*mosi_port) &= inverted_mosi_mask;

                            delayMicroseconds(clock_div);

                            data_mask >>= 1;

                        }
                    }
                    uint8_t returning_data = 0;
                    if (is_LSB_first) {
                        // swap the location of bits
                        for (uint8_t i = 0; i < 8; i++) {
                            if ( data_in & (0x01 << i) )
                                returning_data |= (0x80 >> i);
                        }
                    }
                    else
                        // direct copy
                        returning_data = data_in;
                    return returning_data;
                }
                //
                inline void setBitOrder (uint8_t bitOrder) {
                    is_LSB_first = (bitOrder ? 1 : 0);
                }
                //
                inline void setDataMode (uint8_t dataMode) {
                    data_mode = (dataMode & 0x03);
                }
                //
                inline void setClockDivider (uint8_t clockDiv) {
                    clock_div = (clockDiv & 0x07);
                }
        };
    }
}

#endif // #ifndef SOFT_SPI_H
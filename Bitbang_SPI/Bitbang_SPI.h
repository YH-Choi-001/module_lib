#ifndef BITBANG_SPI_H
#define BITBANG_SPI_H

#include <Arduino.h>
#include <SPI.h>

namespace yh {
    namespace rec {
        class Bitbang_SPI {
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
                // data register
                uint8_t data_reg;
                // 1 if LSBFIRST, 0 if MSBFIRST
                unsigned int is_LSB_first : 1;
                // CPOL << 1 | CPHA
                unsigned int data_mode : 2;
                // clock divider
                unsigned int clock_div : 3;
            public:
                // (PORTB, 0x01, PORTB, 0x02, PINB, 0x04)
                Bitbang_SPI
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
                    #define SCK_PORT PORTB
                    #define SCK_BIT 1
                    #define MOSI_PORT PORTB
                    #define MOSI_BIT  2
                    #define MISO_PIN PINB
                    #define MISO_BIT  3
                    // bit-bang SPI MSBFIRST MODE0 in clock-div == 20
                    /* old version
                    // C-code of the assembly:
                    set_mosi_sec:                                                                                      // t = 0
                    if (!(data_reg & 0x80)) // sbrs // 1-cycle if false, 2-cycle if true and skipped inst. is 1-word   1 | 2 !!!
                        PORTB &= ~(1 << 2); // cbi  // 2-cycles                                                        2 | 0 !!!
                    if (data_reg & 0x80)    // sbrc // 1-cycle if false, 2-cycle if true and skipped inst. is 1-word   2 | 1 !!!
                        PORTB |= (1 << 2);  // sbi  // 2-cycles                                                        0 | 2 !!! t = 5
                    PORTB |= (1 << 1);      // sbi  // 2-cycles                                                        2 | 2     t = 7  // clock rise
                    data_reg <<= 1;         // add  // 1-cycle                                                         1 | 1     t = 8
                    asm volatile ("rjmp .+0\n\t"::);// 2-cycles                                                        2 | 2     t = 10
                    if (PINB & (1 << 3))    // sbic // 1-cycle if false, 2-cycle if true and skipped inst. is 1-word   1 | 2 !!!
                        data_reg |= 0x01;   // ori  // 1-cycle                                                         1 | 0 !!! t = 12
                    asm volatile ("rjmp .+0\n\t"::);// 2-cycles                                                        2 | 2     t = 14
                    asm volatile ("nop\n\t"::);     // 1-cycle                                                         1 | 1     t = 15
                    PORTB &= ~(1 << 1);     // cbi  // 2-cycles                                                        2 | 2     t = 17 // clock fall
                    if (--counter != 0)     // subi // 1-cycle                                                         1 | 1     t = 18
                        goto set_mosi_sec;  // brne // 1-cycle if false, 2-cycle if true                               2 | 2     t = 20
                    */

                    // bit-bang SPI MSBFIRST MODE0 in clock-div == 16
                    /* new version
                    // C-code of the assembly:
                    set_mosi_sec:                                                                                      // t = 0
                    PORTB &= ~(1 << 1);     // cbi  // 2-cycles                                                        2 | 2     t = 2  // clock fall
                    if (!(data_reg & 0x80)) // sbrs // 1-cycle if false, 2-cycle if true and skipped inst. is 1-word   1 | 2 !!!
                        PORTB &= ~(1 << 2); // cbi  // 2-cycles                                                        2 | 0 !!!
                    if (data_reg & 0x80)    // sbrc // 1-cycle if false, 2-cycle if true and skipped inst. is 1-word   2 | 1 !!!
                        PORTB |= (1 << 2);  // sbi  // 2-cycles                                                        0 | 2 !!! t = 7
                    asm volatile ("nop\n\t"::);     // 1-cycle                                                         1 | 1     t = 8
                    PORTB |= (1 << 1);      // sbi  // 2-cycles                                                        2 | 2     t = 10  // clock rise
                    data_reg <<= 1;         // add  // 1-cycle                                                         1 | 1     t = 11
                    if (PINB & (1 << 3))    // sbic // 1-cycle if false, 2-cycle if true and skipped inst. is 1-word   1 | 2 !!!
                        data_reg |= 0x01;   // ori  // 1-cycle                                                         1 | 0 !!! t = 13
                    if (--counter != 0)     // subi // 1-cycle                                                         1 | 1     t = 14
                        goto set_mosi_sec;  // brne // 1-cycle if false, 2-cycle if true                               2 | 2     t = 16
                    PORTB &= ~(1 << 1);     // cbi  // 2-cycles                                                        2 | 2     t = nn // clock fall

                    // clock line status:
                    // _ _                 _ _ _ _ _ _
                    //     _ _ _ _ _ _ _ _            
                    // 1 2 3 4 5 6 7 8 9 * 1 2 3 4 5 6

                    */

                    // bit-bang SPI MSBFIRST MODE1 in clock-div == 16
                    /*
                    // C-code of the assembly:
                    set_mosi_sec:                                                                                      // t = 0
                    PORTB |= (1 << 1);      // sbi  // 2-cycles                                                        2 | 2     t = 2  // clock rise
                    if (!(data_reg & 0x80)) // sbrs // 1-cycle if false, 2-cycle if true and skipped inst. is 1-word   1 | 2 !!!
                        PORTB &= ~(1 << 2); // cbi  // 2-cycles                                                        2 | 0 !!!
                    if (data_reg & 0x80)    // sbrc // 1-cycle if false, 2-cycle if true and skipped inst. is 1-word   2 | 1 !!!
                        PORTB |= (1 << 2);  // sbi  // 2-cycles                                                        0 | 2 !!! t = 7
                    asm volatile ("nop\n\t"::);     // 1-cycle                                                         1 | 1     t = 8
                    PORTB &= ~(1 << 1);     // cbi  // 2-cycles                                                        2 | 2     t = 10 // clock fall
                    data_reg <<= 1;         // add  // 1-cycle                                                         1 | 1     t = 11
                    if (PINB & (1 << 3))    // sbic // 1-cycle if false, 2-cycle if true and skipped inst. is 1-word   1 | 2 !!!
                        data_reg |= 0x01;   // ori  // 1-cycle                                                         1 | 0 !!! t = 13
                    if (--counter != 0)     // subi // 1-cycle                                                         1 | 1     t = 14
                        goto set_mosi_sec;  // brne // 1-cycle if false, 2-cycle if true                               2 | 2     t = 16

                    // clock line status:
                    //     _ _ _ _ _ _ _ _
                    // _ _                 _ _ _ _ _ _
                    // 1 2 3 4 5 6 7 8 9 * 1 2 3 4 5 6

                    */

                    uint8_t counter = 8; // counter that indicates how many bits to be tranferred
                    if (is_LSB_first)
                        data_reg =
                            ((data_reg & 0x80) >> 7)
                            | ((data_reg & 0x40) >> 5)
                            | ((data_reg & 0x20) >> 3)
                            | ((data_reg & 0x10) >> 1)
                            | ((data_reg & 0x08) << 1)
                            | ((data_reg & 0x04) << 3)
                            | ((data_reg & 0x02) << 5)
                            | ((data_reg & 0x01) << 7)
                            ;
                    uint8_t oldSREG = SREG;
                    noInterrupts();
                    if (data_mode & 0x01) {
                        // CPHA == 1
                        // max freq == F_CPU / 16
                        if (data_mode & 0x02) {
                            // CPOL == 1
                            asm volatile (
                                "sbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT |= SCK_BIT;
                                "new_bit_%=:"                  "\n\t" // NEW_BIT:
                                "cbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT &= ~SCK_BIT;
                                "sbrs %[data], 7"              "\n\t" // if (!(data_reg & 0x80))
                                "cbi %[mosi_out], %[mosi_bit]" "\n\t" // MOSI_PORT &= ~MOSI_BIT;
                                "sbrc %[data], 7"              "\n\t" // if (data_reg & 0x80)
                                "sbi %[mosi_out], %[mosi_bit]" "\n\t" // MOSI_PORT |= MOSI_BIT;
                                "nop"                          "\n\t" // nop
                                "sbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT |= SCK_BIT;
                                "add %[data], %[data]"         "\n\t" // data_reg <<= 1;
                                "sbic %[miso_in], %[miso_bit]" "\n\t" // if (MISO_PIN & MISO_BIT)
                                "ori %[data], 0x01"            "\n\t" // data_reg |= 1;
                                "subi %[counter], 0x01"        "\n\t" // counter -= 1;
                                "brne new_bit_%="              "\n\t" // if (counter != 0) goto NEW_BIT;
                            :
                                [data] "+r" (data_reg),
                                [counter] "+r" (counter)
                            :
                                [mosi_out] "I" (_SFR_IO_ADDR(MOSI_PORT)),
                                [mosi_bit] "I" (MOSI_BIT),
                                [sck_out] "I" (_SFR_IO_ADDR(SCK_PORT)),
                                [sck_bit] "I" (SCK_BIT),
                                [miso_in] "I" (_SFR_IO_ADDR(MISO_PIN)),
                                [miso_bit] "I" (MISO_BIT)
                            );
                        } else {
                            // CPOL == 0
                            asm volatile (
                                "cbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT &= ~SCK_BIT;
                                "new_bit_%=:"                  "\n\t" // NEW_BIT:
                                "sbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT |= SCK_BIT;
                                "sbrs %[data], 7"              "\n\t" // if (!(data_reg & 0x80))
                                "cbi %[mosi_out], %[mosi_bit]" "\n\t" // MOSI_PORT &= ~MOSI_BIT;
                                "sbrc %[data], 7"              "\n\t" // if (data_reg & 0x80)
                                "sbi %[mosi_out], %[mosi_bit]" "\n\t" // MOSI_PORT |= MOSI_BIT;
                                "nop"                          "\n\t" // nop
                                "cbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT &= ~SCK_BIT;
                                "add %[data], %[data]"         "\n\t" // data_reg <<= 1;
                                "sbic %[miso_in], %[miso_bit]" "\n\t" // if (MISO_PIN & MISO_BIT)
                                "ori %[data], 0x01"            "\n\t" // data_reg |= 1;
                                "subi %[counter], 0x01"        "\n\t" // counter -= 1;
                                "brne new_bit_%="              "\n\t" // if (counter != 0) goto NEW_BIT;
                            :
                                [data] "+r" (data_reg),
                                [counter] "+r" (counter)
                            :
                                [mosi_out] "I" (_SFR_IO_ADDR(MOSI_PORT)),
                                [mosi_bit] "I" (MOSI_BIT),
                                [sck_out] "I" (_SFR_IO_ADDR(SCK_PORT)),
                                [sck_bit] "I" (SCK_BIT),
                                [miso_in] "I" (_SFR_IO_ADDR(MISO_PIN)),
                                [miso_bit] "I" (MISO_BIT)
                            );
                        }
                    } else {
                        // CPHA == 0
                        // max freq == F_CPU / 16
                        if (data_mode & 0x02) {
                            // CPOL == 1
                            asm volatile (
                                "sbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT |= SCK_BIT;
                                "new_bit_%=:"                  "\n\t" // NEW_BIT:
                                "sbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT |= SCK_BIT;
                                "sbrs %[data], 7"              "\n\t" // if (!(data_reg & 0x80))
                                "cbi %[mosi_out], %[mosi_bit]" "\n\t" // MOSI_PORT &= ~MOSI_BIT;
                                "sbrc %[data], 7"              "\n\t" // if (data_reg & 0x80)
                                "sbi %[mosi_out], %[mosi_bit]" "\n\t" // MOSI_PORT |= MOSI_BIT;
                                "nop"                          "\n\t" // nop
                                "cbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT &= ~SCK_BIT;
                                "add %[data], %[data]"         "\n\t" // data_reg <<= 1;
                                "sbic %[miso_in], %[miso_bit]" "\n\t" // if (MISO_PIN & MISO_BIT)
                                "ori %[data], 0x01"            "\n\t" // data_reg |= 1;
                                "subi %[counter], 0x01"        "\n\t" // counter -= 1;
                                "brne new_bit_%="              "\n\t" // if (counter != 0) goto NEW_BIT;
                                "sbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT |= SCK_BIT;
                            :
                                [data] "+r" (data_reg),
                                [counter] "+r" (counter)
                            :
                                [mosi_out] "I" (_SFR_IO_ADDR(MOSI_PORT)),
                                [mosi_bit] "I" (MOSI_BIT),
                                [sck_out] "I" (_SFR_IO_ADDR(SCK_PORT)),
                                [sck_bit] "I" (SCK_BIT),
                                [miso_in] "I" (_SFR_IO_ADDR(MISO_PIN)),
                                [miso_bit] "I" (MISO_BIT)
                            );
                        } else {
                            // CPOL == 0
                            asm volatile (
                                "cbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT &= ~SCK_BIT;
                                "new_bit_%=:"                  "\n\t" // NEW_BIT:
                                "cbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT &= ~SCK_BIT;
                                "sbrs %[data], 7"              "\n\t" // if (!(data_reg & 0x80))
                                "cbi %[mosi_out], %[mosi_bit]" "\n\t" // MOSI_PORT &= ~MOSI_BIT;
                                "sbrc %[data], 7"              "\n\t" // if (data_reg & 0x80)
                                "sbi %[mosi_out], %[mosi_bit]" "\n\t" // MOSI_PORT |= MOSI_BIT;
                                "nop"                          "\n\t" // nop
                                "sbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT |= SCK_BIT;
                                "add %[data], %[data]"         "\n\t" // data_reg <<= 1;
                                "sbic %[miso_in], %[miso_bit]" "\n\t" // if (MISO_PIN & MISO_BIT)
                                "ori %[data], 0x01"            "\n\t" // data_reg |= 1;
                                "subi %[counter], 0x01"        "\n\t" // counter -= 1;
                                "brne new_bit_%="              "\n\t" // if (counter != 0) goto NEW_BIT;
                                "cbi %[sck_out], %[sck_bit]"   "\n\t" // SCK_PORT &= ~SCK_BIT;
                            :
                                [data] "+r" (data_reg),
                                [counter] "+r" (counter)
                            :
                                [mosi_out] "I" (_SFR_IO_ADDR(MOSI_PORT)),
                                [mosi_bit] "I" (MOSI_BIT),
                                [sck_out] "I" (_SFR_IO_ADDR(SCK_PORT)),
                                [sck_bit] "I" (SCK_BIT),
                                [miso_in] "I" (_SFR_IO_ADDR(MISO_PIN)),
                                [miso_bit] "I" (MISO_BIT)
                            );
                        }
                    }
                    SREG = oldSREG;
                    if (is_LSB_first)
                        data_reg =
                            ((data_reg & 0x80) >> 7)
                            | ((data_reg & 0x40) >> 5)
                            | ((data_reg & 0x20) >> 3)
                            | ((data_reg & 0x10) >> 1)
                            | ((data_reg & 0x08) << 1)
                            | ((data_reg & 0x04) << 3)
                            | ((data_reg & 0x02) << 5)
                            | ((data_reg & 0x01) << 7)
                            ;
                    return data_reg;
                }
                //
                inline void setBitOrder (uint8_t bitOrder) {
                    is_LSB_first = (bitOrder == LSBFIRST);
                }
                //
                inline void setDataMode (uint8_t dataMode) {
                    switch (dataMode) {
                        case SPI_MODE0:
                            data_mode = 0;
                            break;
                        case SPI_MODE1:
                            data_mode = 1;
                            break;
                        case SPI_MODE2:
                            data_mode = 2;
                            break;
                        case SPI_MODE3:
                            data_mode = 3;
                            break;
                        default:
                            data_mode = 0;
                            break;
                    }
                }
                //
                inline void setClockDivider (uint8_t clockDiv) {
                    // clock_div = (clockDiv & 0x07);
                }
        };
    }
}

#endif // #ifndef BITBANG_SPI_H
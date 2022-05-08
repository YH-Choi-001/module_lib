#ifndef MCP_3008_H
#define MCP_3008_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <SPI.h>

namespace yh {
    namespace rec {
        //                  +-\/-+
        //            CH0  1|    |16  VDD
        //            CH1  2|    |15  VREF
        //            CH2  3|    |14  AGND
        //            CH3  4|    |13  CLK
        //            CH4  5|    |12  DOUT
        //            CH5  6|    |11  DIN
        //            CH6  7|    |10  CS/SHDN
        //            CH7  8|    |9   DGND
        //                  +----+
        template <class SPI_class, class SPI_settings>
        class Mcp_3008 {
            private:
                //
            protected:
                // pins that cannot be changed:
                const uint8_t
                    sck_pin,
                    mosi_pin,
                    miso_pin,
                    cs_pin;
                volatile uint8_t *cs_pin_out_reg;
                uint8_t cs_pin_mask;
                // SPI object
                SPI_class *spi_ptr;
                // SPI settings
                SPI_settings spi_settings;
            public:
                // passes the CS pin into the constructor
                Mcp_3008 (const uint8_t init_cs_pin)
                :
                    sck_pin(SCK),
                    mosi_pin(MOSI),
                    miso_pin(MISO),
                    cs_pin(init_cs_pin),
                    spi_ptr(&SPI),
                    spi_settings(3600000UL, MSBFIRST, SPI_MODE0)
                {
                    //
                }
                // passes the CS pin into the constructor
                Mcp_3008 (SPI_class *init_spi_ptr, const uint8_t init_sck_pin, const uint8_t init_mosi_pin, const uint8_t init_miso_pin, const uint8_t init_cs_pin)
                :
                    sck_pin(init_sck_pin),
                    mosi_pin(init_mosi_pin),
                    miso_pin(init_miso_pin),
                    cs_pin(init_cs_pin),
                    spi_ptr(&init_spi_ptr),
                    spi_settings(3600000UL, MSBFIRST, SPI_MODE0)
                {
                    //
                }
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin () {
                    pinMode(cs_pin, OUTPUT); // prioritized line
                    pinMode(sck_pin, OUTPUT);
                    pinMode(mosi_pin, OUTPUT);
                    pinMode(miso_pin, INPUT);
                    cs_pin_out_reg = portOutputRegister(digitalPinToPort(cs_pin));
                    cs_pin_mask = digitalPinToBitMask(cs_pin);
                    (*cs_pin_out_reg) |= cs_pin_mask;
                    spi_ptr->begin();
                }
                // reads the 10-bit analog to digital voltage of the channel inputed
                uint16_t analog_read (const uint8_t channel) __attribute__ ((__always_inline__)) {
                    (*cs_pin_out_reg) &= (~cs_pin_mask);
                    spi_ptr->beginTransaction(spi_settings);
                    spi_ptr->transfer(0x01); // transfer START bit
                    const uint16_t result = (static_cast<uint16_t>(spi_ptr->transfer(0x80 | (channel << 4)) & 0x03) << 8) | spi_ptr->transfer(0x00); // 0x80 selects single channel
                    spi_ptr->endTransaction();
                    (*cs_pin_out_reg) |= cs_pin_mask;
                    return result;
                }
                // reads the 10-bit analog to digital voltage of the channel inputed in differential mode
                uint16_t analog_read_differential (const uint8_t channel) __attribute__ ((__always_inline__)) {
                    (*cs_pin_out_reg) &= (~cs_pin_mask);
                    spi_ptr->beginTransaction(spi_settings);
                    spi_ptr->transfer(0x01); // transfer START bit
                    const uint16_t result = (static_cast<uint16_t>(spi_ptr->transfer(channel << 4) & 0x03) << 8) | spi_ptr->transfer(0x00);
                    spi_ptr->endTransaction();
                    (*cs_pin_out_reg) |= cs_pin_mask;
                    return result;
                }
        };
    }
}

#endif // #ifndef MCP_3008_H
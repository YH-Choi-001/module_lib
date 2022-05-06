#ifndef MCP_3008_H
#define MCP_3008_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <SPI.h>

namespace yh {
    namespace rec {
        class Mcp_3008 {
            private:
                //
            protected:
                // pins that cannot be changed:
                const uint8_t cs_pin;
                volatile uint8_t *cs_pin_out_reg;
                uint8_t cs_pin_mask;
                // SPI settings
                SPISettings spi_settings;
            public:
                // passes the CS pin into the constructor
                Mcp_3008 (const uint8_t init_cs_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // reads the 10-bit analog to digital voltage of the channel inputed
                uint16_t analog_read (const uint8_t channel) __attribute__ ((__always_inline__)) {
                    (*cs_pin_out_reg) &= (~cs_pin_mask);
                    SPI.beginTransaction(spi_settings);
                    SPI.transfer(0x01); // transfer START bit
                    const uint16_t result = (static_cast<uint16_t>(SPI.transfer(0x80 | (channel << 4)) & 0x03) << 8) | SPI.transfer(0x00); // 0x80 selects single channel
                    SPI.endTransaction();
                    (*cs_pin_out_reg) |= cs_pin_mask;
                    return result;
                }
                // reads the 10-bit analog to digital voltage of the channel inputed in differential mode
                uint16_t analog_read_differential (const uint8_t channel) __attribute__ ((__always_inline__)) {
                    (*cs_pin_out_reg) &= (~cs_pin_mask);
                    SPI.beginTransaction(spi_settings);
                    SPI.transfer(0x01); // transfer START bit
                    const uint16_t result = (static_cast<uint16_t>(SPI.transfer(channel << 4) & 0x03) << 8) | SPI.transfer(0x00);
                    SPI.endTransaction();
                    (*cs_pin_out_reg) |= cs_pin_mask;
                    return result;
                }
        };
    }
}

#endif // #ifndef MCP_3008_H
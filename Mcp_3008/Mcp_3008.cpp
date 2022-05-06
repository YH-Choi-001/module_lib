#ifndef MCP_3008_CPP
#define MCP_3008_CPP

#include "Mcp_3008.h"

yh::rec::Mcp_3008::Mcp_3008 (const uint8_t init_cs_pin) :
    cs_pin(init_cs_pin),
    spi_settings(3600000UL, MSBFIRST, SPI_MODE0)
{
    //
}

void yh::rec::Mcp_3008::begin () {
    pinMode(cs_pin, OUTPUT); // prioritized line
    pinMode(SCK, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, INPUT);
    cs_pin_out_reg = portOutputRegister(digitalPinToPort(cs_pin));
    cs_pin_mask = digitalPinToBitMask(cs_pin);
    (*cs_pin_out_reg) |= cs_pin_mask;
    SPI.begin();
}

uint16_t yh::rec::Mcp_3008::analog_read (const uint8_t channel) {
    (*cs_pin_out_reg) &= (~cs_pin_mask);
    SPI.beginTransaction(spi_settings);
    SPI.transfer(0x01); // transfer START bit
    const uint16_t result = ((SPI.transfer(0x80 | (channel << 4)) & 0x03) << 8) | SPI.transfer(0x00); // 0x80 selects single channel
    SPI.endTransaction();
    (*cs_pin_out_reg) |= cs_pin_mask;
    return result;
}

#endif // #ifndef MCP_3008_CPP
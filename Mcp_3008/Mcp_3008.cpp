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

#endif // #ifndef MCP_3008_CPP
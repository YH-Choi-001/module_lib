#ifndef Z4HC595_CPP
#define Z4HC595_CPP __DATE__ ", " __TIME__

#include "74hc595.h"

const SPISettings yh::rec::Z4hc595::default_spi_settings(SPISettings(8000000UL, MSBFIRST, SPI_MODE0));

yh::rec::Z4hc595::Z4hc595 (const uint8_t init_ss_pin) :
    shcp_pin(SCK),
    ds_pin(MOSI),
    stcp_pin(init_ss_pin),
    use_hardware_spi(1)
{
    //
}

yh::rec::Z4hc595::Z4hc595 (const uint8_t init_shcp_pin, const uint8_t init_ds_pin, const uint8_t init_stcp_pin) :
    shcp_pin(init_shcp_pin),
    ds_pin(init_ds_pin),
    stcp_pin(init_stcp_pin),
    use_hardware_spi(0)
{
    //
}

void yh::rec::Z4hc595::begin () {
    pinMode(shcp_pin, OUTPUT);
    pinMode(ds_pin, OUTPUT);
    if (stcp_pin != NC_PINNO) {
        pinMode(stcp_pin, OUTPUT);
        digitalWrite(stcp_pin, HIGH);
    }
    if (use_hardware_spi)
        SPI.begin();
}

void yh::rec::Z4hc595::left_shift (const uint8_t q0_new_val) {
    if (stcp_pin != NC_PINNO)
        digitalWrite(stcp_pin, LOW);
    digitalWrite(ds_pin, (q0_new_val ? HIGH : LOW));
    digitalWrite(shcp_pin, HIGH);
    digitalWrite(shcp_pin, LOW);
    if (stcp_pin != NC_PINNO)
        digitalWrite(stcp_pin, HIGH);
}

void yh::rec::Z4hc595::write (const uint8_t q7_to_q0_new_vals) {
    if (stcp_pin != NC_PINNO)
        digitalWrite(stcp_pin, LOW);
    if (use_hardware_spi) {
        SPI.beginTransaction(default_spi_settings);
        SPI.transfer(q7_to_q0_new_vals);
        SPI.endTransaction();
    } else {
        shiftOut(ds_pin, shcp_pin, MSBFIRST, q7_to_q0_new_vals);
    }
    if (stcp_pin != NC_PINNO)
        digitalWrite(stcp_pin, HIGH);
}

#endif // #ifndef Z4HC595_CPP
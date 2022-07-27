#ifndef JOINMAX_COMPOI_CPP
#define JOINMAX_COMPOI_CPP __DATE__ ", " __TIME__

#include "Joinmax_compoI.h"

uint8_t yh::rec::CompoI::command (const uint8_t cmd) {
    Wire.beginTransmission(i2c_address);
    Wire.write(cmd);
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(1U));
    while (!Wire.available()) {}
    return Wire.read();
}

yh::rec::CompoI::CompoI (const uint8_t init_i2c_address) :
    i2c_address(init_i2c_address)
{
    //
}

void yh::rec::CompoI::begin () {
    // init settings for I2C
    if (!(TWCR & (1 << TWEN))) { // if (TwoWireENable bit is off) { begin I2C communication }
        Wire.begin();
        Wire.setClock(400000); // set I2C to fast mode for faster communication
    }
}

uint8_t yh::rec::CompoI::get_channel_val (const uint8_t channel) {
    return command(((channel < 5) ? (5) : (12)) - channel);
}

uint8_t yh::rec::CompoI::get_max_idx () {
    return 8 - command(8);
}

uint8_t yh::rec::CompoI::get_max_val () {
    return command(9);
}

uint8_t yh::rec::CompoI::get_min_idx () {
    return 8 - command(10);
}

uint8_t yh::rec::CompoI::get_min_val () {
    return command(11);
}

uint8_t yh::rec::CompoI::get_avg_val () {
    return command(12);
}

uint8_t yh::rec::CompoI::set_filter_on () {
    return command(14);
}

uint8_t yh::rec::CompoI::set_filter_off () {
    return command(13);
}

uint8_t yh::rec::CompoI::calibrate () {
    return command(15);
}

#endif // #ifndef JOINMAX_COMPOI_CPP
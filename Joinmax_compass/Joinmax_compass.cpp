#ifndef JOINMAX_COMPASS_CPP
#define JOINMAX_COMPASS_CPP __DATE__ ", " __TIME__

#include "Joinmax_compass.h"

yh::rec::Compass::Compass (const uint8_t init_i2c_address) :
    i2c_address(init_i2c_address)
{
    //
}

void yh::rec::Compass::begin () {
    // init settings for I2C
    if (!(TWCR & (1 << TWEN))) { // if (TwoWireENable bit is off) { begin I2C communication }
        Wire.begin();
        Wire.setClock(400000); // set I2C to fast mode for faster communication
    }
}

uint16_t yh::rec::Compass::get_heading () {
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(2U));
    uint8_t high = Wire.available() ? Wire.read() : 0;
    uint8_t low = Wire.available() ? Wire.read() : 0;
    return high << 8 | low;
}

#endif // #ifndef JOINMAX_COMPASS_CPP
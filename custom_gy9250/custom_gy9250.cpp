#ifndef CUSTOM_GY9250_CPP
#define CUSTOM_GY9250_CPP __DATE__ ", " __TIME__

#include "custom_gy9250.h"
#include "../custom_gy521/custom_gy521.cpp"

Custom_ak8963::Custom_ak8963 (const uint8_t init_i2c_address) :
    i2c_address(init_i2c_address), ASA_X(0), ASA_Y(0), ASA_Z(0), max_x(0), max_y(0), max_z(0), min_x(0), min_y(0), min_z(0),
    range_x(0), range_y(0), range_z(0), raw_x(0), raw_y(0), raw_z(0), rz_heading(0)
{
    //
}

void Custom_ak8963::begin () {
    // init settings to the AK8963 module through I2C
    if (!(TWCR & _BV(TWEN))) { // if (TwoWireENable bit is off) { begin I2C communication }
        Wire.begin();
        Wire.setClock(400000); // set I2C to fast mode for faster communication
    }
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x0A); // accessing the register 0x0A - CNTL1 register to turn off power-down mode, and to select fuse ROM access mode
    Wire.write(0x0F); // select fuse ROM access mode
    Wire.endTransmission();
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x10); // accessing the register 0x10 to 0x12 - ASAX, ASAY, ASAZ registers for the magnetometer value adjustment in fuse ROM
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, 3);
    while (Wire.available() < 3) {}
    ASA_X = Wire.read();
    ASA_Y = Wire.read();
    ASA_Z = Wire.read();
    // set power mode to power-down mode first before other modes, required by datasheet to change power modes
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x0A); // accessing the register 0x0A - CNTL1 register to select power-down mode
    Wire.write(0x00); // select power-down mode
    Wire.endTransmission();
    delayMicroseconds(128); // delay at least 100us, required by datasheet to change power modes
    // // select continuous measurement 2 that samples the magnetic field strength in 100 KHz (this is already the limit of the speed of the chip)
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x0A); // accessing the register 0x0A - CNTL1 register to turn off power-down mode, and to select continuous measurement 2 mode
    Wire.write(0x06); // select continuous measurement 2 mode
    Wire.endTransmission();
}

inline void Custom_ak8963::update_raw () {
    // // refresh magnetometer
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x02); // accessing the register 0x02 - ST1 register
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(1U)); // see if new data is ready
    while (Wire.available() < 1) {}
    if (Wire.read() & 0x01) {
        // new data is ready
        Wire.beginTransmission(i2c_address); // talk to AK8963
        Wire.write(0x03); // accessing the registers of magnetometer x, y, z, where each axis has 2 bytes, from 0x03 to 0x08
        Wire.endTransmission();
        Wire.requestFrom(i2c_address, static_cast<uint8_t>(7U));
        while (Wire.available() < 7) {}
        const int16_t temp_raw_x = (Wire.read() | Wire.read() << 8) * ( ((ASA_X - 128) * 0.5) / 128.0 + 1 );
        const int16_t temp_raw_y = (Wire.read() | Wire.read() << 8) * ( ((ASA_Y - 128) * 0.5) / 128.0 + 1 );
        const int16_t temp_raw_z = (Wire.read() | Wire.read() << 8) * ( ((ASA_Z - 128) * 0.5) / 128.0 + 1 );
        if (!(Wire.read() & 0x10)) { // reading the ST2 register to check if magnetometer overflow has occured
            // no overflow has occured
            // update raw_x, raw_y, raw_z
            raw_x = temp_raw_x;
            raw_y = temp_raw_y;
            raw_z = temp_raw_z;
        }
    }
}

uint8_t Custom_ak8963::who_i_am () {
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x00); // accessing the register 0x00 - WIA register
    const uint8_t err = Wire.endTransmission();
    if (err) return 0; // an error occured while communicating with the chip
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(1U));
    while (Wire.available() < 1) {}
    return Wire.read();
}

void Custom_ak8963::single_calibrate () {
    update_raw();
    // update max values
    if (raw_x > max_x) max_x = raw_x;
    if (raw_y > max_y) max_y = raw_y;
    if (raw_z > max_z) max_z = raw_z;
    // update min values
    if (raw_x < min_x) min_x = raw_x;
    if (raw_y < min_y) min_y = raw_y;
    if (raw_z < min_z) min_z = raw_z;
    // x_mean = (max_x + min_x) / 2;
    // y_mean = (max_y + min_y) / 2;
    // z_mean = (max_z + min_z) / 2;
    range_x = max_x - min_x;
    range_y = max_y - min_y;
    range_z = max_z - min_z;
}

void Custom_ak8963::reset_heading () {
    rz_heading = get_heading();
}

// double map_private (long x, long in_min, long in_max, long out_min, long out_max)
// {
//   return (x - in_min) * (out_max - out_min) / static_cast<double>(in_max - in_min) + out_min;
// }

double Custom_ak8963::get_heading () {
    update_raw();
    // const double
    //     cal_x = raw_x - (min_x + max_x) / 2,
    //     cal_y = raw_y - (min_y + max_y) / 2,
    //     cal_z = raw_z - (min_z + max_z) / 2;
    // const double
    //     cal_x = map_private(raw_x, min_x, max_x, -1023, 1023),
    //     cal_y = map_private(raw_y, min_y, max_y, -1023, 1023),
    //     cal_z = map_private(raw_z, min_z, max_z, -1023, 1023);
    const double
        cal_x = (raw_x - min_x) * 2046 / static_cast<double>(range_x) - 1023,
        cal_y = (raw_y - min_y) * 2046 / static_cast<double>(range_y) - 1023,
        cal_z = (raw_z - min_z) * 2046 / static_cast<double>(range_z) - 1023;
    const double dir = atan2(cal_x, cal_y) / M_PI * 180 - rz_heading;
    return (dir < 0.0) ? (dir + 360.0) : ((dir > 360.0) ? (dir - 360.0) : dir);
}

double Custom_ak8963::get_heading (double (*atan2_function)(double, double)) {
    update_raw();
    // const double
    //     cal_x = raw_x - (min_x + max_x) / 2,
    //     cal_y = raw_y - (min_y + max_y) / 2,
    //     cal_z = raw_z - (min_z + max_z) / 2;
    // const double
    //     cal_x = map_private(raw_x, min_x, max_x, -1023, 1023),
    //     cal_y = map_private(raw_y, min_y, max_y, -1023, 1023),
    //     cal_z = map_private(raw_z, min_z, max_z, -1023, 1023);
    const double
        cal_x = (raw_x - min_x) * 2046 / static_cast<double>(range_x) - 1023,
        cal_y = (raw_y - min_y) * 2046 / static_cast<double>(range_y) - 1023,
        cal_z = (raw_z - min_z) * 2046 / static_cast<double>(range_z) - 1023;
    const double dir = ((atan2_function == NULL) ? (atan2(cal_x, cal_y) / M_PI * 180) : (atan2_function(cal_x, cal_y) / M_PI * 180)) - rz_heading;
    return (dir < 0.0) ? (dir + 360.0) : ((dir > 360.0) ? (dir - 360.0) : dir);
}

Custom_gy9250::Custom_gy9250 (const uint8_t init_i2c_address) :
    Custom_gy521(init_i2c_address), mag(0x0C)
{
    //
}

#endif // #ifndef CUSTOM_GY9250_CPP
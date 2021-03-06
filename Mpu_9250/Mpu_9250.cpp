#ifndef MPU_9250_CPP
#define MPU_9250_CPP __DATE__ ", " __TIME__

#include "Mpu_9250.h"
#include "../Mpu_6050/Mpu_6050.cpp"

yh::rec::Ak_8963::Ak_8963 (const uint8_t init_i2c_address) :
    i2c_address(init_i2c_address), ASA_X(0), ASA_Y(0), ASA_Z(0), max_x(0), max_y(0), max_z(0), min_x(0), min_y(0), min_z(0),
    range_x(0), range_y(0), range_z(0), raw_x(0), raw_y(0), raw_z(0), rz_heading(0)
{
    //
}

void yh::rec::Ak_8963::begin () {
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
    if (Wire.requestFrom(i2c_address, static_cast<uint8_t>(3U)) == static_cast<uint8_t>(3U)) {
        ASA_X = Wire.read();
        ASA_Y = Wire.read();
        ASA_Z = Wire.read();
    }
    // set power mode to power-down mode first before other modes, required by datasheet to change power modes
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x0A); // accessing the register 0x0A - CNTL1 register to select power-down mode
    Wire.write(0x00); // select power-down mode
    Wire.endTransmission();
    delayMicroseconds(100); // delay at least 100us, required by datasheet when changing power modes
    // // select continuous measurement 2 that samples the magnetic field strength in 100 KHz (this is already the limit of the speed of the chip)
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x0A); // accessing the register 0x0A - CNTL1 register to turn off power-down mode, and to select continuous measurement 2 mode
    Wire.write(0x06); // select continuous measurement 2 mode
    Wire.endTransmission();
}

inline void yh::rec::Ak_8963::update_raw () {
    // // refresh magnetometer
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x02); // accessing the register 0x02 - ST1 register
    Wire.endTransmission();
    if (Wire.requestFrom(i2c_address, static_cast<uint8_t>(1U)) == 1) {
        if (Wire.read() & 0x01) {
            // new data is ready
            Wire.beginTransmission(i2c_address); // talk to AK8963
            Wire.write(0x03); // accessing the registers of magnetometer x, y, z, where each axis has 2 bytes, from 0x03 to 0x08
            Wire.endTransmission();
            if (Wire.requestFrom(i2c_address, static_cast<uint8_t>(7U)) == static_cast<uint8_t>(7U)) {
                // const int16_t temp_raw_x = (Wire.read() | Wire.read() << 8) * ( ((ASA_X - 128) * 0.5) / 128.0 + 1 );
                // const int16_t temp_raw_y = (Wire.read() | Wire.read() << 8) * ( ((ASA_Y - 128) * 0.5) / 128.0 + 1 );
                // const int16_t temp_raw_z = (Wire.read() | Wire.read() << 8) * ( ((ASA_Z - 128) * 0.5) / 128.0 + 1 );
                const int16_t temp_raw_x = (Wire.read() | Wire.read() << 8) * (ASA_X + 128) / 256;
                const int16_t temp_raw_y = (Wire.read() | Wire.read() << 8) * (ASA_Y + 128) / 256;
                const int16_t temp_raw_z = (Wire.read() | Wire.read() << 8) * (ASA_Z + 128) / 256;
                if (!(Wire.read() & 0x10)) { // reading the ST2 register to check if magnetometer overflow has occured
                    // no overflow has occured
                    // update raw_x, raw_y, raw_z
                    raw_x = temp_raw_x;
                    raw_y = temp_raw_y;
                    raw_z = temp_raw_z;
                }
            }
        }
    }
}

uint8_t yh::rec::Ak_8963::who_i_am () {
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x00); // accessing the register 0x00 - WIA register
    const uint8_t err = Wire.endTransmission();
    if (err) return 0; // an error occured while communicating with the chip
    if (Wire.requestFrom(i2c_address, static_cast<uint8_t>(1U)) == static_cast<uint8_t>(1U))
        return Wire.read();
    return 0;
}

void yh::rec::Ak_8963::soft_reset () {
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x0B); // accessing the register 0x0B - CNTL2 register
    Wire.write(1 << 0); // writes 1 to SRST bit to reset the chip in software
    Wire.endTransmission();
}

void yh::rec::Ak_8963::single_calibrate () {
    update_raw();
    // update max values
    if (raw_x > max_x) max_x = raw_x;
    if (raw_y > max_y) max_y = raw_y;
    if (raw_z > max_z) max_z = raw_z;
    // update min values
    if (raw_x < min_x) min_x = raw_x;
    if (raw_y < min_y) min_y = raw_y;
    if (raw_z < min_z) min_z = raw_z;
    // update range of values of each axis
    range_x = max_x - min_x;
    range_y = max_y - min_y;
    range_z = max_z - min_z;
}

void yh::rec::Ak_8963::reset_heading () {
    update_raw();
    const double
        cal_x = (raw_x - min_x) * 2 / range_x - 1,
        cal_y = (raw_y - min_y) * 2 / range_y - 1;
        // cal_z = (raw_z - min_z) * 2 / range_z - 1;
    rz_heading = atan2(cal_x, cal_y) * RAD_TO_DEG;
}

double yh::rec::Ak_8963::get_heading () {
    update_raw();
    const double
        cal_x = (raw_x - min_x) * 2 / range_x - 1,
        cal_y = (raw_y - min_y) * 2 / range_y - 1;
        // cal_z = (raw_z - min_z) * 2 / range_z - 1;
    double dir = atan2(cal_x, cal_y) * RAD_TO_DEG - rz_heading;
    if (dir < 0.0) dir += 360.0;
    return dir;
}

yh::rec::Mpu_9250::Mpu_9250 (const uint8_t init_i2c_address) :
    Mpu_6050(init_i2c_address), mag(0x0C)
{
    //
}

#endif // #ifndef MPU_9250_CPP
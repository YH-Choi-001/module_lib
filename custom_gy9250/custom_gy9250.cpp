#ifndef CUSTOM_GY9250_CPP
#define CUSTOM_GY9250_CPP __DATE__ ", " __TIME__

#include "custom_gy9250.h"
#include "../custom_gy521/custom_gy521.cpp"

// uint16_t read_i2c_data_2_bytes (const uint8_t i2c_address, const uint8_t register_no) {
//     Wire.beginTransmission(i2c_address);
//     Wire.write(register_no);
//     Wire.endTransmission();
//     Wire.requestFrom(i2c_address, static_cast<uint8_t>(2U));
//     while (Wire.available() < 2) {}
//     return (Wire.read() << 8) | Wire.read();
// }

Custom_ak8963::Custom_ak8963 (const uint8_t init_i2c_address) :
    i2c_address(init_i2c_address), ASA_X(0), ASA_Y(0), ASA_Z(0), raw_x(0), raw_y(0), raw_z(0), adj_x(0), adj_y(0), adj_z(0)
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

void Custom_ak8963::update_raw () {
    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x0A); // accessing the register 0x0A - CNTL1 register to turn off power-down mode, and to select single-measurement mode
    // Wire.write(0x01); // select single-measurement mode
    Wire.write(0b0010); // select continuous measurement mode 1
    Wire.endTransmission();

    Wire.beginTransmission(i2c_address); // talk to AK8963
    Wire.write(0x02); // accessing the register 0x02 - ST1 register
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(1U));
    while (Wire.available() < 1) {}
    if (Wire.read() & 0x01) {
        Wire.beginTransmission(i2c_address); // talk to AK8963
        Wire.write(0x03); // accessing the registers of magnetometer x, y, z, where each axis has 2 bytes, from 0x03 to 0x08
        Wire.endTransmission();
        Wire.requestFrom(i2c_address, static_cast<uint8_t>(7U));
        while (Wire.available() < 7) {}
        raw_x = Wire.read() | Wire.read() << 8;
        raw_y = Wire.read() | Wire.read() << 8;
        raw_z = Wire.read() | Wire.read() << 8;
        Wire.read();
    }
}

void Custom_ak8963::update_adjusted () {
    update_raw();
    adj_x = raw_x * ( ((ASA_X - 128) * 0.5) / 128.0 + 1 );
    adj_y = raw_y * ( ((ASA_Y - 128) * 0.5) / 128.0 + 1 );
    adj_z = raw_z * ( ((ASA_Z - 128) * 0.5) / 128.0 + 1 );
}

Custom_gy9250::Custom_gy9250 (const uint8_t init_i2c_address) :
    Custom_gy521(init_i2c_address), mag(0x0C)
{
    //
}

#endif // #ifndef CUSTOM_GY9250_CPP
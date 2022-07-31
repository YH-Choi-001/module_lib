#ifndef QMC5883L_CPP
#define QMC5883L_CPP __DATE__ ", " __TIME__

#include "Qmc5883l.h"

// 12000 LSB/G in range == +-2G
// 3000 LSB/G in range == +-8G

// configurations regarding register 0x09 - Control Register 1

#define OSR_512 (0b00 << 6)
#define OSR_256 (0b01 << 6)
#define OSR_128 (0b10 << 6)
#define OSR_64  (0b11 << 6)

#define RNG_2G (0b00 << 4)
#define RNG_8G (0b01 << 4)

#define ODR_10Hz  (0b00 << 2)
#define ODR_50Hz  (0b01 << 2)
#define ODR_100Hz (0b10 << 2)
#define ODR_200Hz (0b11 << 2)

#define MODE_STANDBY    (0b00 << 0)
#define MODE_CONTINUOUS (0b01 << 0)

#define OSR OSR_512          // OVER_SAMPLE_RATIO - larger value leads to less in-band noise and higher power consumption
#define RNG RNG_8G           // RANGE
#define ODR ODR_200Hz        // OUTPUT_DATA_RATE - lower frequency for lower power consumption
#define MODE MODE_CONTINUOUS // MODE_CONTROL

yh::rec::Qmc5883l::Qmc5883l (const uint8_t init_i2c_address, const uint8_t init_drdy_pin) :
    i2c_address(init_i2c_address),
    drdy_pin(init_drdy_pin)
{
    //
}

void yh::rec::Qmc5883l::begin () {
    // init settings to the GY-271 module through I2C
    if (!(TWCR & (1 << TWEN))) { // if (TwoWireENable bit is off) { begin I2C communication }
        Wire.begin();
        Wire.setClock(400000); // set I2C to fast mode for faster communication
    }
    Wire.beginTransmission(i2c_address); // address QMC5883L compass
    Wire.write(0x0B); // select register 0x0B - SET/RESET Period Register
    Wire.write(0x01); // write value of 0x01 (recommended by datasheet) to the register
    Wire.endTransmission();
    Wire.beginTransmission(i2c_address); // address QMC5883L compass
    Wire.write(0x09); // select register 0x09 - Control Register 1
    Wire.write(OSR | RNG | ODR | MODE); // write the selected configurations to the register
    Wire.endTransmission();
    if (drdy_pin != NC_PINNO) {
        pinMode(drdy_pin, INPUT);
        drdy_pin_in_reg = portInputRegister(digitalPinToPort(drdy_pin));
        drdy_pin_mask = digitalPinToBitMask(drdy_pin);
    } else {
        drdy_pin_in_reg = portInputRegister(digitalPinToPort(0));
        drdy_pin_mask = 0;
    }
}

void yh::rec::Qmc5883l::soft_reset () {
    Wire.beginTransmission(i2c_address); // address QMC5883L compass
    Wire.write(0x0A); // select register 0x0A - Control Register 2
    Wire.write(1 << 7); // write SOFT_RST bit to high
    Wire.endTransmission();
}

int16_t yh::rec::Qmc5883l::get_raw_x () {
    Wire.beginTransmission(i2c_address); // address QMC5883L compass
    Wire.write(0x00); // select register 0x00 - XOUT_L
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(2U)); // request 2 bytes from compass
    uint8_t low = Wire.available() ? Wire.read() : 0;
    uint8_t high = Wire.available() ? Wire.read() : 0;
    return (high << 8) | low;
}

int16_t yh::rec::Qmc5883l::get_raw_y () {
    Wire.beginTransmission(i2c_address); // address QMC5883L compass
    Wire.write(0x02); // select register 0x00 - YOUT_L
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(2U)); // request 2 bytes from compass
    uint8_t low = Wire.available() ? Wire.read() : 0;
    uint8_t high = Wire.available() ? Wire.read() : 0;
    return (high << 8) | low;
}

int16_t yh::rec::Qmc5883l::get_raw_z () {
    Wire.beginTransmission(i2c_address); // address QMC5883L compass
    Wire.write(0x04); // select register 0x04 - ZOUT_L
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(2U)); // request 2 bytes from compass
    uint8_t low = Wire.available() ? Wire.read() : 0;
    uint8_t high = Wire.available() ? Wire.read() : 0;
    return (high << 8) | low;
}

yh::rec::Mag_field_raw_t yh::rec::Qmc5883l::get_raw_mag () {
    Wire.beginTransmission(i2c_address); // address QMC5883L compass
    Wire.write(0x00); // select register 0x00 - XOUT_L
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(6U)); // request 6 bytes from compass
    yh::rec::Mag_field_raw_t temp;
    {
        uint8_t low = Wire.available() ? Wire.read() : 0;
        uint8_t high = Wire.available() ? Wire.read() : 0;
        temp.x = (high << 8) | low;
    }
    {
        uint8_t low = Wire.available() ? Wire.read() : 0;
        uint8_t high = Wire.available() ? Wire.read() : 0;
        temp.y = (high << 8) | low;
    }
    {
        uint8_t low = Wire.available() ? Wire.read() : 0;
        uint8_t high = Wire.available() ? Wire.read() : 0;
        temp.z = (high << 8) | low;
    }
    return temp;
}

int16_t yh::rec::Qmc5883l::get_raw_temperature () {
    Wire.beginTransmission(i2c_address); // address QMC5883L compass
    Wire.write(0x07); // select register 0x06 - TOUT_L
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(2U)); // request 2 bytes from compass
    uint8_t low = Wire.available() ? Wire.read() : 0;
    uint8_t high = Wire.available() ? Wire.read() : 0;
    return (high << 8) | low;
}

double yh::rec::Qmc5883l::get_uT_x () {
    #if (RNG == RNG_8G)
    return get_raw_x() / 3000.0;
    #else // (RNG == RNG_2G)
    return get_raw_x() / 12000.0;
    #endif
}

double yh::rec::Qmc5883l::get_uT_y () {
    #if (RNG == RNG_8G)
    return get_raw_y() / 3000.0;
    #else // (RNG == RNG_2G)
    return get_raw_y() / 12000.0;
    #endif
}

double yh::rec::Qmc5883l::get_uT_z () {
    #if (RNG == RNG_8G)
    return get_raw_z() / 3000.0;
    #else // (RNG == RNG_2G)
    return get_raw_z() / 12000.0;
    #endif
}

yh::rec::Mag_field_uT_t yh::rec::Qmc5883l::get_uT_mag () {
    Mag_field_uT_t temp;
    Mag_field_raw_t raw = get_raw_mag();
    #if (RNG == RNG_8G)
    temp.x = raw.x / 3000.0;
    temp.y = raw.y / 3000.0;
    temp.z = raw.z / 3000.0;
    #else // (RNG == RNG_2G)
    temp.x = raw.x / 12000.0;
    temp.y = raw.y / 12000.0;
    temp.z = raw.z / 12000.0;
    #endif
    return temp;
}

void yh::rec::Qmc5883l::compass_cal (uint16_t cal_times) {
    int16_t x_max_temp = -32768, x_min_temp = 32767, y_max_temp = -32768, y_min_temp = 32767;
    for (uint16_t i = 0; i < cal_times; i++) {
        Wire.beginTransmission(i2c_address); // address QMC5883L compass
        Wire.write(0x00); // select register 0x00 - XOUT_L
        Wire.endTransmission();
        Wire.requestFrom(i2c_address, static_cast<uint8_t>(4U)); // request 4 bytes from compass
        {
            const uint8_t xlow = Wire.available() ? Wire.read() : 0;
            const uint8_t xhigh = Wire.available() ? Wire.read() : 0;
            const int16_t x = (xhigh << 8) | xlow;
            if (x > x_max_temp) {
                x_max_temp = x;
            }
            if (x < x_min_temp) {
                x_min_temp = x;
            }
        }
        {
            const uint8_t ylow = Wire.available() ? Wire.read() : 0;
            const uint8_t yhigh = Wire.available() ? Wire.read() : 0;
            const int16_t y = (yhigh << 8) | ylow;
            if (y > y_max_temp) {
                y_max_temp = y;
            }
            if (y < y_min_temp) {
                y_min_temp = y;
            }
        }
    }
    x_range = x_max_temp - x_min_temp;
    x_min = x_min_temp;
    y_range = y_max_temp - y_min_temp;
    y_min = y_min_temp;
}

void yh::rec::Qmc5883l::reset_heading () {
    re_zero_heading = 0;
    re_zero_heading = get_heading();
}

uint16_t yh::rec::Qmc5883l::get_heading () {
    if (drdy_pin != NC_PINNO) {
        if ((*drdy_pin_in_reg) & drdy_pin_mask) {
            Wire.beginTransmission(i2c_address); // address QMC5883L compass
            Wire.write(0x00); // select register 0x00 - XOUT_L
            Wire.endTransmission();
            Wire.requestFrom(i2c_address, static_cast<uint8_t>(4U)); // request 4 bytes from compass
            const uint8_t xlow = Wire.available() ? Wire.read() : 0;
            const uint8_t xhigh = Wire.available() ? Wire.read() : 0;
            const int16_t x = (xhigh << 8) | xlow;
            const uint8_t ylow = Wire.available() ? Wire.read() : 0;
            const uint8_t yhigh = Wire.available() ? Wire.read() : 0;
            const int16_t y = (yhigh << 8) | ylow;
            int16_t temp_heading = 360 - atan2(static_cast<double>((y - y_min) * 2) / y_range - 1, static_cast<double>((x - x_min) * 2) / x_range - 1) * RAD_TO_DEG;
            temp_heading -= re_zero_heading;
            if (temp_heading < 0) {
                temp_heading += 360;
            }
            if (temp_heading >= 360) {
                temp_heading -= 360;
            }
            return (heading = temp_heading);
        }
    } else {
        Wire.beginTransmission(i2c_address); // address QMC5883L compass
        Wire.write(0x06); // select register 0x06 - Status Register
        Wire.endTransmission();
        Wire.requestFrom(i2c_address, static_cast<uint8_t>(1U));
        const uint8_t status = Wire.read();
        if (status & (1 << 2)) { // check if new data is ready
            if (!(status & (1 << 1))) { // check if overflow does not occur
                Wire.beginTransmission(i2c_address); // address QMC5883L compass
                Wire.write(0x00); // select register 0x00 - XOUT_L
                Wire.endTransmission();
                Wire.requestFrom(i2c_address, static_cast<uint8_t>(4U)); // request 4 bytes from compass
                const uint8_t xlow = Wire.available() ? Wire.read() : 0;
                const uint8_t xhigh = Wire.available() ? Wire.read() : 0;
                const int16_t x = (xhigh << 8) | xlow;
                const uint8_t ylow = Wire.available() ? Wire.read() : 0;
                const uint8_t yhigh = Wire.available() ? Wire.read() : 0;
                const int16_t y = (yhigh << 8) | ylow;
                int16_t temp_heading = 360 - atan2(static_cast<double>((y - y_min) * 2) / y_range - 1, static_cast<double>((x - x_min) * 2) / x_range - 1) * RAD_TO_DEG;
                temp_heading -= re_zero_heading;
                if (temp_heading < 0) {
                    temp_heading += 360;
                }
                if (temp_heading >= 360) {
                    temp_heading -= 360;
                }
                return (heading = temp_heading);
            }
        }
    }
    return heading;
}

#endif // #ifndef QMC5883L_CPP
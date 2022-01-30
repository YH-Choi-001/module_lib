#include "custom_gy521.h"

Custom_gy521::Custom_gy521 (const uint8_t init_i2c_address) :
    i2c_address(init_i2c_address)
{
    // roll = 0;
    // pitch = 0;
    yaw = 0;
}

inline void Custom_gy521::begin () {
    // init settings to the GY-521 module through I2C
    Wire.begin();
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(0x6B); // accessing the register 6B - Power Management
    Wire.write(0b00000000); // setting SLEEP register to 0
    Wire.endTransmission();  
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(0x1B); // accessing the register 1B - Gyroscope Configuration
    Wire.write(0b00010000); // setting the gyro to scale +/- 1000deg./s (2 of [0:3]) assume that the robot cannot rotate more than 2.5 circles per second
    Wire.endTransmission();
}

inline void Custom_gy521::reset_yaw () {
    yaw = 0;
}

double Custom_gy521::update_yaw (const double correction) {
    static unsigned long previous_micros_reading = micros();
    Wire.beginTransmission(i2c_address);
    Wire.write(0x47);
    Wire.endTransmission();
    Wire.requestFrom(i2c_address,2);
    // commands above take around 944 - 952 us
    while (Wire.available() < 2);
        const unsigned long t_diff = micros() - previous_micros_reading;
        yaw -= ( ( ((Wire.read()<<8) | Wire.read()) + correction) / 32800000.0 * t_diff   ); // angle change per sec * time past in secs
        previous_micros_reading += t_diff;
        if (yaw >= 360) yaw -= 360;
        if (yaw < 0) yaw += 360;
    return yaw;
}

inline double Custom_gy521::get_yaw () {
    return yaw;
}
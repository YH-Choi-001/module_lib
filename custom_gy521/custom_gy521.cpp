#include "custom_gy521.h"

uint16_t read_i2c_data_2_bytes (const uint8_t i2c_address, const uint8_t register_no) {
    Wire.beginTransmission(i2c_address);
    Wire.write(register_no);
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(2U));
    while (Wire.available() < 2) {}
    return (Wire.read() << 8) | Wire.read();
}

Custom_gy521::Custom_gy521 (const uint8_t init_i2c_address) :
    i2c_address(init_i2c_address), roll(0), pitch(0), yaw(0), corr_roll(0), corr_pitch(0), corr_yaw(0)
{
    //
}

void Custom_gy521::begin () {
    // init settings to the GY-521 module through I2C
    Wire.begin();
    Wire.setClock(400000);
    // Wire.beginTransmission(i2c_address);
    // switch (Wire.endTransmission()) { // perform check to ensure i2c_address is correct
    //     case 0:
    //         // i2c_address is correct
    //         break;
    //     case 4:
    //         // i2c_address confirmed, but unknown error occured
    //         break;
    //     default:
    //         // i2c_address error
    //         return;
    // }
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(0x6B); // accessing the register 6B - Power Management
    Wire.write(0b00000000); // setting SLEEP register to 0
    Wire.endTransmission();  
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(0x1B); // accessing the register 1B - Gyroscope Configuration
    Wire.write(0b00010000); // setting the gyro to scale +/- 1000deg./s (2 of [0:3]) assume that the robot cannot rotate more than 2.5 circles per second
    Wire.endTransmission();
}

uint8_t Custom_gy521::who_am_i () {
    Wire.beginTransmission(i2c_address);
    Wire.write(0x75);
    const uint8_t err = Wire.endTransmission();
    if (err) return 0;
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(1U));
    while (Wire.available() < 1) {}
    return Wire.read();
}

void Custom_gy521::cal_gyro (const uint16_t sampling_amount, const void (*updating_function)(void) = NULL) {
    corr_roll = 0;
    corr_pitch = 0;
    corr_yaw = 0;
    for (uint16_t i = 0; i < sampling_amount; i++) {
        Wire.beginTransmission(i2c_address);
        Wire.write(0x43);
        Wire.endTransmission();
        Wire.requestFrom(i2c_address, static_cast<uint8_t>(6U));
        // commands above take around 944 - 952 us
        while (Wire.available() < 6) {}
        corr_roll += static_cast<double>((Wire.read() << 8) | Wire.read());
        corr_pitch += static_cast<double>((Wire.read() << 8) | Wire.read());
        corr_yaw += static_cast<double>((Wire.read() << 8) | Wire.read());
        delayMicroseconds(10);
        if (updating_function)
            updating_function();
    }
    corr_roll /= -static_cast<double>(sampling_amount);
    corr_pitch /= -static_cast<double>(sampling_amount);
    corr_yaw /= -static_cast<double>(sampling_amount);
}

void Custom_gy521::update_gyro () {
    static unsigned long previous_micros_reading = micros();
    Wire.beginTransmission(i2c_address);
    Wire.write(0x43);
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(6U));
    // commands above take around 944 - 952 us
    while (Wire.available() < 6) {}
    const unsigned long t_diff = micros() - previous_micros_reading;
    roll += ( ((Wire.read()<<8) | Wire.read()) + corr_roll) / 32800000.0 * t_diff; // angle change per sec * time past in secs
    pitch -= ( ((Wire.read()<<8) | Wire.read()) + corr_pitch) / 32800000.0 * t_diff; // angle change per sec * time past in secs
    yaw -= ( ((Wire.read()<<8) | Wire.read()) + corr_yaw) / 32800000.0 * t_diff; // angle change per sec * time past in secs
    previous_micros_reading += t_diff;
    if (roll >= 360.0) roll -= 360.0;
    if (roll < 0.0) roll += 360.0;
    if (pitch >= 360.0) pitch -= 360.0;
    if (pitch < 0.0) pitch += 360.0;
    if (yaw >= 360.0) yaw -= 360.0;
    if (yaw < 0.0) yaw += 360.0;
}


// #define CAL_(axis,reg_addr,delay_time) \
// double Custom_gy521::cal_##axis (const uint16_t sampling_amount) { \
//     corr_##axis = 0; \
//     for (uint16_t i = 0; i < sampling_amount; i++) { \
//         corr_##axis += static_cast<double>(read_i2c_data_2_bytes(i2c_address, reg_addr)); \
//         delayMicroseconds(delay_time); \
//     } \
//     corr_##axis /= -static_cast<double>(sampling_amount); \
//     return corr_##axis; \
// }

// CAL_(roll,0x43,10)
// CAL_(pitch,0x45,10)
// CAL_(yaw,0x47,10)

// #undef CAL_

// #define UPDATE_(axis,sign,reg_addr) \
// double Custom_gy521::update_##axis () { \
//     static unsigned long previous_micros_reading = micros(); \
//     const unsigned long t_diff = micros() - previous_micros_reading; \
//     axis sign##= ( static_cast<double>(read_i2c_data_2_bytes(i2c_address, reg_addr)) + corr_##axis) / 32800000.0 * t_diff; \
//     previous_micros_reading += t_diff; \
//     if (axis >= 360.0) axis -= 360.0; \
//     if (axis < 0.0) axis += 360.0; \
//     return axis; \
// }

// UPDATE_(roll,+,0x43)
// UPDATE_(pitch,-,0x45)
// UPDATE_(yaw,-,0x47)

// #undef UPDATE_
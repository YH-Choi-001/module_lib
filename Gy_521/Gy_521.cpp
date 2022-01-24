#ifndef GY_521_CPP
#define GY_521_CPP __DATE__ ", " __TIME__

#include "Gy_521.h"

void yh::rec::Gy_521::write_i2c (const uint8_t target_i2c_addr, const uint8_t register_no, const uint8_t data) {
    Wire.beginTransmission(target_i2c_addr);
    Wire.write(register_no);
    Wire.write(data);
    Wire.endTransmission();
}

void yh::rec::Gy_521::read_i2c (const uint8_t target_i2c_addr, const uint8_t register_no, const uint8_t len, uint8_t *byte_array) {
    Wire.beginTransmission(target_i2c_addr);
    Wire.write(register_no);
    Wire.endTransmission();
    Wire.requestFrom(target_i2c_addr, len);
    for (uint8_t idx = 0; idx < len; idx++) {
        while (Wire.available() < 1);
        byte_array[idx] = Wire.read();
    }
}

yh::rec::Gy_521::Gy_521 (const uint8_t init_i2c_address) :
    i2c_address(init_i2c_address),
    accel_LSB_sensitivity(16384.0),
    gyro_LSB_sensitivity(131.0)
{
    //
}

inline void yh::rec::Gy_521::begin () {
    Wire.begin();
    // accessing the register 6B - Power Management (Sec. 4.28)
    // setting SLEEP register to 0. (Required; see Note on p. 9)
    write_i2c(i2c_address, 0x6B, 0b00000000);
}

// ====================================================================================================
// ========================================== accelerometer ===========================================
// ====================================================================================================

void yh::rec::Gy_521::set_accel_range (const uint8_t set_accel_range) {
    accel_range = set_accel_range > 3 ? 3 : set_accel_range;
    write_i2c(i2c_address, 0x1C, (accel_range << 3) & 0b00011000); // register 0x1C - gyroscope config
    accel_LSB_sensitivity = 16384.0 / static_cast<double>(1U << accel_range);
}

uint16_t yh::rec::Gy_521::get_raw_accel_x () {
    {
        uint8_t temp_buf [2];
        read_i2c(i2c_address, 0x3B, 2, temp_buf); // register 0x3B to 0x3C - accelerometer x
        raw_accel_x = (static_cast<uint16_t>(temp_buf[0]) << 8) | temp_buf[1];
    }
    return raw_accel_x;
}

uint16_t yh::rec::Gy_521::get_raw_accel_y () {
    {
        uint8_t temp_buf [2];
        read_i2c(i2c_address, 0x3D, 2, temp_buf); // register 0x3D to 0x3E - accelerometer y
        raw_accel_y = (static_cast<uint16_t>(temp_buf[0]) << 8) | temp_buf[1];
    }
    return raw_accel_y;
}

uint16_t yh::rec::Gy_521::get_raw_accel_z () {
    {
        uint8_t temp_buf [2];
        read_i2c(i2c_address, 0x3F, 2, temp_buf); // register 0x3F to 0x40 - accelerometer x
        raw_accel_z = (static_cast<uint16_t>(temp_buf[0]) << 8) | temp_buf[1];
    }
    return raw_accel_z;
}

uint64_t yh::rec::Gy_521::get_all_raw_accel () {
    {
        uint8_t temp_buf [6];
        read_i2c(i2c_address, 0x3B, 6, temp_buf); // register 0x3B to 0x40 - accelerometer x y z
        raw_accel_x = (static_cast<uint16_t>(temp_buf[0]) << 8) | temp_buf[1];
        raw_accel_y = (static_cast<uint16_t>(temp_buf[2]) << 8) | temp_buf[3];
        raw_accel_z = (static_cast<uint16_t>(temp_buf[4]) << 8) | temp_buf[5];
    }
    return (static_cast<uint64_t>(raw_accel_x) << 32) | (static_cast<uint64_t>(raw_accel_y) << 16) | raw_accel_z;
}

double yh::rec::Gy_521::get_accel_x () {
    {
        accel_x = static_cast<double>(get_raw_accel_x()) / accel_LSB_sensitivity;
    }
    return accel_x;
}

double yh::rec::Gy_521::get_accel_y () {
    {
        accel_y = static_cast<double>(get_raw_accel_y())/ accel_LSB_sensitivity;
    }
    return accel_y;
}

double yh::rec::Gy_521::get_accel_z () {
    {
        accel_z = static_cast<double>(get_raw_accel_z()) / accel_LSB_sensitivity;
    }
    return accel_z;
}

double yh::rec::Gy_521::cal_accel_x () {
    unsigned long time_spent = 0;
    unsigned long prev_log_time = millis();
    double cal_val = 0;
    for (uint16_t i = 0; i < 2048; i++) {
        unsigned long time_diff = millis() - prev_log_time;
        cal_val += get_accel_x() * time_diff / 1000.0;
        time_spent += time_diff;
        prev_log_time += time_diff;
    }
    return accel_x_corr = cal_val / time_spent;
}

double yh::rec::Gy_521::cal_accel_y () {
    unsigned long time_spent = 0;
    unsigned long prev_log_time = millis();
    double cal_val = 0;
    for (uint16_t i = 0; i < 2048; i++) {
        unsigned long time_diff = millis() - prev_log_time;
        cal_val += get_accel_y() * time_diff / 1000.0;
        time_spent += time_diff;
        prev_log_time += time_diff;
    }
    return accel_y_corr = cal_val / time_spent;
}

double yh::rec::Gy_521::cal_accel_z () {
    unsigned long time_spent = 0;
    unsigned long prev_log_time = millis();
    double cal_val = 0;
    for (uint16_t i = 0; i < 2048; i++) {
        unsigned long time_diff = millis() - prev_log_time;
        cal_val += get_accel_z() * time_diff / 1000.0;
        time_spent += time_diff;
        prev_log_time += time_diff;
    }
    return accel_z_corr = cal_val / time_spent;
}

// ====================================================================================================
// =========================================== thermometer ============================================
// ====================================================================================================

uint16_t yh::rec::Gy_521::get_temp () {
    {
        uint8_t temp_buf [2];
        read_i2c(i2c_address, 0x41, 2, temp_buf); // register 0x41 to 0x42 - thermometer
        temp = (static_cast<uint16_t>(temp_buf[0]) << 8) | temp_buf[1];
    }
    return temp;
}

// ====================================================================================================
// ============================================ gyroscopes ============================================
// ====================================================================================================

void yh::rec::Gy_521::set_gyro_range (const uint8_t set_gyro_range) {
    gyro_range = set_gyro_range > 3 ? 3 : set_gyro_range;
    write_i2c(i2c_address, 0x1B, (gyro_range << 3) & 0b00011000); // register 0x1B - gyroscope config
    // gyro_LSB_sensitivity = 131.0 / static_cast<double>(1U << gyro_range);
    switch (gyro_range) {
        case 0:
            gyro_LSB_sensitivity = 131.0;
            break;
        case 1:
            gyro_LSB_sensitivity = 65.5;
            break;
        case 2:
            gyro_LSB_sensitivity = 32.8;
            break;
        case 3:
            gyro_LSB_sensitivity = 16.4;
            break;
    }
}

uint16_t yh::rec::Gy_521::get_raw_gyro_x () {
    {
        uint8_t temp_buf [2];
        read_i2c(i2c_address, 0x43, 2, temp_buf); // register 0x43 to 0x44 - gyroscope x
        raw_gyro_x = (static_cast<uint16_t>(temp_buf[0]) << 8) | temp_buf[1];
    }
    return raw_gyro_x;
}

uint16_t yh::rec::Gy_521::get_raw_gyro_y () {
    {
        uint8_t temp_buf [2];
        read_i2c(i2c_address, 0x45, 2, temp_buf); // register 0x45 to 0x46 - gyroscope y
        raw_gyro_y = (static_cast<uint16_t>(temp_buf[0]) << 8) | temp_buf[1];
    }
    return raw_gyro_y;
}

uint16_t yh::rec::Gy_521::get_raw_gyro_z () {
    {
        uint8_t temp_buf [2];
        read_i2c(i2c_address, 0x47, 2, temp_buf); // register 0x47 to 0x48 - gyroscope z
        raw_gyro_z = (static_cast<uint16_t>(temp_buf[0]) << 8) | temp_buf[1];
    }
    return raw_gyro_z;
}

uint64_t yh::rec::Gy_521::get_all_raw_gyro () {
    {
        uint8_t temp_buf [6];
        read_i2c(i2c_address, 0x43, 6, temp_buf); // register 0x43 to 0x48 - gyroscope x y z
        raw_gyro_x = (static_cast<uint16_t>(temp_buf[0]) << 8) | temp_buf[1];
        raw_gyro_y = (static_cast<uint16_t>(temp_buf[2]) << 8) | temp_buf[3];
        raw_gyro_z = (static_cast<uint16_t>(temp_buf[4]) << 8) | temp_buf[5];
    }
    return (static_cast<uint64_t>(raw_gyro_x) << 32) | (static_cast<uint64_t>(raw_gyro_y) << 16) | raw_gyro_z;
}

double yh::rec::Gy_521::get_gyro_x () {
    {
        gyro_x = static_cast<double>(get_raw_gyro_x()) / gyro_LSB_sensitivity;
    }
    return gyro_x;
}

double yh::rec::Gy_521::get_gyro_y () {
    {
        gyro_y = static_cast<double>(get_raw_gyro_y()) / gyro_LSB_sensitivity;
    }
    return gyro_y;
}

double yh::rec::Gy_521::get_gyro_z () {
    {
        gyro_z = static_cast<double>(get_raw_gyro_z()) / gyro_LSB_sensitivity;
    }
    return gyro_z;
}

double yh::rec::Gy_521::cal_gyro_x () {
    unsigned long time_spent = 0;
    unsigned long prev_log_time = millis();
    double cal_val = 0;
    for (uint16_t i = 0; i < 2048; i++) {
        unsigned long time_diff = millis() - prev_log_time;
        cal_val += get_gyro_x() * time_diff / 1000.0;
        time_spent += time_diff;
        prev_log_time += time_diff;
    }
    return gyro_x_corr = cal_val / time_spent;
}

double yh::rec::Gy_521::cal_gyro_y () {
    unsigned long time_spent = 0;
    unsigned long prev_log_time = millis();
    double cal_val = 0;
    for (uint16_t i = 0; i < 2048; i++) {
        unsigned long time_diff = millis() - prev_log_time;
        cal_val += get_gyro_y() * time_diff / 1000.0;
        time_spent += time_diff;
        prev_log_time += time_diff;
    }
    return gyro_y_corr = cal_val / time_spent;
}

double yh::rec::Gy_521::cal_gyro_z () {
    unsigned long time_spent = 0;
    double cal_val = 0;
    unsigned long prev_log_time = millis();
    for (uint16_t i = 0; i < 2048; i++) {
        unsigned long time_diff = millis() - prev_log_time;
        // cal_val += get_gyro_z() * time_diff;
        // time_spent += time_diff;
        // cal_val += get_gyro_z();

        // cal_val = (cal_val * i + get_gyro_z() / time_diff);
        cal_val += get_gyro_z() * time_diff / 1000.0;
        prev_log_time += time_diff;
    }
    // return gyro_z_corr = cal_val / time_spent;
    return gyro_z_corr = cal_val / 2048.0;
}

// ====================================================================================================
// =============================================== all ================================================
// ====================================================================================================

void yh::rec::Gy_521::get_raw_accel_temp_gyro () {
    {
        uint8_t temp_buf [14];
        read_i2c(i2c_address, 0x3B, 14, temp_buf); // register 0x3B to 0x48 - accel x y z temp gyro x y z
        raw_accel_x = (static_cast<uint16_t>(temp_buf[0]) << 8)  | temp_buf[1];
        raw_accel_y = (static_cast<uint16_t>(temp_buf[2]) << 8)  | temp_buf[3];
        raw_accel_z = (static_cast<uint16_t>(temp_buf[4]) << 8)  | temp_buf[5];
        temp    = (static_cast<uint16_t>(temp_buf[6]) << 8)  | temp_buf[7];
        raw_gyro_x  = (static_cast<uint16_t>(temp_buf[8]) << 8)  | temp_buf[9];
        raw_gyro_y  = (static_cast<uint16_t>(temp_buf[10]) << 8) | temp_buf[11];
        raw_gyro_z  = (static_cast<uint16_t>(temp_buf[12]) << 8) | temp_buf[13];
    }
}

#endif //#ifndef GY_521_CPP
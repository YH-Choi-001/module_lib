#ifndef CUSTOM_GY521_CPP
#define CUSTOM_GY521_CPP __DATE__ ", " __TIME__

#include "custom_gy521.h"

Quaternion &Quaternion::operator = (const Quaternion rhs) {
    this->w = rhs.w;
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    return *this;
}

Quaternion &Quaternion::operator *= (const Quaternion rhs) {
    return (*this) = ((*this) * rhs);
}

inline double Quaternion::vectors_squared () {
    return (w * w + x * x + y * y + z * z);
}

inline double Quaternion::getMagnitude() {
    return sqrt(this->vectors_squared());
}

inline void Quaternion::normalize() {
    double m = this->getMagnitude();
    w /= m;
    x /= m;
    y /= m;
    z /= m;
}

Quaternion operator * (const Quaternion lhs, const Quaternion rhs) {
    // Quaternion multiplication is defined by:
    //     (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
    //     (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
    //     (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
    //     (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2
    return Quaternion(
        (lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z),
        (lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y),
        (lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x),
        (lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w)
    );
}

// waits for the buffer to be filled, return 1 if timeout
bool wait_i2c_buf (const uint8_t buflen) __attribute__((__always_inline__));
bool wait_i2c_buf (const uint8_t buflen) {
    const unsigned long starting_time = micros();
    const unsigned long max_waiting_time = (buflen * 500);
    while (Wire.available() < buflen) {
        if ((micros() - starting_time) > max_waiting_time) return 1;
    }
    return 0;
}

Custom_gy521::Custom_gy521 (const uint8_t init_i2c_address) :
    i2c_address(init_i2c_address), /*roll(0), pitch(0),*/ yaw(0), corr_roll(0), corr_pitch(0), corr_yaw(0), q(1, 0, 0, 0)
{
    //
}

void Custom_gy521::begin () {
    // init settings to the GY-521 module through I2C
    if (!(TWCR & (1 << TWEN))) { // if (TwoWireENable bit is off) { begin I2C communication }
        Wire.begin();
        Wire.setClock(400000); // set I2C to fast mode for faster communication
    }
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(0x6B); // accessing the register 6B - Power Management 1
    Wire.write(0b00000000); // setting SLEEP bit to 0
    Wire.endTransmission();
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(0x6C); // accessing the register 6C - Power Management 2
    Wire.write(0b00000000); // enabling accel x y z, enabling gyro x y z
    Wire.endTransmission();
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(0x1B); // accessing the register 1B - Gyroscope Configuration
    // Wire.write(0b00010000); // setting the gyro to scale +/- 1000deg./s (2 of [0:3]) assume that the robot cannot rotate more than 2.5 circles per second
    Wire.write(0b00011000); // setting the gyro to scale +/- 2000deg./s (3 of [0:3]) to minimize chance of error when the referee lifts up the robot during match
    Wire.endTransmission();
    prev_micros_reading = micros();
}

uint8_t Custom_gy521::who_am_i () {
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(0x75); // accessing the who am I register
    const uint8_t err = Wire.endTransmission();
    if (err) return 0; // an error occured while communicating with the chip
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(1U));
    wait_i2c_buf(1U);
    return Wire.read();
}

double Custom_gy521::update_temp () {
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(0x41); // accessing the register of thermometer, which has 2 bytes, from 0x41 to 0x42
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(2U));
    wait_i2c_buf(2U);
    const int16_t TEMP_OUT = Wire.read() << 8 | Wire.read();
    if (this->who_am_i() == 0x68) {
    // MPU-6000 or MPU-6050
        return TEMP_OUT / 340.0 + 36.53; // this formula is from register map
    } else {
        // MPU-6500 or MPU-9250
        return (TEMP_OUT - 21/*room_temp_offset*/) / 333.87/*sensitivity in LSB per degree Celsius */ + 21.0; // this formula is from register map
    }
}

void Custom_gy521::cal_gyro (const uint32_t sampling_amount, void (*updating_function)(void)) {
    corr_roll = 0;
    corr_pitch = 0;
    corr_yaw = 0;
    for (uint32_t i = 0; i < sampling_amount; i++) {
        Wire.beginTransmission(i2c_address); // talk to GY-521
        Wire.write(0x43); // accessing the registers of gyroscope x, y, z, where each axis has 2 bytes, from 0x43 to 0x48
        Wire.endTransmission();
        Wire.requestFrom(i2c_address, static_cast<uint8_t>(6U));
        // commands above take around 944 - 952 us in 100 KHz clock frequency
        if (wait_i2c_buf(6U)) {
            continue;
        }
        corr_roll -= static_cast<double>((Wire.read() << 8) | Wire.read());
        corr_pitch -= static_cast<double>((Wire.read() << 8) | Wire.read());
        corr_yaw -= static_cast<double>((Wire.read() << 8) | Wire.read());
        delayMicroseconds(10);
        if (updating_function)
            updating_function();
    }
    corr_roll /= static_cast<double>(sampling_amount);
    corr_pitch /= static_cast<double>(sampling_amount);
    corr_yaw /= static_cast<double>(sampling_amount);
}

void Custom_gy521::reset_gyro () {
    q.w = 1;
    q.x = 0;
    q.y = 0;
    q.z = 0;
}

void Custom_gy521::update_gyro () {
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(0x43); // accessing the registers of gyroscope x, y, z, where each axis has 2 bytes, from 0x43 to 0x48
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, static_cast<uint8_t>(6U));
    // commands above take around 944 - 952 us in 100 KHz clock frequency
    if (wait_i2c_buf(6U)) return;
    const unsigned long t_diff = micros() - prev_micros_reading;

    // for sensitivity == +- 1000 degree per sec.
    // d_roll = ( ((Wire.read()<<8) | Wire.read()) + corr_roll) / 32800000.0 * t_diff; // angle change per sec * time past in secs
    // d_pitch = ( ((Wire.read()<<8) | Wire.read()) + corr_pitch) / 32800000.0 * t_diff; // angle change per sec * time past in secs
    // d_yaw = ( ((Wire.read()<<8) | Wire.read()) + corr_yaw) / 32800000.0 * t_diff; // angle change per sec * time past in secs

    // for sensitivity == +- 2000 degree per sec.
    d_roll = ( ((Wire.read()<<8) | Wire.read()) + corr_roll) / 16400000.0 * t_diff; // angle change per sec * time past in secs
    d_pitch = ( ((Wire.read()<<8) | Wire.read()) + corr_pitch) / 16400000.0 * t_diff; // angle change per sec * time past in secs
    d_yaw = ( ((Wire.read()<<8) | Wire.read()) + corr_yaw) / 16400000.0 * t_diff; // angle change per sec * time past in secs

    // // without quaternion calculations:
    // d_roll = ( ((Wire.read()<<8) | Wire.read())) / 32800000.0 * t_diff; // angle change per sec * time past in secs
    // d_pitch = -( ((Wire.read()<<8) | Wire.read())) / 32800000.0 * t_diff; // angle change per sec * time past in secs
    // d_yaw = -( ((Wire.read()<<8) | Wire.read())) / 32800000.0 * t_diff; // angle change per sec * time past in secs

    #ifdef ALL_AXES_ROTATION
    struct Some_vectors {
        double x, y, z;
    } gravity;
    #endif // #ifdef ALL_AXES_ROTATION
    const double d_yaw_rad_2 = d_yaw * PI / 360.0;
    const double d_pitch_rad_2 = d_pitch * PI / 360.0;
    const double d_roll_rad_2 = d_roll * PI / 360.0;
    // delta Quaternion
    const double
        cy = cos(d_yaw_rad_2),
        sy = sin(d_yaw_rad_2),
        cp = cos(d_pitch_rad_2),
        sp = sin(d_pitch_rad_2),
        cr = cos(d_roll_rad_2),
        sr = sin(d_roll_rad_2);
    Quaternion delta (
        cr * cp * cy + sr * sp * sy,
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy
    );
    q *= delta;
    q.normalize();

    #ifdef ALL_AXES_ROTATION
    gravity.x = 2 * (q.x * q.z - q.w * q.y);
    gravity.y = 2 * (q.w * q.x + q.y * q.z);
    gravity.z = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
    #endif // #ifdef ALL_AXES_ROTATION

    #ifdef ALL_AXES_ROTATION
    roll = atan(gravity.y / sqrt(gravity.x * gravity.x + gravity.z * gravity.z)) * RAD_TO_DEG;
    pitch = atan(gravity.x / sqrt(gravity.y * gravity.y + gravity.z * gravity.z)) * RAD_TO_DEG;
    #endif // #ifdef ALL_AXES_ROTATION
    //
    //
    //
        // delta.w = cr * cp * cy + sr * sp * sy,
        // delta.x = sr * cp * cy - cr * sp * sy,
        // delta.y = cr * sp * cy + sr * cp * sy,
        // delta.z = cr * cp * sy - sr * sp * cy
    //
    //
    //
        // q.w = (lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z),
        // q.x = (lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y),
        // q.y = (lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x),
        // q.z = (lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w)
    //
    //
    //
    // yaw = atan2(2 * q.x * q.y - 2 * q.w * q.z, 2 * q.w * q.w + 2 * q.x * q.x - 1) * RAD_TO_DEG;
    yaw = atan2(2 * (q.x * q.y - q.w * q.z), 2 * (q.w * q.w + q.x * q.x) - 1) * RAD_TO_DEG;

    prev_micros_reading += t_diff;
    // if (roll >= 360.0) roll -= 360.0;
    // if (roll < 0.0) roll += 360.0;
    // if (pitch >= 360.0) pitch -= 360.0;
    // if (pitch < 0.0) pitch += 360.0;
    if (yaw >= 360.0) yaw -= 360.0;
    if (yaw < 0.0) yaw += 360.0;
}


void Custom_gy521::enable_ext_i2c_slave_sensors () {
    Wire.beginTransmission(i2c_address);
    Wire.write(0x37);
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, 1);
    if (wait_i2c_buf(1U)) return;
    const uint8_t old_val = Wire.read();
    // write new value to MPU-9250 at register 0x37 with BYPASS_EN bit set HIGH
    Wire.beginTransmission(i2c_address);
    Wire.write(0x37);
    Wire.write(old_val | 0x02); // 0x02 is the BYPASS_EN bit
    Wire.endTransmission();
}

#endif // #ifndef CUSTOM_GY521_CPP
#ifndef MPU_6050_CPP
#define MPU_6050_CPP __DATE__ ", " __TIME__

#include "Mpu_6050.h"

// #define GYRO_RANGE 1000
#define GYRO_RANGE 2000

yh::rec::Quaternion operator * (const yh::rec::Quaternion lhs, const yh::rec::Quaternion rhs) {

    // definitions:
    // i^2 = j^2 = k^2 = ijk = -1
    // ij = k, ji = -k
    // ki = j, ik = -j
    // jk = i, kj = -i

    //      Quaternion 1     *     Quaternion 2
    //   (a + bi + cj + dk)  *  (e + fi + gj + hk)
    // = a(e + fi + gj + hk)  + bi(e + fi + gj + hk)     + cj(e + fi + gj + hk)     + dk(e + fi + gj + hk)
    // = ae + afi + agj + ahk + bie + bifi + bigj + bihk + cje + cjfi + cjgj + cjhk + dke + dkfi + dkgj + dkhk
    // = ae + afi + agj + ahk + bei - bf   +bgk   - bhj  + cej - cfk  - cg   + chi  + dek + dfj  - dgi  - dh
    // = (ae - bf - cg - dh) + (af + be + ch - dg)i + (ag - bh + ce + df)j + (ah + bg - cf + de)k

    // Quaternion multiplication is defined by:
    //     (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
    //     (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
    //     (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
    //     (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2
    return yh::rec::Quaternion(
        (lhs.w * rhs.w   -   lhs.x * rhs.x   -   lhs.y * rhs.y   -   lhs.z * rhs.z),
        (lhs.w * rhs.x   +   lhs.x * rhs.w   +   lhs.y * rhs.z   -   lhs.z * rhs.y),
        (lhs.w * rhs.y   -   lhs.x * rhs.z   +   lhs.y * rhs.w   +   lhs.z * rhs.x),
        (lhs.w * rhs.z   +   lhs.x * rhs.y   -   lhs.y * rhs.x   +   lhs.z * rhs.w)
    );
}

yh::rec::Mpu_6050::Mpu_6050 (const uint8_t init_i2c_address) :
    i2c_address(init_i2c_address), d_roll(0), d_pitch(0), d_yaw(0), corr_roll(0), corr_pitch(0), corr_yaw(0), q(1, 0, 0, 0)
{
    //
}

void yh::rec::Mpu_6050::begin () {
    // init settings to the GY-521 module through I2C
    if (!(TWCR & (1 << TWEN))) { // if (TwoWireENable bit is off) { begin I2C communication }
        Wire.begin();
        Wire.setClock(400000); // set I2C to fast mode for faster communication
    }
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(static_cast<uint8_t>(0x6B)); // accessing the register 6B - Power Management 1
    Wire.write(static_cast<uint8_t>(0b00000000)); // setting SLEEP bit to 0
    // Wire.endTransmission();
    // Wire.beginTransmission(i2c_address); // talk to GY-521
    // Wire.write(static_cast<uint8_t>(0x6C)); // accessing the register 6C - Power Management 2
    Wire.write(static_cast<uint8_t>(0b00000000)); // enabling accel x y z, enabling gyro x y z
    Wire.endTransmission();
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(static_cast<uint8_t>(0x1B)); // accessing the register 1B - Gyroscope Configuration
    #if (GYRO_RANGE == 250)
    Wire.write(static_cast<uint8_t>(0b00000000)); // setting the gyro to scale +/- 250deg./s (0 of [0:3])
    #elif (GYRO_RANGE == 500)
    Wire.write(static_cast<uint8_t>(0b00001000)); // setting the gyro to scale +/- 500deg./s (1 of [0:3])
    #elif (GYRO_RANGE == 1000)
    Wire.write(static_cast<uint8_t>(0b00010000)); // setting the gyro to scale +/- 1000deg./s (2 of [0:3]) assume that the robot cannot rotate more than 2.5 circles per second
    #elif (GYRO_RANGE == 2000)
    Wire.write(static_cast<uint8_t>(0b00011000)); // setting the gyro to scale +/- 2000deg./s (3 of [0:3]) to minimize chance of error when the referee lifts up the robot during match
    #endif
    Wire.endTransmission();
    prev_micros_reading = micros();
}

uint8_t yh::rec::Mpu_6050::who_am_i () {
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(static_cast<uint8_t>(0x75)); // accessing the who am I register
    if (Wire.endTransmission()) return 0; // an error occured while communicating with the chip
    if (Wire.requestFrom(i2c_address, static_cast<uint8_t>(1U)) == static_cast<uint8_t>(1U))
        return Wire.read();
    return 0;
}

double yh::rec::Mpu_6050::get_temp () {
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(static_cast<uint8_t>(0x41)); // accessing the register of thermometer, which has 2 bytes, from 0x41 to 0x42
    Wire.endTransmission();
    if (Wire.requestFrom(i2c_address, static_cast<uint8_t>(2U)) == static_cast<uint8_t>(2U)) {
        const int16_t TEMP_OUT = ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() );
        if (this->who_am_i() == 0x68) {
        // MPU-6000 or MPU-6050
            return TEMP_OUT / 340.0 + 36.53; // this formula is from register map
        } else {
            // MPU-6500 or MPU-9250
            return (TEMP_OUT - 21/*room_temp_offset*/) / 333.87/*sensitivity in LSB per degree Celsius */ + 21.0; // this formula is from register map
        }
    } else
        return 0;
}

void yh::rec::Mpu_6050::cal_gyro (const uint32_t sampling_amount, void (*updating_function)(void)) {
    corr_roll  = 0;
    corr_pitch = 0;
    corr_yaw   = 0;
    for (uint32_t i = 0; i < sampling_amount; i++) {
        Wire.beginTransmission(i2c_address); // talk to GY-521
        Wire.write(static_cast<uint8_t>(0x43)); // accessing the registers of gyroscope x, y, z, where each axis has 2 bytes, from 0x43 to 0x48
        Wire.endTransmission();
        if (Wire.requestFrom(i2c_address, static_cast<uint8_t>(6U)) == static_cast<uint8_t>(6U)) {
            corr_roll  += static_cast<double>( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() );
            corr_pitch += static_cast<double>( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() );
            corr_yaw   += static_cast<double>( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() );
            if (updating_function)
                updating_function();
        }
        delayMicroseconds(10);
    }
    corr_roll  /= static_cast<double>(sampling_amount);
    corr_pitch /= static_cast<double>(sampling_amount);
    corr_yaw   /= static_cast<double>(sampling_amount);
    #if 0 // we subtract the offset by ourselves instead of letting the mpu chip do the job,
          // because built-in offset-subtracting feature is only available on MPU6500 and MPU9250.
    // by register map of MPU9250, n_OFFS_USR == OffsetLSB * 2^(FS_SEL - 2)
    #if (GYRO_RANGE == 250) // FS_SEL == 0
    corr_roll  /= 4;
    corr_pitch /= 4;
    corr_yaw   /= 4;
    #elif (GYRO_RANGE == 500) // FS_SEL == 1
    corr_roll  /= 2;
    corr_pitch /= 2;
    corr_yaw   /= 2;
    #elif (GYRO_RANGE == 1000) // FS_SEL == 2
    // nothing to do
    #elif (GYRO_RANGE == 2000) // FS_SEL == 3
    corr_roll  *= 2;
    corr_pitch *= 2;
    corr_yaw   *= 2;
    #endif
    Wire.beginTransmission(i2c_address);
    Wire.write(0x13); // accessing the gyro offset (user) registers
    {
        uint16_t temp_buf [3] = {
            static_cast<int16_t>(corr_roll),
            static_cast<int16_t>(corr_pitch),
            static_cast<int16_t>(corr_yaw)
        };
        Wire.write(temp_buf[0] >> 8);
        Wire.write(temp_buf[0] & 0xff);
        Wire.write(temp_buf[1] >> 8);
        Wire.write(temp_buf[1] & 0xff);
        Wire.write(temp_buf[2] >> 8);
        Wire.write(temp_buf[2] & 0xff);
    }
    Wire.endTransmission();
    #endif
}

void yh::rec::Mpu_6050::reset_gyro () {
    q.w = 1;
    q.x = 0;
    q.y = 0;
    q.z = 0;
}

void yh::rec::Mpu_6050::update_gyro () {
    Wire.beginTransmission(i2c_address); // talk to GY-521
    Wire.write(static_cast<uint8_t>(0x43)); // accessing the registers of gyroscope x, y, z, where each axis has 2 bytes, from 0x43 to 0x48
    Wire.endTransmission();
    if (Wire.requestFrom(i2c_address, static_cast<uint8_t>(6U)) == static_cast<uint8_t>(6U)) {
        // commands above take around 944 - 952 us at 100 KHz I2C clock rate
        const unsigned long t_diff = micros() - prev_micros_reading;

        #if (GYRO_RANGE == 250)
        // for sensitivity == +- 250 degree per sec.
        d_roll  = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_roll    ) / 131000000.0 * t_diff; // angle change per sec * time past in secs
        d_pitch = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_pitch   ) / 131000000.0 * t_diff; // angle change per sec * time past in secs
        d_yaw   = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_yaw     ) / 131000000.0 * t_diff; // angle change per sec * time past in secs
        #elif (GYRO_RANGE == 500)
        // for sensitivity == +- 500 degree per sec.
        d_roll  = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_roll    ) / 65500000.0 * t_diff; // angle change per sec * time past in secs
        d_pitch = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_pitch   ) / 65500000.0 * t_diff; // angle change per sec * time past in secs
        d_yaw   = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_yaw     ) / 65500000.0 * t_diff; // angle change per sec * time past in secs
        #elif (GYRO_RANGE == 1000)
        // for sensitivity == +- 1000 degree per sec.
        d_roll  = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_roll    ) / 32800000.0 * t_diff; // angle change per sec * time past in secs
        d_pitch = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_pitch   ) / 32800000.0 * t_diff; // angle change per sec * time past in secs
        d_yaw   = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_yaw     ) / 32800000.0 * t_diff; // angle change per sec * time past in secs
        #elif (GYRO_RANGE == 2000)
        // for sensitivity == +- 2000 degree per sec.
        d_roll  = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_roll    ) / 16400000.0 * t_diff; // angle change per sec * time past in secs
        d_pitch = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_pitch   ) / 16400000.0 * t_diff; // angle change per sec * time past in secs
        d_yaw   = (   ( (static_cast<int16_t>(Wire.read()) << 8) | Wire.read() ) - corr_yaw     ) / 16400000.0 * t_diff; // angle change per sec * time past in secs
        #endif

        const double d_roll_rad_div_2  = PI / 360.0 * d_roll;
        const double d_pitch_rad_div_2 = PI / 360.0 * d_pitch;
        const double d_yaw_rad_div_2   = PI / 360.0 * d_yaw;
        // delta Quaternion
        // a little note that sin(pi - theta) == sin(theta), and cos(-theta) == cos(theta)
        const double
            cy = cos(d_yaw_rad_div_2),
            sy = sin(d_yaw_rad_div_2),
            cp = cos(d_pitch_rad_div_2),
            sp = sin(d_pitch_rad_div_2),
            cr = cos(d_roll_rad_div_2),
            sr = sin(d_roll_rad_div_2);
        q *= Quaternion
        (
            cr * cp * cy + sr * sp * sy,
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy  // this is the rotation expressed in quaternions
        );
        q.normalize();

        prev_micros_reading += t_diff;
    }
}

double yh::rec::Mpu_6050::get_roll () {
    uint8_t oldSREG = SREG;
    noInterrupts();
    const double
        gravity_x = 2 * (q.x * q.z - q.w * q.y),
        gravity_y = 2 * (q.w * q.x + q.y * q.z),
        // gravity_z = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
        gravity_z = 2 * (q.w * q.w + q.z * q.z) - 1;
    // since w^2 + x^2 + y^2 + z^2 = 1
    //                   w^2 + z^2 = 1 - x^2 - y^2
    //               w^2 + z^2 - 1 = - x^2 - y^2
    // 2 * (q.w * q.w + q.z * q.z) - 1
    SREG = oldSREG;
    return atan(gravity_y / sqrt(gravity_x * gravity_x + gravity_z * gravity_z)) * RAD_TO_DEG; // gravity_y == x, gravity_x and gravity_z are y and z
}

double yh::rec::Mpu_6050::get_pitch () {
    uint8_t oldSREG = SREG;
    noInterrupts();
    const double
        gravity_x = 2 * (q.x * q.z - q.w * q.y),
        gravity_y = 2 * (q.w * q.x + q.y * q.z),
        // gravity_z = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
        gravity_z = 2 * (q.w * q.w + q.z * q.z) - 1;
    SREG = oldSREG;
    return atan(gravity_x / sqrt(gravity_y * gravity_y + gravity_z * gravity_z)) * RAD_TO_DEG; // gravity_x == z, gravity_y and gravity_z are x and y // gravity_x == z, gravity_y == x, gravity_z == y
}

double yh::rec::Mpu_6050::get_yaw () {
    uint8_t oldSREG = SREG;
    noInterrupts();
    // yaw = atan2(2 * q.x * q.y - 2 * q.w * q.z, 2 * q.w * q.w + 2 * q.x * q.x - 1) * RAD_TO_DEG;
    const double yaw = atan2(2 * (q.x * q.y - q.w * q.z), 2 * (q.w * q.w + q.x * q.x) - 1) * RAD_TO_DEG;
    SREG = oldSREG;
    return yaw < 0 ? yaw + 360.0 : yaw;
}

yh::rec::Euler_angle yh::rec::Mpu_6050::get_euler_angles () {
    uint8_t oldSREG = SREG;
    noInterrupts();
    const double
        gravity_x = 2 * (q.x * q.z - q.w * q.y),
        gravity_y = 2 * (q.w * q.x + q.y * q.z),
        // gravity_z = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
        gravity_z = 2 * (q.w * q.w + q.z * q.z) - 1;
    const double
        yaw_y_read = (q.x * q.y - q.w * q.z),
        yaw_x_read = (q.w * q.w + q.x * q.x);
    SREG = oldSREG;
    return Euler_angle(
        atan(gravity_y / sqrt(gravity_x * gravity_x + gravity_z * gravity_z)) * RAD_TO_DEG,
        atan(gravity_x / sqrt(gravity_y * gravity_y + gravity_z * gravity_z)) * RAD_TO_DEG,
        atan2(2 * yaw_y_read, 2 * yaw_x_read - 1) * RAD_TO_DEG
    );
}

void yh::rec::Mpu_6050::enable_ext_i2c_slave_sensors () {
    // load the old value of register 0x37
    Wire.beginTransmission(i2c_address);
    Wire.write(static_cast<uint8_t>(0x37));
    Wire.endTransmission();
    if (Wire.requestFrom(i2c_address, static_cast<uint8_t>(1U)) == static_cast<uint8_t>(1U)) {
        const uint8_t old_val = Wire.read();
        // write new value to MPU-9250 at register 0x37 with BYPASS_EN bit set HIGH
        Wire.beginTransmission(i2c_address);
        Wire.write(static_cast<uint8_t>(0x37));
        Wire.write(static_cast<uint8_t>(old_val | 0x02)); // 0x02 is the BYPASS_EN bit mask
        Wire.endTransmission();
    }
}

#endif // #ifndef MPU_6050_CPP
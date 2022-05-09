#ifndef MPU_6500_CPP
#define MPU_6500_CPP __DATE__ ", " __TIME__

#include "Mpu_6500.h"

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

#if 0 // the following are moved to the header file, to maintain compatibility with the Usart_mspim library

void yh::rec::Mpu_6500::begin () {
    pinMode(cs_pin, OUTPUT); // prioritized line
    pinMode(SCK, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, INPUT);
    cs_pin_output_reg = portOutputRegister(digitalPinToPort(cs_pin));
    cs_pin_mask = digitalPinToBitMask(cs_pin);
    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH
    SPI.begin();
    delayMicroseconds(1); // 1 us == 1000 ns

    // burst write is supported, so we use burst write to configure register 6A - 6C.
    (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
    SPI.beginTransaction(SPI_general_settings); // talk to MPU-6500
    SPI.transfer(static_cast<uint8_t>(0x6A)); // accessing the register 6A - User Control
    SPI.transfer(static_cast<uint8_t>(0b00010000)); // setting I2C_IF_DIS bit to 1 to ensure the chip is in SPI mode
    // SPI.endTransaction();
    // SPI.beginTransaction(SPI_general_settings);
    // SPI.transfer(static_cast<uint8_t>(0x6B)); // accessing the register 6B - Power Management 1
    SPI.transfer(static_cast<uint8_t>(0b00000000)); // setting SLEEP bit to 0
    // SPI.endTransaction();
    // SPI.beginTransaction(SPI_general_settings);
    // SPI.transfer(static_cast<uint8_t>(0x6C)); // accessing the register 6C - Power Management 2
    SPI.transfer(static_cast<uint8_t>(0b00000000)); // enabling accel x y z, enabling gyro x y z
    SPI.endTransaction();
    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH

    delayMicroseconds(1); // 1 us == 1000 ns

    (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
    SPI.beginTransaction(SPI_general_settings); // talk to MPU-6500
    SPI.transfer(static_cast<uint8_t>(0x1B)); // accessing the register 1B - Gyroscope Configuration
    #if (GYRO_RANGE == 250)
    SPI.transfer(static_cast<uint8_t>(0b00000000)); // setting the gyro to scale +/- 250deg./s (0 of [0:3])
    #elif (GYRO_RANGE == 500)
    SPI.transfer(static_cast<uint8_t>(0b00001000)); // setting the gyro to scale +/- 500deg./s (1 of [0:3])
    #elif (GYRO_RANGE == 1000)
    SPI.transfer(static_cast<uint8_t>(0b00010000)); // setting the gyro to scale +/- 1000deg./s (2 of [0:3]) assume that the robot cannot rotate more than 2.5 circles per second
    #elif (GYRO_RANGE == 2000)
    SPI.transfer(static_cast<uint8_t>(0b00011000)); // setting the gyro to scale +/- 2000deg./s (3 of [0:3]) to minimize chance of error when the referee lifts up the robot during match
    #endif
    SPI.endTransaction();
    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH

    prev_micros_reading = micros();
}

uint8_t yh::rec::Mpu_6500::who_am_i () {
    (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
    SPI.beginTransaction(SPI_general_settings); // talk to MPU-6500
    SPI.transfer(MPU_6500_READ_BIT | static_cast<uint8_t>(0x75)); // accessing the who am I register
    const uint8_t who_am_i_value = SPI.transfer(0x00);
    SPI.endTransaction();
    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH
}

double yh::rec::Mpu_6500::get_temp () {
    (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
    SPI.beginTransaction(SPI_read_sensor_reg_settings); // talk to MPU-6500
    SPI.transfer(MPU_6500_READ_BIT | static_cast<uint8_t>(0x41)); // accessing the register of thermometer, which has 2 bytes, from 0x41 to 0x42
    const int16_t TEMP_OUT = ( (static_cast<int16_t>(SPI.transfer(0x00)) << 8) | SPI.transfer(0x00) );
    SPI.endTransaction();
    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH
    // for MPU-6500 or MPU-9250 only: (not applicable to MPU-6050)
    return (TEMP_OUT - 21/*room_temp_offset*/) / 333.87/*sensitivity in LSB per degree Celsius */ + 21.0; // this formula is from register map
}

void yh::rec::Mpu_6500::cal_gyro (const uint32_t sampling_amount, void (*updating_function)(void)) {
    corr_roll  = 0;
    corr_pitch = 0;
    corr_yaw   = 0;
    for (uint32_t i = 0; i < sampling_amount; i++) {
        (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
        SPI.beginTransaction(SPI_read_sensor_reg_settings); // talk to MPU-6500
        SPI.transfer(MPU_6500_READ_BIT | static_cast<uint8_t>(0x43)); // accessing the registers of gyroscope x, y, z, where each axis has 2 bytes, from 0x43 to 0x48
        corr_roll  += static_cast<double>( (static_cast<int16_t>(SPI.transfer(0x00)) << 8) | SPI.transfer(0x00) );
        corr_pitch += static_cast<double>( (static_cast<int16_t>(SPI.transfer(0x00)) << 8) | SPI.transfer(0x00) );
        corr_yaw   += static_cast<double>( (static_cast<int16_t>(SPI.transfer(0x00)) << 8) | SPI.transfer(0x00) );
        SPI.endTransaction();
        (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH
        if (updating_function)
            updating_function();
        delayMicroseconds(4);
    }
    corr_roll  /= static_cast<double>(sampling_amount);
    corr_pitch /= static_cast<double>(sampling_amount);
    corr_yaw   /= static_cast<double>(sampling_amount);
}

void yh::rec::Mpu_6500::reset_gyro () {
    q.w = 1;
    q.x = 0;
    q.y = 0;
    q.z = 0;
}

void yh::rec::Mpu_6500::update_gyro () {
    const Gyro_packet d_gyro = update_gyro_isr();

    #if (GYRO_RANGE == 250)
    // for sensitivity == +- 250 degree per sec.
    d_roll  = (d_gyro.d_roll_raw  - corr_roll ) / 131000000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    d_pitch = (d_gyro.d_pitch_raw - corr_pitch) / 131000000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    d_yaw   = (d_gyro.d_yaw_raw   - corr_yaw  ) / 131000000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    #elif (GYRO_RANGE == 500)
    // for sensitivity == +- 500 degree per sec.
    d_roll  = (d_gyro.d_roll_raw  - corr_roll ) / 65500000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    d_pitch = (d_gyro.d_pitch_raw - corr_pitch) / 65500000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    d_yaw   = (d_gyro.d_yaw_raw   - corr_yaw  ) / 65500000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    #elif (GYRO_RANGE == 1000)
    // for sensitivity == +- 1000 degree per sec.
    d_roll  = (d_gyro.d_roll_raw  - corr_roll ) / 32800000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    d_pitch = (d_gyro.d_pitch_raw - corr_pitch) / 32800000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    d_yaw   = (d_gyro.d_yaw_raw   - corr_yaw  ) / 32800000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    #elif (GYRO_RANGE == 2000)
    // for sensitivity == +- 2000 degree per sec.
    d_roll  = (d_gyro.d_roll_raw  - corr_roll ) / 16400000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    d_pitch = (d_gyro.d_pitch_raw - corr_pitch) / 16400000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    d_yaw   = (d_gyro.d_yaw_raw   - corr_yaw  ) / 16400000.0 * d_gyro.d_time; // angle change per sec * time past in secs
    #endif

    const double d_roll_rad_div_2  = PI / 360.0 * d_roll;
    const double d_pitch_rad_div_2 = PI / 360.0 * d_pitch;
    const double d_yaw_rad_div_2   = PI / 360.0 * d_yaw;
    // delta Quaternion
    // Quaternion_of_that_angle == cos(d_angle / 2) + (i or j or k)sin(d_angle / 2)
    const double
        cy = cos(d_yaw_rad_div_2),
        sy = sin(d_yaw_rad_div_2),
        cp = cos(d_pitch_rad_div_2),
        sp = sin(d_pitch_rad_div_2),
        cr = cos(d_roll_rad_div_2),
        sr = sin(d_roll_rad_div_2);

    // Given that: pow(e, ix) == cos(x) + i sin(x),
    //  (cos(y) + k sin(y))(cos(p) + j sin(p))(cos(r) + i sin(r))
    // == cos(y) * cos(p) * cos(r) + k sin(y) * j sin(p) * i sin(r)
    //  + cos(y) * cos(p) * i sin(r) + k sin(y) * j sin(p) * cos(r)
    //  + cos(y) * j sin(p) * cos(r) + k sin(y) * cos(p) * i sin(r)
    //  + k sin(y) * cos(p) * cos(r) + cos(y) * j sin(p) * i sin(r)

    // == cos(y) * cos(p) * cos(r) + kji sin(y) * sin(p) * sin(r)
    //  + i cos(y) * cos(p) * sin(r) + kj sin(y) * sin(p) * cos(r)
    //  + j cos(y) * sin(p) * cos(r) + ki sin(y) * cos(p) * sin(r)
    //  + k sin(y) * cos(p) * cos(r) + ji cos(y) * sin(p) * sin(r)

    // == cos(y) * cos(p) * cos(r) + (+1) sin(y) * sin(p) * sin(r)
    //  + i cos(y) * cos(p) * sin(r) + (-i) sin(y) * sin(p) * cos(r)
    //  + j cos(y) * sin(p) * cos(r) + (j) sin(y) * cos(p) * sin(r)
    //  + k sin(y) * cos(p) * cos(r) + (-k) cos(y) * sin(p) * sin(r)

    // == cos(y) * cos(p) * cos(r) + sin(y) * sin(p) * sin(r)
    //  + i cos(y) * cos(p) * sin(r) - i sin(y) * sin(p) * cos(r)
    //  + j cos(y) * sin(p) * cos(r) + j sin(y) * cos(p) * sin(r)
    //  + k sin(y) * cos(p) * cos(r) - k cos(y) * sin(p) * sin(r)

    // == cos(y) * cos(p) * cos(r) + sin(y) * sin(p) * sin(r)
    //  + (cos(y) * cos(p) * sin(r) - sin(y) * sin(p) * cos(r))i
    //  + (cos(y) * sin(p) * cos(r) + sin(y) * cos(p) * sin(r))j
    //  + (sin(y) * cos(p) * cos(r) - cos(y) * sin(p) * sin(r))k
    q *= Quaternion
    (
        cy * cp * cr + sy * sp * sr,
        cy * cp * sr - sy * sp * cr,
        cy * sp * cr + sy * cp * sr,
        sy * cp * cr - cy * sp * sr  // this is the rotation expressed in quaternions
    );
    q.normalize();

}

double yh::rec::Mpu_6500::get_roll () {
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

double yh::rec::Mpu_6500::get_pitch () {
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

double yh::rec::Mpu_6500::get_yaw () {
    uint8_t oldSREG = SREG;
    noInterrupts();
    // yaw = atan2(2 * q.x * q.y - 2 * q.w * q.z, 2 * q.w * q.w + 2 * q.x * q.x - 1) * RAD_TO_DEG;
    const double yaw = atan2(2 * (q.x * q.y - q.w * q.z), 2 * (q.w * q.w + q.x * q.x) - 1) * RAD_TO_DEG;
    SREG = oldSREG;
    return yaw < 0 ? yaw + 360.0 : yaw;
}

yh::rec::Euler_angle yh::rec::Mpu_6500::get_euler_angles () {
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

#endif

#endif // #ifndef MPU_6500_CPP
#ifndef CUSTOM_GY521_H
#define CUSTOM_GY521_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <Wire.h>

// this class has been customized for RCJ soccer robots use only
class Custom_gy521 {
    private:
        //
    protected:
        // the 7-bit I2C address of the chip [0x00:0x7f]
        const uint8_t i2c_address;
    public:
        // accel features: // not published
        // using the equations of motions
        // 1st equation: s = ut + (1/2) a t^2
        // 2nd equation: v = u + at
        // // the instantaneous displacement
        // double sx, sy, sz;
        // // the instantaneous velocity
        // double vx, vy, vz;
        // // the correction added to every reading from the chip
        // double corr_ax, corr_ay, az;

        // gyro features:
        // the current value of the angles [0:359.99999999999999999999]
        volatile double roll, pitch, yaw;
        // the correction added to every reading from the chip
        volatile double corr_roll, corr_pitch, corr_yaw;
        // inits the 7-bit I2C address of the chip to init_i2c_address
        Custom_gy521 (const uint8_t init_i2c_address);
        // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
        // calls pinMode function and config the pin modes
        void begin ();

        // gets the who_am_i value to clarify whether this chip is mpu6050, mpu6500, or mpu9250
        // if the chip is mpu6000 or mpu6050, the returned value should be 0x68
        // if the chip is mpu6500, the returned value should be 0x70
        // if the chip is mpu9250, the returned value should be 0x71
        uint8_t who_am_i ();

        // calibrates the gyroscope, and give the corrections to corr_roll, corr_pitch, corr_yaw respectively
        // ATTENTION: WHEN GYROSCOPE CALIBRATION IS IN PROGRESS, PUT THE CHIP ON A FLAT SURFACE,
        // HOLD STILL, UNTIL CALIBRATION FUNCTION HAS RETURNED
        void cal_gyro (const uint16_t sampling_amount = 8192);
        // gets 6 bytes from gyro (uses calibrated data to correct)
        void update_gyro ();

        // calibrates roll, and give the corrections to corr_roll
        // ATTENTION: WHEN GYROSCOPE CALIBRATION IS IN PROGRESS, PUT THE CHIP ON A FLAT SURFACE,
        // HOLD STILL, UNTIL CALIBRATION FUNCTION HAS EXITED
        double cal_roll (const uint16_t sampling_amount = 8192);
        // gets 2 bytes from roll (uses calibrated data to correct)
        double update_roll ();
        // calibrates pitch, and give the corrections to corr_pitch
        // ATTENTION: WHEN GYROSCOPE CALIBRATION IS IN PROGRESS, PUT THE CHIP ON A FLAT SURFACE,
        // HOLD STILL, UNTIL CALIBRATION FUNCTION HAS EXITED
        double cal_pitch (const uint16_t sampling_amount = 8192);
        // gets 2 bytes from pitch (uses calibrated data to correct)
        double update_pitch ();
        // calibrates yaw, and give the corrections to corr_yaw
        // ATTENTION: WHEN GYROSCOPE CALIBRATION IS IN PROGRESS, PUT THE CHIP ON A FLAT SURFACE,
        // HOLD STILL, UNTIL CALIBRATION FUNCTION HAS EXITED
        double cal_yaw (const uint16_t sampling_amount = 8192);
        // gets 2 bytes from yaw (uses calibrated data to correct)
        double update_yaw ();
};

#endif // #ifndef CUSTOM_GY521_H
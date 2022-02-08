#ifndef CUSTOM_GY521_H
#define CUSTOM_GY521_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <Wire.h>

// this class has been customized for RCJ soccer robots use only
class Custom_gy521 {
    private:
        // get myself
        Custom_gy521 &myself () { return *this; }
    protected:
        // the 7-bit I2C address of the chip [0x00:0x7f]
        const uint8_t i2c_address;
        // the current value of the angles [0:359.99999999999999999999]
        double roll, pitch, yaw;
    public:
        // the correction added to every reading from the chip
        double corr_roll, corr_pitch, corr_yaw;
        // inits the 7-bit I2C address of the chip to init_i2c_address
        Custom_gy521 (const uint8_t init_i2c_address);
        // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
        // calls pinMode function and config the pin modes
        void begin ();

        // gets the who_am_i value to clarify whether this chip is mpu6050, mpu6500, or mpu9250
        // if the chip is mpu6050, the returned value should be 0x68
        // if the chip is mpu6500, the returned value should be 0x70
        // if the chip is mpu9250, the returned value should be 0x71
        uint8_t who_am_I ();

        // sets all gyro to 0
        void reset_gyro ();
        // calibrates the gyroscope, and give the corrections to corr_roll, corr_pitch, corr_yaw respectively
        // ATTENTION: WHEN GYROSCOPE CALIBRATION IS IN PROGRESS, PUT THE CHIP ON A STEADY SURFACE,
        // HOLD STILL, UNTIL CALIBRATION FUNCTION HAS RETURNED
        void cal_gyro (const uint16_t sampling_amount = 8192);
        // gets 6 bytes from gyro (uses calibrated data to correct)
        void update_gyro ();
        // gets 6 bytes from gyro
        void update_gyro (const double corr_x, const double corr_y, const double corr_z);

        // sets roll to 0
        void reset_roll ();
        // calibrates roll, and give the corrections to corr_roll
        // ATTENTION: WHEN GYROSCOPE CALIBRATION IS IN PROGRESS, PUT THE CHIP ON A STEADY SURFACE,
        // HOLD STILL, UNTIL CALIBRATION FUNCTION HAS RETURNED
        double cal_roll (const uint16_t sampling_amount = 8192);
        // gets 2 bytes from roll (uses calibrated data to correct)
        double update_roll ();
        // gets 2 bytes from roll
        double update_roll (const double corr_x);
        // returns not-updated roll
        double get_roll ();

        // sets pitch to 0
        void reset_pitch ();
        // calibrates pitch, and give the corrections to corr_pitch
        // ATTENTION: WHEN GYROSCOPE CALIBRATION IS IN PROGRESS, PUT THE CHIP ON A STEADY SURFACE,
        // HOLD STILL, UNTIL CALIBRATION FUNCTION HAS RETURNED
        double cal_pitch (const uint16_t sampling_amount = 8192);
        // gets 2 bytes from pitch (uses calibrated data to correct)
        double update_pitch ();
        // gets 2 bytes from pitch
        double update_pitch (const double corr_y);
        // returns not-updated pitch
        double get_pitch ();

        // sets yaw to 0
        void reset_yaw ();
        // calibrates yaw, and give the corrections to corr_yaw
        // ATTENTION: WHEN GYROSCOPE CALIBRATION IS IN PROGRESS, PUT THE CHIP ON A STEADY SURFACE,
        // HOLD STILL, UNTIL CALIBRATION FUNCTION HAS RETURNED
        double cal_yaw (const uint16_t sampling_amount = 8192);
        // gets 2 bytes from yaw (uses calibrated data to correct)
        double update_yaw ();
        // gets 2 bytes from yaw
        double update_yaw (const double corr_z);
        // returns not-updated yaw
        double get_yaw ();
};

#endif // #ifndef CUSTOM_GY521_H
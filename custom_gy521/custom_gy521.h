// for your reference only

// MPU-60X0 (for MPU-6050)
    // datasheet
    // https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
    // register map
    // https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf

// MPU-6500
    // datasheet
    // http://3cfeqx1hf82y3xcoull08ihx-wpengine.netdna-ssl.com/wp-content/uploads/2020/06/PS-MPU-6500A-01-v1.3.pdf
    // register map
    // https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6500-Register-Map2.pdf

// MPU-9250
    // datasheet
    // https://3cfeqx1hf82y3xcoull08ihx-wpengine.netdna-ssl.com/wp-content/uploads/2015/02/PS-MPU-9250A-01-v1.1.pdf
    // register map
    // https://3cfeqx1hf82y3xcoull08ihx-wpengine.netdna-ssl.com/wp-content/uploads/2017/11/RM-MPU-9250A-00-v1.6.pdf
    // AK8963 datasheet and register map
    // https://cdn.datasheetspdf.com/pdf-down/A/K/8/AK8963-AsahiKaseiMicrosystems.pdf



#ifndef CUSTOM_GY521_H
#define CUSTOM_GY521_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <Wire.h>

#define MPU6000 0x68
#define MPU6050 0x68
#define MPU6500 0x70
#define MPU9250 0x71

class Quaternion {
    public:
        double w;
        double x;
        double y;
        double z;

        Quaternion () : w(1.0), x(0.0), y(0.0), z(0.0) { }

        Quaternion (double nw, double nx, double ny, double nz) : w(nw), x(nx), y(ny), z(nz) { }

        Quaternion &operator = (const Quaternion rhs);

        Quaternion &operator *= (const Quaternion rhs);

        double vectors_squared (); // call me to check if bias from 1 is inacceptable

        double getMagnitude ();

        void normalize ();
};

Quaternion operator * (const Quaternion lhs, const Quaternion rhs);

// Un-comment line 69 below to use roll and pitch (disabled by default to save execute time).
// Do not just define ALL_AXES_ROTATION in your *.ino sketch,
// where the roll and pitch member variables are only declared,
// but the values are not computed in the implementation file.
// What I mean is, if you really want to use roll and pitch,
// un-comment line 69 below, which is to edit the library's header file.
// #define ALL_AXES_ROTATION

// this class has been customized for RCJ soccer robots use only
class Custom_gy521 {
    private:
        //
    protected:
        // the 7-bit I2C address of the chip [0x00:0x7f]
        const uint8_t i2c_address;
        // the quaternion of the current status
        Quaternion q;
        // the time of the previous reading in micros
        unsigned long prev_micros_reading;
    public:

        // gyro features:
        // the current value of the angles [0:359.99999999999999999999]
        volatile double
        #ifdef ALL_AXES_ROTATION
        roll, pitch,
        #endif // #ifdef ALL_AXES_ROTATION
        yaw;
        // the linear difference of roll, pitch, yaw between the latest and the previous measurement
        volatile double d_roll, d_pitch, d_yaw;
        // the correction added to every reading from the chip
        double corr_roll, corr_pitch, corr_yaw;
        // inits the 7-bit I2C address of the chip to init_i2c_address
        Custom_gy521 (const uint8_t init_i2c_address);
        // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
        // configures the settings of the I2C bus and the chip
        virtual void begin ();

        // gets the who_am_i value to clarify whether this chip is mpu6050, mpu6500, or mpu9250
        // if the chip is mpu6000 or mpu6050, the returned value should be 0x68
        // if the chip is mpu6500, the returned value should be 0x70
        // if the chip is mpu9250, the returned value should be 0x71
        uint8_t who_am_i ();

        // gets 6 bytes from accelerometer
        // void update_accel ();

        // gets temperature in degree Celsius
        double update_temp ();

        // calibrates the gyroscope, and give the corrections to corr_roll, corr_pitch, corr_yaw respectively
        // ATTENTION: WHEN GYROSCOPE CALIBRATION IS IN PROGRESS, PUT THE CHIP ON A FLAT SURFACE,
        // HOLD STILL, UNTIL CALIBRATION FUNCTION HAS RETURNED
        void cal_gyro (const uint32_t sampling_amount = 8192, void (*updating_function)(void) = NULL);
        // resets the roll, pitch and yaw values
        void reset_gyro ();
        // gets 6 bytes from gyroscope (uses calibrated data to correct)
        // this function consumes 2000 - 2150 microseconds in FAST I2C MODE
        void update_gyro ();

        // specially made for the external AK8963 magnetometer attached to MPU-9250
        // but this is also applicable to MPU-6050 chip, so I put this method over here instead of inside Custom_gy9250
        void enable_ext_i2c_slave_sensors ();
};

#endif // #ifndef CUSTOM_GY521_H
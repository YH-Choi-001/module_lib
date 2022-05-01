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



#ifndef MPU_6500_H
#define MPU_6500_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <SPI.h>

#define MPU6000 0x68
#define MPU6050 0x68
#define MPU6500 0x70
#define MPU9250 0x71


namespace yh {
    namespace rec {

        class Euler_angle;
        class Quaternion;

        class Euler_angle {
            public:
                // angles
                double
                    roll,
                    pitch,
                    yaw;

                // default constructor
                Euler_angle () : roll(0.0), pitch(0.0), yaw(0.0) { }

                Euler_angle (const Euler_angle &init) : roll(init.roll), pitch(init.pitch), yaw(init.yaw) { }

                Euler_angle (const double r, const double p, const double y) : roll(r), pitch(p), yaw(y) { }

                inline void operator = (const Euler_angle rhs) {
                    this->roll  = rhs.roll;
                    this->pitch = rhs.pitch;
                    this->yaw   = rhs.yaw;
                }

        };

        class Quaternion {
            public:
                // vectors
                double
                    w,
                    x,
                    y,
                    z;

                // default constructor
                Quaternion () : w(1.0), x(0.0), y(0.0), z(0.0) { }

                Quaternion (const Quaternion &init) : w(init.w), x(init.x), y(init.y), z(init.z) { }

                Quaternion (const double nw, const double nx, const double ny, const double nz) : w(nw), x(nx), y(ny), z(nz) { }

                Quaternion (const double roll, const double pitch, const double yaw) {
                    const double d_roll_rad_div_2  = PI / 360.0 * roll;
                    const double d_pitch_rad_div_2 = PI / 360.0 * pitch;
                    const double d_yaw_rad_div_2   = PI / 360.0 * yaw;
                    // delta Quaternion
                    // a little note that sin(pi - theta) == sin(theta), and cos(-theta) == cos(theta)
                    const double
                        cy = cos(d_yaw_rad_div_2),
                        sy = sin(d_yaw_rad_div_2),
                        cp = cos(d_pitch_rad_div_2),
                        sp = sin(d_pitch_rad_div_2),
                        cr = cos(d_roll_rad_div_2),
                        sr = sin(d_roll_rad_div_2);
                    w = cr * cp * cy + sr * sp * sy;
                    x = sr * cp * cy - cr * sp * sy;
                    y = cr * sp * cy + sr * cp * sy;
                    z = cr * cp * sy - sr * sp * cy;
                }

                inline void operator = (const Quaternion rhs) {
                    this->w = rhs.w;
                    this->x = rhs.x;
                    this->y = rhs.y;
                    this->z = rhs.z;
                }

                inline void operator *= (const Quaternion rhs) {
                    const double
                        product_w = (w * rhs.w   -   x * rhs.x   -   y * rhs.y   -   z * rhs.z),
                        product_x = (w * rhs.x   +   x * rhs.w   +   y * rhs.z   -   z * rhs.y),
                        product_y = (w * rhs.y   -   x * rhs.z   +   y * rhs.w   +   z * rhs.x),
                        product_z = (w * rhs.z   +   x * rhs.y   -   y * rhs.x   +   z * rhs.w);
                    this->w = product_w;
                    this->x = product_x;
                    this->y = product_y;
                    this->z = product_z;
                }

                // call me to check if bias from 1 is inacceptable
                inline double vectors_squared () { return (w * w + x * x + y * y + z * z); }

                inline void normalize () {
                    const double m = sqrt(this->vectors_squared());
                    w /= m;
                    x /= m;
                    y /= m;
                    z /= m;
                }
        };

        Quaternion operator * (const Quaternion lhs, const Quaternion rhs);

        struct Gyro_packet {
            int16_t
                d_roll_raw,
                d_pitch_raw,
                d_yaw_raw;
            unsigned long
                d_time;
        };

        #define MPU_6500_READ_BIT (0x80)
        // this class has been customized for RCJ soccer robots use only
        class Mpu_6500 {
            private:
                //
            protected:
                // pins that cannot be changed:
                // CS pin of MPU-6500
                const uint8_t cs_pin;
                volatile uint8_t *cs_pin_output_reg;
                uint8_t cs_pin_mask;
                // the quaternion of the current status
                Quaternion q;
                // the time of the previous reading in micros
                unsigned long prev_micros_reading;
                // general SPI settings for accessing any registers in the chip
                const SPISettings SPI_general_settings;
                // specialized (quicker) SPI settings for reading sensor and interrupt registers only
                const SPISettings SPI_read_sensor_reg_settings;
            public:
                // gyro features:
                // the linear difference of roll, pitch, yaw between the latest and the previous measurement
                volatile double d_roll, d_pitch, d_yaw;
                // the correction added to every reading from the chip
                double corr_roll, corr_pitch, corr_yaw;
                // inits the digital pin connected to the nCS pin of the MPU-6500
                Mpu_6500 (const uint8_t init_cs_pin);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // configures the settings of the SPI bus and the chip
                virtual void begin ();

                // gets the who_am_i value to clarify whether this chip is mpu6050, mpu6500, or mpu9250
                // if the chip is mpu6000 or mpu6050, the returned value should be 0x68
                // if the chip is mpu6500, the returned value should be 0x70
                // if the chip is mpu9250, the returned value should be 0x71
                uint8_t who_am_i ();

                // gets 6 bytes from accelerometer
                // void update_accel ();

                // gets temperature in degree Celsius
                double get_temp ();

                // calibrates the gyroscope, and give the corrections to corr_roll, corr_pitch, corr_yaw respectively
                // ATTENTION: WHEN GYROSCOPE CALIBRATION IS IN PROGRESS, PUT THE CHIP ON A FLAT SURFACE,
                // HOLD STILL, UNTIL CALIBRATION FUNCTION HAS RETURNED
                void cal_gyro (const uint32_t sampling_amount = 8192, void (*updating_function)(void) = NULL);
                // resets the roll, pitch and yaw values
                void reset_gyro ();
                // the isr that is optimized to be very short
                const Gyro_packet update_gyro_isr () __attribute__((__always_inline__)) {
                    Gyro_packet temp;
                    temp.d_time = micros() - prev_micros_reading;
                    (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
                    SPI.beginTransaction(SPI_read_sensor_reg_settings); // talk to MPU-6500
                    SPI.transfer(MPU_6500_READ_BIT | static_cast<uint8_t>(0x43)); // accessing the registers of gyroscope x, y, z, where each axis has 2 bytes, from 0x43 to 0x48
                    temp.d_roll_raw  = ((static_cast<int16_t>(SPI.transfer(0x00)) << 8) | SPI.transfer(0x00));
                    temp.d_pitch_raw = ((static_cast<int16_t>(SPI.transfer(0x00)) << 8) | SPI.transfer(0x00));
                    temp.d_yaw_raw   = ((static_cast<int16_t>(SPI.transfer(0x00)) << 8) | SPI.transfer(0x00));
                    SPI.endTransaction();
                    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH
                    prev_micros_reading += temp.d_time;
                    return temp;
                }
                // gets 6 bytes from gyroscope (uses calibrated data to correct)
                // this function consumes 2000 - 2150 microseconds in FAST I2C MODE
                void update_gyro ();

                // gets roll of Euler angles
                double get_roll ();
                // gets pitch of Euler angles
                double get_pitch ();
                // gets yaw of Euler angles
                double get_yaw ();
                // gets all 3 axes of rotation of Euler angles
                Euler_angle get_euler_angles ();

                // Specially made for the external AK8963 magnetometer attached to MPU-9250,
                // but this is also applicable to MPU-6050 chip, so I put this method over here instead of inside Custom_gy9250.
                // This method is removed from this SPI version of MPU-6500 library,
                // because we are here using SPI to communicate with MPU-6500,
                // so there is no need for us to bypass the MPU-6500 chip to access the other slaves through I2C.
                // void enable_ext_i2c_slave_sensors ();
        };
    }
}

#endif // #ifndef MPU_6500_H
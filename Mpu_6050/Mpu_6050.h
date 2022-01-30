#ifndef MPU_6050_H
#define MPU_6050_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <Wire.h>

namespace yh {
    namespace rec {
        class Mpu_6050 {
            private:
                //
            protected:
                const uint8_t i2c_address; // either 0x68 when AD0 is low, or 0x69 when AD0 is high
                void write_i2c (const uint8_t target_i2c_addr, const uint8_t register_no, const uint8_t data);
                void read_i2c (const uint8_t target_i2c_addr, const uint8_t register_no, const uint8_t len, uint8_t *byte_array);
                // accelerometers
                uint8_t accel_range; // sensitivity, available from 0 to 3
                double accel_LSB_sensitivity; // stores the denominator after getting accel raw value
                // gyroscopes
                uint8_t gyro_range; // sensitivity, available from 0 to 3
                double gyro_LSB_sensitivity; // stores the denominator after getting gyro raw value
            // protected:
            public:
                // accelerometers
                // measures linear acceleration without dividing the denominator
                uint16_t
                    raw_accel_x,
                    raw_accel_y,
                    raw_accel_z;
                // measures linear acceleration, unit is 9.81 m s^-2
                double
                    accel_x,
                    accel_y,
                    accel_z;
                // thermometers
                uint16_t temp;   // measures temperature of the surrounding, unit is degree Celsius
                // gyroscopes
                // measures rotational acceleration without dividing the denominator
                uint16_t
                    raw_gyro_x,
                    raw_gyro_y,
                    raw_gyro_z;
                // measures rotational acceleration, unit is degree / second
                double
                    gyro_x,
                    gyro_y,
                    gyro_z;
            public:
                // measures the correction added per reading
                double
                    accel_x_corr,
                    accel_y_corr,
                    accel_z_corr;
                // measures the correction added per reading
                double
                    gyro_x_corr,
                    gyro_y_corr,
                    gyro_z_corr;
                // inits the argument to the i2c address of this object
                Mpu_6050 (const uint8_t init_i2c_address);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                inline void begin ();
                // accel
                // sets the sensitivity of accelerometer [0:3]
                void set_accel_range (const uint8_t set_accel_range); // sensitivity
                // get the unprocessed accelerometer values by reading the registers in the chip
                uint16_t
                    get_raw_accel_x (),
                    get_raw_accel_y (),
                    get_raw_accel_z ();
                // get the unprocessed accelerometer values by reading the registers in the chip
                uint64_t
                    get_all_raw_accel ();
                // get the processed accelerometer values by dividing the raw values with the LSB_sensitivity
                double
                    get_accel_x (),
                    get_accel_y (),
                    get_accel_z ();
                // // calibrate the accelerometers x, y, z
                // double
                //     cal_accel_x (),
                //     cal_accel_y (),
                //     cal_accel_z ();
                // temp
                uint16_t get_temp ();
                // gyro
                // sets the sensitivity of gyroscope [0:3]
                void set_gyro_range (const uint8_t set_gyro_range); // sensitivity
                // get the unprocessed gyroscope values by reading the registers in the chip
                uint16_t
                    get_raw_gyro_x (),
                    get_raw_gyro_y (),
                    get_raw_gyro_z ();
                // get the unprocessed gyroscope values by reading the registers in the chip
                uint64_t get_all_raw_gyro ();
                // get the processed gyroscope values by dividing the raw values with the LSB_sensitivity
                double
                    get_gyro_x (),
                    get_gyro_y (),
                    get_gyro_z ();
                // // calibrate the gyroscopes x, y, z
                // double
                //     cal_gyro_x (),
                //     cal_gyro_y (),
                //     cal_gyro_z ();
                // all
                void get_raw_accel_temp_gyro ();
        };
    }
}

#endif //#ifndef MPU_6050_H
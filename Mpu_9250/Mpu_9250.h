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



#ifndef MPU_9250_H
#define MPU_9250_H __DATE__ ", " __TIME__

#include "../Mpu_6050/Mpu_6050.h"

namespace yh {
    namespace rec {
        // this class has been customized for RCJ soccer robots use only
        class Ak_8963 {
            private:
                //
            protected:
                // the 7-bit I2C address of the chip [0x00:0x7f]
                const uint8_t i2c_address;
                // sensitivity adjustment value of the axes
                // the correction ratio multiplied to every reading from the chip
                uint8_t ASA_X, ASA_Y, ASA_Z;
                // maximum strength of electromagnetic field recorded at this place
                int16_t max_x, max_y, max_z;
                // the current value of the raw magnetic field strength
                // note: must be a signed 16-bit integer
                int16_t raw_x, raw_y, raw_z;
                // update raw x, y, z
                inline void update_raw ();
                // use this to do polling
                // inline void polling_update_raw ();
            public:
                // minimum strength of electromagnetic field recorded at this place
                int16_t min_x, min_y, min_z;
                // range of strength of electromagnetic field recorded at this place
                double range_x, range_y, range_z;
                // the return-to-zero heading
                double rz_heading;
                // inits the 7-bit I2C address of the chip to init_i2c_address
                Ak_8963 (const uint8_t init_i2c_address);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // configures the settings of the I2C bus and the chip
                void begin ();

                // gets the WIA value to clarify whether this chip is ak8963
                // if the chip is ak8963, the returned value should be 0x48
                uint8_t who_i_am ();

                // calibrates the magnetometer, and give the mean values to x_mean, y_mean, z_mean for compass usage
                // ATTENTION: WHEN MAGNETOMETER CALIBRATION IS IN PROGRESS, SWING THE CHIP IN ALL DIRECTIONS OF X, Y AND Z.
                void single_calibrate ();

                // sets the current heading to 0
                void reset_heading ();

                // get heading of the chip
                double get_heading ();
        };
        // this class has been customized for RCJ soccer robots use only
        class Mpu_9250 : public Mpu_6050 {
            private:
                //
            protected:
                //
            public:
                // the AK-8963 magnetometer object inside the MPU-9250 chip
                Ak_8963 mag;
                //
                Mpu_9250 (const uint8_t init_i2c_address);
        };

    }
}

#endif // #ifndef MPU_9250_H
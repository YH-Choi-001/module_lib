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



#ifndef CUSTOM_GY9250_H
#define CUSTOM_GY9250_H __DATE__ ", " __TIME__

#include "../custom_gy521/custom_gy521.h"

// this class has been customized for RCJ soccer robots use only
class Custom_ak8963 {
    private:
        //
    protected:
        // the 7-bit I2C address of the chip [0x00:0x7f]
        const uint8_t i2c_address;
        // the correction ratio multiplied to every reading from the chip
        double ASA_X, ASA_Y, ASA_Z;
    public:
        // mag features:
        // the current value of the raw magnetic field strength
        // note: must be a signed 16-bit integer
        volatile int16_t raw_x, raw_y, raw_z;
        // the current value of the adjusted magnetic field strength
        volatile double adj_x, adj_y, adj_z;
        // inits the 7-bit I2C address of the chip to init_i2c_address
        Custom_ak8963 (const uint8_t init_i2c_address);
        // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
        // calls pinMode function and config the pin modes
        virtual void begin ();

        // gets the WIA value to clarify whether this chip is ak8963
        // if the chip is ak8963, the returned value should be 0x48
        uint8_t who_i_am ();

        // update raw x, y, z
        void update_raw ();

        // update adjusted x, y, z
        void update_adjusted ();
};

// this class has been customized for RCJ soccer robots use only
class Custom_gy9250 : public Custom_gy521 {
    private:
        //
    protected:
        //
    public:
        //
        Custom_ak8963 mag;
        //
        Custom_gy9250 (const uint8_t init_i2c_address);
};

#endif // #ifndef CUSTOM_GY9250_H
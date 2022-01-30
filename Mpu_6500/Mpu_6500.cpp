#ifndef MPU_6500_CPP
#define MPU_6500_CPP __DATE__ ", " __TIME__

#include "Mpu_6500.h"

#include "../Mpu_6050/Mpu_6050.cpp"

yh::rec::Mpu_6500::Mpu_6500 (const uint8_t init_i2c_address) :
    Mpu_6050(init_i2c_address)
{
    //
}

#endif // #ifndef MPU_6500_CPP
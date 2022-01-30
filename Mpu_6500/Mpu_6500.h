#ifndef MPU_6500_H
#define MPU_6500_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <Wire.h>

#include "../Mpu_6050/Mpu_6050.h"

namespace yh {
    namespace rec {
        class Mpu_6500 : public Mpu_6050 {
            private:
                //
            protected:
                //
            public:
                Mpu_6500 (const uint8_t init_i2c_address);
        };
    }
}

#endif // #ifndef MPU_6500_H
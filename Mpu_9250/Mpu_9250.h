#ifndef MPU_9250_H
#define MPU_9250_H __DATE__ ", " __TIME__

#include "../Gy_521/Gy_521.h"

namespace yh {
    namespace rec {
        class Mpu_9250 : protected Gy_521 {
            private:
                //
            protected:
                //
            public:
                Mpu_9250 (const uint8_t init_i2c_address);
        };
    }
}

#endif // #ifndef MPU_9250_H
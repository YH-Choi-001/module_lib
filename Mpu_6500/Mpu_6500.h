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

        // #define GYRO_RANGE 250
        // #define GYRO_RANGE 500
        // #define GYRO_RANGE 1000
        #define GYRO_RANGE 2000 // selects the largest range to minimize chance of error when the referee lifts up the robot during match

        #define MPU_6500_READ_BIT (0x80)
        // this class has been customized for RCJ soccer robots use only
        template <class SPI_class, class SPI_settings>
        class Mpu_6500 {
            private:
                //
            protected:
                // pins that cannot be changed:
                const uint8_t
                    sck_pin,
                    mosi_pin,
                    miso_pin,
                    cs_pin;
                volatile uint8_t *cs_pin_output_reg;
                uint8_t cs_pin_mask;
                // the quaternion of the current status
                Quaternion q;
                // the time of the previous reading in micros
                unsigned long prev_micros_reading;
                // SPI object
                SPI_class *spi_ptr;
                // general SPI settings for accessing any registers in the chip
                const SPI_settings SPI_general_settings;
                // specialized (quicker) SPI settings for reading sensor and interrupt registers only
                const SPI_settings SPI_read_sensor_reg_settings;
            public:
                // gyro features:
                // the linear difference of roll, pitch, yaw between the latest and the previous measurement
                volatile double d_roll, d_pitch, d_yaw;
                // the correction added to every reading from the chip
                double corr_roll, corr_pitch, corr_yaw;
                // inits the digital pin connected to the nCS pin of the MPU-6500
                Mpu_6500 (const uint8_t init_cs_pin) :
                    sck_pin(typeid(SPI_class) == typeid(SPIClass) ? SCK : 0),
                    mosi_pin(typeid(SPI_class) == typeid(SPIClass) ? MOSI : 0),
                    miso_pin(typeid(SPI_class) == typeid(SPIClass) ? MISO : 0),
                    cs_pin(init_cs_pin),
                    SPI_general_settings((((F_CPU / 2) > 1000000U) ? 1000000U : (F_CPU / 2)), MSBFIRST, SPI_MODE3), // according to datasheet, accessing all registers have a maximum of 1MHz clock rate
                    SPI_read_sensor_reg_settings((((F_CPU / 2) > 20000000U) ? 20000000U : (F_CPU / 2)), MSBFIRST, SPI_MODE3), // according to datasheet, reading sensor and interrupt registers have a maximum of 20MHz clock rate
                    d_roll(0),
                    d_pitch(0),
                    d_yaw(0),
                    corr_roll(0),
                    corr_pitch(0),
                    corr_yaw(0),
                    q(1, 0, 0, 0),
                    spi_ptr(typeid(SPI_class) == typeid(SPIClass) ? (&SPI) : ((SPI_class *)NULL))
                {
                    //
                }

                // inits the digital pin connected to the nCS pin of the MPU-6500
                Mpu_6500 (SPI_class *init_spi_ptr, const uint8_t init_sck_pin, const uint8_t init_mosi_pin, const uint8_t init_miso_pin, const uint8_t init_cs_pin) :
                    sck_pin(typeid(SPI_class) == typeid(SPIClass) ? SCK : init_sck_pin),
                    mosi_pin(typeid(SPI_class) == typeid(SPIClass) ? MOSI : init_mosi_pin),
                    miso_pin(typeid(SPI_class) == typeid(SPIClass) ? MISO : init_miso_pin),
                    cs_pin(init_cs_pin),
                    SPI_general_settings((((F_CPU / 2) > 1000000U) ? 1000000U : (F_CPU / 2)), MSBFIRST, SPI_MODE3), // according to datasheet, accessing all registers have a maximum of 1MHz clock rate
                    SPI_read_sensor_reg_settings((((F_CPU / 2) > 20000000U) ? 20000000U : (F_CPU / 2)), MSBFIRST, SPI_MODE3), // according to datasheet, reading sensor and interrupt registers have a maximum of 20MHz clock rate
                    d_roll(0),
                    d_pitch(0),
                    d_yaw(0),
                    corr_roll(0),
                    corr_pitch(0),
                    corr_yaw(0),
                    q(1, 0, 0, 0),
                    spi_ptr(init_spi_ptr)
                {
                    //
                }

                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // configures the settings of the SPI bus and the chip
                void begin () {
                    pinMode(cs_pin, OUTPUT); // prioritized line
                    pinMode(sck_pin, OUTPUT);
                    pinMode(mosi_pin, OUTPUT);
                    pinMode(miso_pin, INPUT);
                    cs_pin_output_reg = portOutputRegister(digitalPinToPort(cs_pin));
                    cs_pin_mask = digitalPinToBitMask(cs_pin);
                    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH
                    spi_ptr->begin();
                    delayMicroseconds(1); // 1 us == 1000 ns

                    // burst write is supported, so we use burst write to configure register 6A - 6C.
                    (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
                    spi_ptr->beginTransaction(SPI_general_settings); // talk to MPU-6500
                    spi_ptr->transfer(static_cast<uint8_t>(0x6A)); // accessing the register 6A - User Control
                    spi_ptr->transfer(static_cast<uint8_t>(0b00010000)); // setting I2C_IF_DIS bit to 1 to ensure the chip is in SPI mode
                    // spi_ptr->endTransaction();
                    // spi_ptr->beginTransaction(SPI_general_settings);
                    // spi_ptr->transfer(static_cast<uint8_t>(0x6B)); // accessing the register 6B - Power Management 1
                    spi_ptr->transfer(static_cast<uint8_t>(0b00000000)); // setting SLEEP bit to 0
                    // spi_ptr->endTransaction();
                    // spi_ptr->beginTransaction(SPI_general_settings);
                    // spi_ptr->transfer(static_cast<uint8_t>(0x6C)); // accessing the register 6C - Power Management 2
                    spi_ptr->transfer(static_cast<uint8_t>(0b00000000)); // enabling accel x y z, enabling gyro x y z
                    spi_ptr->endTransaction();
                    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH

                    delayMicroseconds(1); // 1 us == 1000 ns

                    (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
                    spi_ptr->beginTransaction(SPI_general_settings); // talk to MPU-6500
                    spi_ptr->transfer(static_cast<uint8_t>(0x1B)); // accessing the register 1B - Gyroscope Configuration
                    #if (GYRO_RANGE == 250)
                    spi_ptr->transfer(static_cast<uint8_t>(0b00000000)); // setting the gyro to scale +/- 250deg./s (0 of [0:3])
                    #elif (GYRO_RANGE == 500)
                    spi_ptr->transfer(static_cast<uint8_t>(0b00001000)); // setting the gyro to scale +/- 500deg./s (1 of [0:3])
                    #elif (GYRO_RANGE == 1000)
                    spi_ptr->transfer(static_cast<uint8_t>(0b00010000)); // setting the gyro to scale +/- 1000deg./s (2 of [0:3]) assume that the robot cannot rotate more than 2.5 circles per second
                    #elif (GYRO_RANGE == 2000)
                    spi_ptr->transfer(static_cast<uint8_t>(0b00011000)); // setting the gyro to scale +/- 2000deg./s (3 of [0:3]) to minimize chance of error when the referee lifts up the robot during match
                    #endif
                    spi_ptr->endTransaction();
                    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH

                    prev_micros_reading = micros();
                }

                // gets the who_am_i value to clarify whether this chip is mpu6050, mpu6500, or mpu9250
                // if the chip is mpu6000 or mpu6050, the returned value should be 0x68 (impossible, bc mpu60X0 does not have SPI interface)
                // if the chip is mpu6500, the returned value should be 0x70
                // if the chip is mpu9250, the returned value should be 0x71
                uint8_t who_am_i () {
                    (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
                    spi_ptr->beginTransaction(SPI_general_settings); // talk to MPU-6500
                    spi_ptr->transfer(MPU_6500_READ_BIT | static_cast<uint8_t>(0x75)); // accessing the who am I register
                    const uint8_t who_am_i_value = spi_ptr->transfer(0x00);
                    spi_ptr->endTransaction();
                    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH
                }

                // gets 6 bytes from accelerometer
                // void update_accel ();

                // gets temperature in degree Celsius
                double get_temp () {
                    (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
                    spi_ptr->beginTransaction(SPI_read_sensor_reg_settings); // talk to MPU-6500
                    spi_ptr->transfer(MPU_6500_READ_BIT | static_cast<uint8_t>(0x41)); // accessing the register of thermometer, which has 2 bytes, from 0x41 to 0x42
                    const int16_t TEMP_OUT = ( (static_cast<int16_t>(spi_ptr->transfer(0x00)) << 8) | spi_ptr->transfer(0x00) );
                    spi_ptr->endTransaction();
                    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH
                    // for MPU-6500 or MPU-9250 only: (not applicable to MPU-6050)
                    return (TEMP_OUT - 21/*room_temp_offset*/) / 333.87/*sensitivity in LSB per degree Celsius */ + 21.0; // this formula is from register map
                }

                // calibrates the gyroscope, and give the corrections to corr_roll, corr_pitch, corr_yaw respectively
                // ATTENTION: WHEN GYROSCOPE CALIBRATION IS IN PROGRESS, PUT THE CHIP ON A FLAT SURFACE,
                // HOLD STILL, UNTIL CALIBRATION FUNCTION HAS RETURNED
                void cal_gyro (const uint32_t sampling_amount = 8192, void (*updating_function)(void) = NULL) {
                    corr_roll  = 0;
                    corr_pitch = 0;
                    corr_yaw   = 0;
                    for (uint32_t i = 0; i < sampling_amount; i++) {
                        (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
                        spi_ptr->beginTransaction(SPI_read_sensor_reg_settings); // talk to MPU-6500
                        spi_ptr->transfer(MPU_6500_READ_BIT | static_cast<uint8_t>(0x43)); // accessing the registers of gyroscope x, y, z, where each axis has 2 bytes, from 0x43 to 0x48
                        corr_roll  += static_cast<double>( (static_cast<int16_t>(spi_ptr->transfer(0x00)) << 8) | spi_ptr->transfer(0x00) );
                        corr_pitch += static_cast<double>( (static_cast<int16_t>(spi_ptr->transfer(0x00)) << 8) | spi_ptr->transfer(0x00) );
                        corr_yaw   += static_cast<double>( (static_cast<int16_t>(spi_ptr->transfer(0x00)) << 8) | spi_ptr->transfer(0x00) );
                        spi_ptr->endTransaction();
                        (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH
                        if (updating_function)
                            updating_function();
                        delayMicroseconds(4);
                    }
                    corr_roll  /= static_cast<double>(sampling_amount);
                    corr_pitch /= static_cast<double>(sampling_amount);
                    corr_yaw   /= static_cast<double>(sampling_amount);
                }

                // resets the roll, pitch and yaw values
                void reset_gyro () { q.w = 1, q.x = 0, q.y = 0, q.z = 0; }

                // the isr that is optimized to be very short
                const Gyro_packet update_gyro_isr () __attribute__((__always_inline__)) {
                    Gyro_packet temp;
                    temp.d_time = micros() - prev_micros_reading;
                    (*cs_pin_output_reg) &= (~cs_pin_mask); // CS pin set to LOW
                    spi_ptr->beginTransaction(SPI_read_sensor_reg_settings); // talk to MPU-6500
                    spi_ptr->transfer(MPU_6500_READ_BIT | static_cast<uint8_t>(0x43)); // accessing the registers of gyroscope x, y, z, where each axis has 2 bytes, from 0x43 to 0x48
                    temp.d_roll_raw  = ((static_cast<int16_t>(spi_ptr->transfer(0x00)) << 8) | spi_ptr->transfer(0x00));
                    temp.d_pitch_raw = ((static_cast<int16_t>(spi_ptr->transfer(0x00)) << 8) | spi_ptr->transfer(0x00));
                    temp.d_yaw_raw   = ((static_cast<int16_t>(spi_ptr->transfer(0x00)) << 8) | spi_ptr->transfer(0x00));
                    spi_ptr->endTransaction();
                    (*cs_pin_output_reg) |= cs_pin_mask; // CS pin set to HIGH
                    prev_micros_reading += temp.d_time;
                    return temp;
                }

                // gets 6 bytes from gyroscope (uses calibrated data to correct)
                // this function consumes about 1300 microseconds in Arduino Mega 8MHz SPI in the simulator
                void update_gyro (const Gyro_packet d_gyro) {
                    #if (GYRO_RANGE == 250)
                    // for sensitivity == +- 250 degree per sec.
                    d_roll  = (d_gyro.d_roll_raw  - corr_roll ) / 131000000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    d_pitch = (d_gyro.d_pitch_raw - corr_pitch) / 131000000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    d_yaw   = (d_gyro.d_yaw_raw   - corr_yaw  ) / 131000000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    #elif (GYRO_RANGE == 500)
                    // for sensitivity == +- 500 degree per sec.
                    d_roll  = (d_gyro.d_roll_raw  - corr_roll ) / 65500000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    d_pitch = (d_gyro.d_pitch_raw - corr_pitch) / 65500000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    d_yaw   = (d_gyro.d_yaw_raw   - corr_yaw  ) / 65500000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    #elif (GYRO_RANGE == 1000)
                    // for sensitivity == +- 1000 degree per sec.
                    d_roll  = (d_gyro.d_roll_raw  - corr_roll ) / 32800000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    d_pitch = (d_gyro.d_pitch_raw - corr_pitch) / 32800000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    d_yaw   = (d_gyro.d_yaw_raw   - corr_yaw  ) / 32800000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    #elif (GYRO_RANGE == 2000)
                    // for sensitivity == +- 2000 degree per sec.
                    d_roll  = (d_gyro.d_roll_raw  - corr_roll ) / 16400000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    d_pitch = (d_gyro.d_pitch_raw - corr_pitch) / 16400000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    d_yaw   = (d_gyro.d_yaw_raw   - corr_yaw  ) / 16400000.0 * d_gyro.d_time; // angle change per sec * time past in secs
                    #endif

                    const double d_roll_rad_div_2  = PI / 360.0 * d_roll;
                    const double d_pitch_rad_div_2 = PI / 360.0 * d_pitch;
                    const double d_yaw_rad_div_2   = PI / 360.0 * d_yaw;
                    // delta Quaternion
                    // Quaternion_of_that_angle == cos(d_angle / 2) + (i or j or k)sin(d_angle / 2)
                    const double
                        cy = cos(d_yaw_rad_div_2),
                        sy = sin(d_yaw_rad_div_2),
                        cp = cos(d_pitch_rad_div_2),
                        sp = sin(d_pitch_rad_div_2),
                        cr = cos(d_roll_rad_div_2),
                        sr = sin(d_roll_rad_div_2);

                    // Given that: pow(e, ix) == cos(x) + i sin(x),
                    //  (cos(y) + k sin(y))(cos(p) + j sin(p))(cos(r) + i sin(r))
                    // == cos(y) * cos(p) * cos(r) + k sin(y) * j sin(p) * i sin(r)
                    //  + cos(y) * cos(p) * i sin(r) + k sin(y) * j sin(p) * cos(r)
                    //  + cos(y) * j sin(p) * cos(r) + k sin(y) * cos(p) * i sin(r)
                    //  + k sin(y) * cos(p) * cos(r) + cos(y) * j sin(p) * i sin(r)

                    // == cos(y) * cos(p) * cos(r) + kji sin(y) * sin(p) * sin(r)
                    //  + i cos(y) * cos(p) * sin(r) + kj sin(y) * sin(p) * cos(r)
                    //  + j cos(y) * sin(p) * cos(r) + ki sin(y) * cos(p) * sin(r)
                    //  + k sin(y) * cos(p) * cos(r) + ji cos(y) * sin(p) * sin(r)

                    // == cos(y) * cos(p) * cos(r) + (+1) sin(y) * sin(p) * sin(r)
                    //  + i cos(y) * cos(p) * sin(r) + (-i) sin(y) * sin(p) * cos(r)
                    //  + j cos(y) * sin(p) * cos(r) + (j) sin(y) * cos(p) * sin(r)
                    //  + k sin(y) * cos(p) * cos(r) + (-k) cos(y) * sin(p) * sin(r)

                    // == cos(y) * cos(p) * cos(r) + sin(y) * sin(p) * sin(r)
                    //  + i cos(y) * cos(p) * sin(r) - i sin(y) * sin(p) * cos(r)
                    //  + j cos(y) * sin(p) * cos(r) + j sin(y) * cos(p) * sin(r)
                    //  + k sin(y) * cos(p) * cos(r) - k cos(y) * sin(p) * sin(r)

                    // == cos(y) * cos(p) * cos(r) + sin(y) * sin(p) * sin(r)
                    //  + (cos(y) * cos(p) * sin(r) - sin(y) * sin(p) * cos(r))i
                    //  + (cos(y) * sin(p) * cos(r) + sin(y) * cos(p) * sin(r))j
                    //  + (sin(y) * cos(p) * cos(r) - cos(y) * sin(p) * sin(r))k
                    q *= Quaternion
                    (
                        cy * cp * cr + sy * sp * sr,
                        cy * cp * sr - sy * sp * cr,
                        cy * sp * cr + sy * cp * sr,
                        sy * cp * cr - cy * sp * sr  // this is the rotation expressed in quaternions
                    );
                    q.normalize();
                }

                // gets roll of Euler angles
                double get_roll () {
                    uint8_t oldSREG = SREG;
                    noInterrupts();
                    const double
                        gravity_x = 2 * (q.x * q.z - q.w * q.y),
                        gravity_y = 2 * (q.w * q.x + q.y * q.z),
                        // gravity_z = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
                        gravity_z = 2 * (q.w * q.w + q.z * q.z) - 1;
                    // since w^2 + x^2 + y^2 + z^2 = 1
                    //                   w^2 + z^2 = 1 - x^2 - y^2
                    //               w^2 + z^2 - 1 = - x^2 - y^2
                    // 2 * (q.w * q.w + q.z * q.z) - 1
                    SREG = oldSREG;
                    return atan(gravity_y / sqrt(gravity_x * gravity_x + gravity_z * gravity_z)) * RAD_TO_DEG;
                }

                // gets pitch of Euler angles
                double get_pitch () {
                    uint8_t oldSREG = SREG;
                    noInterrupts();
                    const double
                        gravity_x = 2 * (q.x * q.z - q.w * q.y),
                        gravity_y = 2 * (q.w * q.x + q.y * q.z),
                        // gravity_z = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
                        gravity_z = 2 * (q.w * q.w + q.z * q.z) - 1;
                    SREG = oldSREG;
                    return atan(gravity_x / sqrt(gravity_y * gravity_y + gravity_z * gravity_z)) * RAD_TO_DEG;
                }

                // gets yaw of Euler angles
                double get_yaw () {
                    uint8_t oldSREG = SREG;
                    noInterrupts();
                    // yaw = atan2(2 * q.x * q.y - 2 * q.w * q.z, 2 * q.w * q.w + 2 * q.x * q.x - 1) * RAD_TO_DEG;
                    const double yaw = atan2(2 * (q.x * q.y - q.w * q.z), 2 * (q.w * q.w + q.x * q.x) - 1) * RAD_TO_DEG;
                    SREG = oldSREG;
                    return yaw < 0 ? yaw + 360.0 : yaw;
                }

                // gets all 3 axes of rotation of Euler angles
                Euler_angle get_euler_angles () {
                    uint8_t oldSREG = SREG;
                    noInterrupts();
                    const double
                        gravity_x = 2 * (q.x * q.z - q.w * q.y), // 2 * (xi * zk - w * yj) == 2 * (-(xz)j - (wy)j) == -2 * (xz + wy)j
                        gravity_y = 2 * (q.w * q.x + q.y * q.z), // 2 * (w * xi + yj * zk) == 2 * ((wx)i + (yz)i) == 2 * (wx + yz)i
                        // gravity_z = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
                        gravity_z = 2 * (q.w * q.w + q.z * q.z) - 1; // 2 * (ww + zkzk) - 1 == 2 * (ww - zz) - 1
                    const double yaw = atan2(2 * (q.x * q.y - q.w * q.z), 2 * (q.w * q.w + q.x * q.x) - 1) * RAD_TO_DEG;
                        // yaw_y_read = 2 * (q.x * q.y - q.w * q.z), // 2 * (xi * yj - w * zk) == 2 * ((xy)k - (wz)k) == 2 * (xy - wz)k
                        // yaw_x_read = 2 * (q.w * q.w + q.x * q.x) - 1;
                    SREG = oldSREG;
                    return Euler_angle(
                        atan(gravity_y / sqrt(gravity_x * gravity_x + gravity_z * gravity_z)) * RAD_TO_DEG,
                        atan(gravity_x / sqrt(gravity_y * gravity_y + gravity_z * gravity_z)) * RAD_TO_DEG,
                        yaw < 0 ? yaw + 360.0 : yaw;
                    );
                }

                // Specially made for the external AK8963 magnetometer attached to MPU-9250,
                // but this is also applicable to MPU-6050 chip, so I put this method over here instead of inside Custom_gy9250.
                // This method is removed from this SPI version of MPU-6500 library,
                // because we are here using SPI to communicate with MPU-6500,
                // so there is no need for us to bypass the MPU-6500 chip to access the other slaves through I2C.
                // void enable_ext_i2c_slave_sensors ();
        };
        typedef Mpu_6500<SPIClass, SPISettings> Mpu_6500_default_SPI;
    }
}

#endif // #ifndef MPU_6500_H
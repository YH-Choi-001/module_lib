#ifndef QMC5883L_H
#define QMC5883L_H __DATE__ ", " __TIME__

#include <Arduino.h>
#include <Wire.h>

#ifndef NC_PINNO
#define NC_PINNO 255
#endif

namespace yh {
    namespace rec {
        struct Mag_field_raw_t {
            int16_t x, y, z;
        };
        struct Mag_field_uT_t {
            double x, y, z;
        };
        class Qmc5883l {
            private:
                // the 7-bit I2C address of the chip [0x00:0x7f]
                const uint8_t i2c_address;
                // pins that cannot be changed:
                // DRDY pin (data ready pin) (HIGH for ready, LOW for not ready)
                const uint8_t drdy_pin;
                volatile uint8_t *drdy_pin_in_reg;
                uint8_t drdy_pin_mask;
                // stores the raw values of field strength in each axis
                Mag_field_raw_t raw_data;
                // stores the heading of the compass
                uint16_t heading;
            public:
                // for compass calibration use only
                int16_t x_range, x_min, y_range, y_min, z_range, z_min;
                uint16_t re_zero_heading;
                //
                Qmc5883l (const uint8_t init_i2c_address = 0x0D, const uint8_t init_drdy_pin = NC_PINNO);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // configures the settings of the I2C bus and the chip
                void begin ();

                // low-level functions

                // @brief Resets the QMC5883L in software.
                void soft_reset ();
                // @brief Updates new data from the QMC5883L.
                bool update ();
                // @return The raw value recorded by the magnetometer in the x axis.
                int16_t get_raw_x ();
                // @return The raw value recorded by the magnetometer in the y axis.
                int16_t get_raw_y ();
                // @return The raw value recorded by the magnetometer in the z axis.
                int16_t get_raw_z ();
                // @return A package containing raw values recorded by the magnetometer in 3 axes.
                Mag_field_raw_t get_raw_mag ();
                // The gain is factory-calibrated, but not the offset,
                // so only the relative temperature is accurate.
                // @return The raw value of temperature sensor recorded by the chip in 2's complement (100 LSB / degree Celsius).
                int16_t get_raw_temperature ();
                // @return The magnetic field strength in uT recorded by the magnetometer in the x axis.
                double get_uT_x ();
                // @return The magnetic field strength in uT recorded by the magnetometer in the y axis.
                double get_uT_y ();
                // @return The magnetic field strength in uT recorded by the magnetometer in the z axis.
                double get_uT_z ();
                // @return A package containing the magnetic field strength in uT recorded by the magnetometer in 3 axes.
                Mag_field_uT_t get_uT_mag ();

                // high-level functions

                // @brief calibrate the compass
                void compass_cal ();
                // @brief takes the current heading of compass as zero
                void reset_heading ();
                // @return the heading of compass in degrees [0:359]
                uint16_t get_heading ();
        };
    }
}

#endif // #ifndef QMC5883L_H
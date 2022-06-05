#ifndef TCS3200_H
#define TCS3200_H __DATE__ ", " __TIME__

#include <Arduino.h>

namespace yh {
    namespace rec {
        class Tcs3200 {
            private:
                //
            protected:
                // pins that cannot be changed:
                const uint8_t
                    s0_pin,
                    s1_pin,
                    s2_pin,
                    s3_pin,
                    out_pin;
            public:
                enum filter_t {
                    RED,
                    BLUE,
                    ALL,
                    GREEN
                };
                Tcs3200 (const Tcs3200 &init_obj);
                // @brief pass the s0 - s3 pins and out pin to the constructor
                Tcs3200 (const uint8_t init_s0, const uint8_t init_s1, const uint8_t init_s2, const uint8_t init_s3, const uint8_t init_out);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // @brief select the updating frequency in percentage
                void select_frequency (const uint8_t freq);
                // @brief select the colour filter for the photodiodes
                void select_filter (const filter_t colour);
                // @return the intensity of light received [0 - 255]
                uint16_t read_single_colour ();
                // @param red_val the pointer to uint8_t to write the red val
                // @param green_val the pointer to uint8_t to write the green val
                // @param blue_val the pointer to uint8_t to write the blue val
                void read_rgb (uint8_t *const red_val, uint8_t *const green_val, uint8_t *const blue_val);
                // @param hue the pointer to uint16_t to write the hue
                // @param sat the pointer to uint8_t to write the saturation
                // @param val the pointer to uint8_t to write the value
                void read_hsv (uint16_t *const hue, uint8_t *const sat, uint8_t *const val);
                // @return true if the sensor is seeing white, false otherwise
                bool seeing_white ();
        };
    }
}

#endif // #ifndef TCS3200_H
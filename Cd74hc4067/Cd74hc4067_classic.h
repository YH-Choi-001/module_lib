#ifdef CD74HC4067_H

#ifndef CD74HC4067_CLASSIC_H
#define CD74HC4067_CLASSIC_H __DATE__ ", " __TIME__

#include <Arduino.h>

namespace yh {
    namespace rec {

        #ifndef NC_PINNO
        #define NC_PINNO 255
        #endif // #ifndef NC_PINNO

        //                  +-\/-+
        //    Common I/O   1|    |24  VCC
        //            I7   2|    |23  I8
        //            I6   3|    |22  I9
        //            I5   4|    |21  I10
        //            I4   5|    |20  I11
        //            I3   6|    |19  I12
        //            I2   7|    |18  I13
        //            I1   8|    |17  I14
        //            I0   9|    |16  I15
        //            S0  10|    |15  E
        //            S1  11|    |14  S2
        //           GND  12|    |13  S3
        //                  +----+
        class Cd74hc4067 {
            private:
                //
            protected:
                // pins that cannot be changed:
                const uint8_t
                    pin_s0,
                    pin_s1,
                    pin_s2,
                    pin_s3,
                    pin_en;
                volatile uint8_t
                    *pin_s0_out_reg,
                    *pin_s1_out_reg,
                    *pin_s2_out_reg,
                    *pin_s3_out_reg,
                    *pin_en_out_reg;
                uint8_t
                    pin_s0_mask,
                    pin_s1_mask,
                    pin_s2_mask,
                    pin_s3_mask,
                    pin_en_mask;
            public:
                Cd74hc4067 (const Cd74hc4067 &init_obj);
                //
                Cd74hc4067 (
                    const uint8_t init_pin_s0,
                    const uint8_t init_pin_s1,
                    const uint8_t init_pin_s2,
                    const uint8_t init_pin_s3,
                    const uint8_t init_pin_en
                );
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // select the channel among the 16 analog input channels
                void select_channel (const uint8_t channel);
                // enable the module
                void enable ();
                // disable the module
                void disable ();
        };
    }
}

#endif // #ifndef CD74HC4067_CLASSIC_H

#endif // #ifdef CD74HC4067_H
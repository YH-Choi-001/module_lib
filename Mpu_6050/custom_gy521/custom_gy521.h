#ifndef CUSTOM_GY521_H
#define CUSTOM_GY521_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <Wire.h>

// this class has been customized for RCJ soccer robots use only
class Custom_gy521 {
    private:
        //
    protected:
        //
        const uint8_t i2c_address;
        //
        double yaw;
    public:
        //
        Custom_gy521 (const uint8_t init_i2c_address);
        // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
        // calls pinMode function and config the pin modes
        void begin ();

        // sets yaw to 0
        void reset_yaw ();
        // gets 2 bytes from yaw
        double update_yaw (const double correction);
        // returns not-updated yaw
        double get_yaw ();
};

#endif // #ifndef CUSTOM_GY521_H
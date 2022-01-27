#ifndef UART_COM_H
#define UART_COM_H __DATE__ ", " _TIME__

// declares the class Serial_ to prevent VS_code from reporting errors of undeclared types
class Serial_;

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        // note that if values not on this table are transmitted,
        // the replying arduino may have undefined behaviours
        // note that items that have the name \"9-bit\" actually
        // transmit 10 bits, which the MSB is reserved for data error indication
        enum UART_com_keys_t {
            // capital letters first
            FB_UTS_H       = 'A', // find-ball ultrasound HIGH byte
            // SET_BAUD       = 'B', // set baud rate
            CMPAS_H        = 'C', // compass HIGH byte
            DRBLR_SPD      = 'D', // set dribbler speed (-1, 0, 1)
            SUN_DIR_H      = 'E', // the direction of the strongest ball (Sun) HIGH byte
            SUN_VAL_H      = 'F', // the intensity of the strongest ball (Sun) HIGH byte
            GRYSCL_8_DIR   = 'G', // 8 simple directions of grayscale (F, LF, L, LB, RF, R, RB, B)
            HORI_GRYSCL    = 'H', // horizontal grayscales (backup)
            CMPAS_9        = 'K', // 9-bit compass
            FB_UTS_9       = 'L', // 9-bit find-ball ultrasound
            UTS0_9         = 'M', // 9-bit ultrasound 0
            UTS1_9         = 'N', // 9-bit ultrasound 1
            UTS2_9         = 'O', // 9-bit ultrasound 2
            UTS3_9         = 'P', // 9-bit ultrasound 3
            RST_CMPAS_0    = 'R', // reset compass to 0
            SET_CMPAS      = 'S', // set compass to an inputed direction
            VERT_GRYSCL    = 'V', // vertical grayscales (backup)
            UTS0_H         = 'W', // ultrasound 0 HIGH byte
            UTS1_H         = 'X', // ultrasound 1 HIGH byte
            UTS2_H         = 'Y', // ultrasound 2 HIGH byte
            UTS3_H         = 'Z', // ultrasound 3 HIGH byte
            // small letters next
            FB_UTS_L       = 'a', // find-ball ultrasound LOW byte
            CMPAS_L        = 'c', // compass LOW byte
            SUN_DIR_L      = 'e', // the direction of the strongest ball (Sun) LOW byte
            SUN_VAL_L      = 'f', // the intensity of the strongest ball (Sun) LOW byte
            CAL_GRYSCL     = 'g', // request calibrate grayscales
            SET_MOTOR_DIR  = 'o', // set the direction of the motor
            MOTOR_0_SPD    = 'p', // set the speed of motor 0
            MOTOR_1_SPD    = 'q', // set the speed of motor 1
            MOTOR_2_SPD    = 'r', // set the speed of motor 2
            MOTOR_3_SPD    = 's', // set the speed of motor 3
            WHO_AM_I       = 0x75,// get the identity of the replying arduino
            UTS0_L         = 'w', // ultrasound 0 LOW byte
            UTS1_L         = 'x', // ultrasound 1 LOW byte
            UTS2_L         = 'y', // ultrasound 2 LOW byte
            UTS3_L         = 'z'  // ultrasound 3 LOW byte
            // the priority of returning data from replying arduino:
                // 0. grayscale // should not access grayscale through UART, but digital pins
                // 1. compass     (HIGH > 9-bit > LOW)
                // 2. compoundeye (HIGH > LOW)
                // 3. ultrasound  (HIGH > 9-bit > LOW)
            // then the priority of configuring data in replying aruino:
                // 4. set motor speed
                // 5. set dribbler speed
                // 6. calibrate grayscale
                // 7. software reset compass
                // 8. software set compass direction
        };
        class UART_com {
            private:
                //
            protected:
                // the selected UART serial object
                Serial_ &uart_serial;
                // write a key to replying arduino
                void write_key (const uint8_t key);
                // write a byte of data to replying arduino
                void write_data (const uint8_t key, const uint8_t data);
                // write 2 bytes of data to replying arduino
                void write_data (const uint8_t key, const uint8_t data0, const uint8_t data1);
                // request a byte from writing the key to replying arduino
                uint8_t request_data (const uint8_t key);
                // request a byte from writing the key to replying arduino, with a timeout
                uint8_t request_data (const uint8_t key, const unsigned long timeout);
                // request 2 bytes from writing the key to replying arduino
                uint16_t request_2_data (const uint8_t key);
                // request a byte from writing the key to replying arduino, with a timeout
                uint16_t request_2_data (const uint8_t key, const unsigned long timeout);
                // flag to indicate if request_data(..., timeout) or request_2_data(..., timeout) has occured a time-out
                unsigned int request_timeout : 1;
                // flag to indicate if the replying arduino's who_am_i value is accepted
                unsigned int who_am_i_error : 1;
            public:
                // inits the object of UART serial communication to this object
                // can be Serial, Serial1, Serial2, Serial3 for arduino mega boards
                // depends on which UART you have connected to
                UART_com (Serial_ &init_serial_obj);
                // calls uart_serial.begin(baud) and sets the baud rate
                inline void begin (const uint32_t baud);
                // returns the who_am_i value of the board
                uint8_t get_who_am_i ();

                // motors:

                // (c0 d0 c1 d1 c2 d2 c3 d3)
                // cN flag indicates if the direction needs to be updated
                // dN flag indicates +ive or -ive direction of motor
                void set_motor_direction (const uint8_t direction);
                // set the speed of motor 0
                void set_motor_0_spd (const uint8_t absolute_spd);
                // set the speed of motor 1
                void set_motor_1_spd (const uint8_t absolute_spd);
                // set the speed of motor 2
                void set_motor_2_spd (const uint8_t absolute_spd);
                // set the speed of motor 3
                void set_motor_3_spd (const uint8_t absolute_spd);

                // grayscales:

                // returns if grayscales of (F, LF, L, LB, RF, R, RB, B) direction are touching the white line
                uint8_t grayscales_8_sides_touch_white ();
                // returns if grayscales of (L0, L1, L2, L3, R0, R1, R2, R3) are touching the white line
                uint8_t hori_grayscales_touch_white ();
                // returns if grayscales of (F0, F1, F2, F3, B0, B1, B2, B3) are touching the white line
                uint8_t vert_grayscales_touch_white ();
                // holds the thread and waits for the calibration of grayscales to be completed
                void request_calibrate_grayscales ();

                // compoundeyes:

                // returns the direction of the Sun with a resolution of 4degrees ( << 2 )
                uint16_t sun_direction_HIGH ();
                // returns the intensity of the Sun with a resolution of 4units_of_brightness ( << 2 )
                uint16_t sun_intensity_HIGH ();
                // returns the direction of the Sun % 256
                uint8_t sun_direction_LOW ();
                // returns the intensity of the Sun % 256
                uint8_t sun_intensity_LOW ();

                // ultrasounds:

                // returns the distance in cm with a resolution of 4cm ( << 2 )
                uint16_t find_ball_ultrasound_HIGH ();
                // returns the distance in cm % 256
                uint8_t find_ball_ultrasound_LOW ();
                // returns the distance in cm with a resolution of 4cm ( << 2 )
                uint16_t ultrasound_0_HIGH ();
                // returns the distance in cm % 256
                uint8_t ultrasound_0_LOW ();
                // returns the distance in cm with a resolution of 4cm ( << 2 )
                uint16_t ultrasound_1_HIGH ();
                // returns the distance in cm % 256
                uint8_t ultrasound_1_LOW ();
                // returns the distance in cm with a resolution of 4cm ( << 2 )
                uint16_t ultrasound_2_HIGH ();
                // returns the distance in cm % 256
                uint8_t ultrasound_2_LOW ();
                // returns the distance in cm with a resolution of 4cm ( << 2 )
                uint16_t ultrasound_3_HIGH ();
                // returns the distance in cm % 256
                uint8_t ultrasound_3_LOW ();

                // returns the distance in cm with a resolution of 1cm
                uint16_t find_ball_ultrasound_9bit ();
                // returns the distance in cm with a resolution of 1cm
                uint16_t ultrasound_0_9bit ();
                // returns the distance in cm with a resolution of 1cm
                uint16_t ultrasound_1_9bit ();
                // returns the distance in cm with a resolution of 1cm
                uint16_t ultrasound_2_9bit ();
                // returns the distance in cm with a resolution of 1cm
                uint16_t ultrasound_3_9bit ();

                // compass:

                // returns the direction of field_north with a resolution of 4degrees ( << 2 )
                uint16_t compass_HIGH ();
                // returns the direction of field_north % 256
                uint8_t compass_LOW ();
                // returns the direction of field_north with a resolution of 1degree
                uint16_t compass_9bit ();
                // resets the field_north of the compass to current facing direction
                void reset_compass ();
                // sets the current facing direction to an inputed direction
                void set_compass_direction (const uint16_t direction);

                // dribbler:

                // sets the speed of the dribbler to (clockwise or anti-clockwise with full-power) or (0)
                void set_dribbler_spd (const int8_t spd);
        };
    }
}

#endif // #ifndef UART_COM_H
#ifndef BC_COM_H
#define BC_COM_H __DATE__ ", " _TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        // This key chart is designed for data exchange of 2 arduinos
        // through communication protocol with clock lines such as I2C and SPI,
        // which relies on the clock line to indicate end of burst readings, to increase efficiency.
        // This key chart is not suggested to use on UART, bluetooth and Zigbee devices.
        // Note that if values not on this table are transmitted,
        // the replying device may have undefined behaviours.
        // @return the Board Communication With Clock line communication keys
        #define BC_WCL_com_keys_t_CAP_SUFFIX -'A'
        #define BC_WCL_com_keys_t_SML_SUFFIX -'a'+26
        enum BC_WCL_com_keys_t {
            // capital letters first
            UTS_0_H = 'A' BC_WCL_com_keys_t_CAP_SUFFIX,
            UTS_0_L = 'B' BC_WCL_com_keys_t_CAP_SUFFIX,
            UTS_1_H = 'C' BC_WCL_com_keys_t_CAP_SUFFIX,
            UTS_1_L = 'D' BC_WCL_com_keys_t_CAP_SUFFIX,
            UTS_2_H = 'E' BC_WCL_com_keys_t_CAP_SUFFIX,
            UTS_2_L = 'F' BC_WCL_com_keys_t_CAP_SUFFIX,
            UTS_3_H = 'G' BC_WCL_com_keys_t_CAP_SUFFIX,
            UTS_3_L = 'H' BC_WCL_com_keys_t_CAP_SUFFIX,
            UTS_4_H = 'I' BC_WCL_com_keys_t_CAP_SUFFIX,
            UTS_4_L = 'J' BC_WCL_com_keys_t_CAP_SUFFIX,
            YAW_H   = 'K' BC_WCL_com_keys_t_CAP_SUFFIX,
            YAW_L   = 'L' BC_WCL_com_keys_t_CAP_SUFFIX,
            PITCH_H = 'M' BC_WCL_com_keys_t_CAP_SUFFIX,
            PITCH_L = 'N' BC_WCL_com_keys_t_CAP_SUFFIX,
            ROLL_H  = 'O' BC_WCL_com_keys_t_CAP_SUFFIX,
            ROLL_L  = 'P' BC_WCL_com_keys_t_CAP_SUFFIX,
            HORI_GRYSCLS = 'Q' BC_WCL_com_keys_t_CAP_SUFFIX,
            VERT_GRYSCLS = 'R' BC_WCL_com_keys_t_CAP_SUFFIX,
            SUN_DIR_H    = 'S' BC_WCL_com_keys_t_CAP_SUFFIX,
            SUN_DIR_L    = 'T' BC_WCL_com_keys_t_CAP_SUFFIX,
            SUN_INTEN_H  = 'U' BC_WCL_com_keys_t_CAP_SUFFIX,
            SUN_INTEN_L  = 'V' BC_WCL_com_keys_t_CAP_SUFFIX,
            BAL_DIR_H    = 'W' BC_WCL_com_keys_t_CAP_SUFFIX,
            BAL_DIR_L    = 'X' BC_WCL_com_keys_t_CAP_SUFFIX,
            BAL_INTEN_H  = 'Y' BC_WCL_com_keys_t_CAP_SUFFIX,
            BAL_INTEN_L  = 'Z' BC_WCL_com_keys_t_CAP_SUFFIX,
            // small letters next
            SET_UTS_0_RNG = 'a' BC_WCL_com_keys_t_SML_SUFFIX,
            SET_UTS_1_RNG = 'b' BC_WCL_com_keys_t_SML_SUFFIX,
            SET_UTS_2_RNG = 'c' BC_WCL_com_keys_t_SML_SUFFIX,
            SET_UTS_3_RNG = 'd' BC_WCL_com_keys_t_SML_SUFFIX,
            SET_UTS_4_RNG = 'e' BC_WCL_com_keys_t_SML_SUFFIX,
            SET_MOTOR_DIR = 'f' BC_WCL_com_keys_t_SML_SUFFIX,
            SET_MOTOR_0_SPD = 'g' BC_WCL_com_keys_t_SML_SUFFIX,
            SET_MOTOR_1_SPD = 'h' BC_WCL_com_keys_t_SML_SUFFIX,
            SET_MOTOR_2_SPD = 'i' BC_WCL_com_keys_t_SML_SUFFIX,
            SET_MOTOR_3_SPD = 'j' BC_WCL_com_keys_t_SML_SUFFIX,
            RESET_GYRO_QUAT = 'k' BC_WCL_com_keys_t_SML_SUFFIX,
            CAL_GYRO        = 'l' BC_WCL_com_keys_t_SML_SUFFIX,
            CAL_GRYSCL      = 'q' BC_WCL_com_keys_t_SML_SUFFIX,
            SET_DEFAULT_REG = 'r' BC_WCL_com_keys_t_SML_SUFFIX,
            SET_DRBLR_SPD   = 's' BC_WCL_com_keys_t_SML_SUFFIX,
            TOG_MIRROR_MODE = 't' BC_WCL_com_keys_t_SML_SUFFIX,
            WHO_AM_I        = 'u' BC_WCL_com_keys_t_SML_SUFFIX
            // the priority of returning data from replying arduino:
                // 1. compass     (HIGH > LOW)
                // 2. ultrasound  (HIGH > LOW)
                // 3. grayscale   (HORI > VERT)
                // 4. compoundeye (HIGH > LOW)
            // then the priority of configuring data in replying aruino:
                // 5. set motor dir and speed
                // 6. set dribbler speed
                // 7. software reset compass
                // 8. calibrate grayscale
                // 9. calibrate gyroscope
                // 10. set range of ultrasounds
                // 11. return WHO_AM_I
        };
        // This key chart is designed for data exchange of 2 arduinos
        // through the UART communication protocol,
        // which the keys tell the receiver to expect how many bytes of data.
        // This key chart has a lower efficiency in data exchange than the BC_WCL version.
        // Note that if values not on this table are transmitted,
        // the replying device may have undefined behaviours.
        // @return the Board Communication through UART communication keys
        enum BC_UART_com_keys_t {
            // capital letters first
            UTS_0_H     = 'A',
            UTS_0_9_bit = 'B',
            UTS_1_H     = 'C',
            UTS_1_9_bit = 'D',
            UTS_2_H     = 'E',
            UTS_2_9_bit = 'F',
            UTS_3_H     = 'G',
            UTS_3_9_bit = 'H',
            UTS_4_H     = 'I',
            UTS_4_9_bit = 'J',
            YAW_H        = 'K',
            YAW_9_bit    = 'L',
            PITCH_H      = 'M',
            PITCH_9_bit  = 'N',
            ROLL_H       = 'O',
            ROLL_9_bit   = 'P',
            HORI_GRYSCLS = 'Q',
            VERT_GRYSCLS = 'R',
            SUN_DIR_H    = 'S',
            SUN_DIR_9_bit    = 'T',
            SUN_INTEN_H      = 'U',
            SUN_INTEN_9_bit  = 'V',
            BAL_DIR_H        = 'W',
            BAL_DIR_9_bit    = 'X',
            BAL_INTEN_H      = 'Y',
            BAL_INTEN_9_bit  = 'Z',
            // small letters next
            SET_UTS_0_RNG = 'a',
            SET_UTS_1_RNG = 'b',
            SET_UTS_2_RNG = 'c',
            SET_UTS_3_RNG = 'd',
            SET_UTS_4_RNG = 'e',
            SET_MOTOR_DIR = 'f',
            SET_MOTOR_0_SPD = 'g',
            SET_MOTOR_1_SPD = 'h',
            SET_MOTOR_2_SPD = 'i',
            SET_MOTOR_3_SPD = 'j',
            RESET_GYRO_QUAT = 'k',
            CAL_GYRO        = 'l',
            CAL_GRYSCL      = 'q',
            SET_DEFAULT_REG = 'r',
            SET_DRBLR_SPD   = 's',
            TOG_MIRROR_MODE = 't',
            WHO_AM_I        = 'u'
            // the priority of returning data from replying arduino:
                // 1. compass     (HIGH > LOW)
                // 2. ultrasound  (HIGH > LOW)
                // 3. grayscale   (HORI > VERT)
                // 4. compoundeye (HIGH > LOW)
            // then the priority of configuring data in replying aruino:
                // 5. set motor dir and speed
                // 6. set dribbler speed
                // 7. software reset compass
                // 8. calibrate grayscale
                // 9. calibrate gyroscope
                // 10. set range of ultrasounds
                // 11. return WHO_AM_I
        };
        class Inter_board_com_slave {
            private:
                //
            protected:
                // the flag to indicate whether the received byte is data or address;
                bool is_data;
                // the counter that tells which register to be outputed
                uint8_t reg_counter;
                //
                virtual uint8_t read_byte () { return 0; }
                //
                virtual void write_byte (uint8_t data) { data = data; return; }
            public:
                //
                Inter_board_com_slave () :
                    is_data(false)
                {
                    //
                }
                // 26 * 2
                volatile uint8_t com_reg [52];
                //
                void on_receive_service () {
                    if (is_data) {
                        // reg_counter < 97 ? reg_counter - 'A' : reg_counter - 'a';
                        com_reg[reg_counter] = read_byte();
                        reg_counter++;
                        if ( reg_counter == (sizeof(com_reg) / sizeof(com_reg[0])) )
                            reg_counter = 0;
                    } else {
                        reg_counter = read_byte();
                        is_data = true;
                    }
                }
                //
                void on_transmit_service () {
                    write_byte(com_reg[reg_counter]);
                }
                //
                void reset_commnication () {
                    is_data = false;
                    reg_counter = com_reg[BC_WCL_com_keys_t::SET_DEFAULT_REG];
                }
        };
        class BC_com_slave {
            private:
                //
            protected:
                // read the data received
                virtual uint8_t read_key_or_data ();
                // write a byte of data to master arduino
                virtual void write_data (const uint8_t data);
                // write 2 bytes of data to replying arduino
                virtual void write_data_9bit (const uint16_t data);
                // Who am I? (as a slave)
                uint8_t who_am_i;
            public:
                // constructor
                BC_com_slave ();
                // returns the who_am_i value of the replying arduino
                void set_who_am_i (const uint8_t who_am_i_val) { who_am_i = who_am_i_val; }

                // method to be called on receive:
                void on_receive ();

                // motors:

                // (c0 d0 c1 d1 c2 d2 c3 d3)
                // cN flag indicates if the direction needs to be updated
                // dN flag indicates +ive or -ive direction of motor
                virtual void set_motor_direction (const uint8_t direction);
                // set the speed of motor 0
                virtual void set_motor_0_spd (const uint8_t absolute_spd);
                // set the speed of motor 1
                virtual void set_motor_1_spd (const uint8_t absolute_spd);
                // set the speed of motor 2
                virtual void set_motor_2_spd (const uint8_t absolute_spd);
                // set the speed of motor 3
                virtual void set_motor_3_spd (const uint8_t absolute_spd);

                // grayscales:

                // returns if grayscales of (F, LF, L, LB, RF, R, RB, B) direction are touching the white line
                virtual uint8_t grayscales_8_sides_touch_white ();
                // returns if grayscales of (L0, L1, L2, L3, R0, R1, R2, R3) are touching the white line
                virtual uint8_t hori_grayscales_touch_white ();
                // returns if grayscales of (F0, F1, F2, F3, B0, B1, B2, B3) are touching the white line
                virtual uint8_t vert_grayscales_touch_white ();
                // holds the thread and waits for the calibration of grayscales to be completed
                virtual void request_calibrate_grayscales ();

                // compoundeyes:

                // returns the direction of the Sun with a resolution of 2degrees ( << 1 )
                virtual uint8_t sun_direction_HIGH ();
                // returns the intensity of the Sun with a resolution of 2units_of_brightness ( << 1 )
                virtual uint8_t sun_intensity_HIGH ();
                // returns the direction of the Sun % 256
                virtual uint8_t sun_direction_LOW ();
                // returns the intensity of the Sun % 256
                virtual uint8_t sun_intensity_LOW ();

                // returns the direction of the filter_ball with a resolution of 2degrees ( << 1 )
                virtual uint8_t filter_ball_direction_HIGH ();
                // returns the intensity of the filter_ball with a resolution of 2units_of_brightness ( << 1 )
                virtual uint8_t filter_ball_intensity_HIGH ();
                // returns the direction of the filter_ball % 256
                virtual uint8_t filter_ball_direction_LOW ();
                // returns the intensity of the filter_ball % 256
                virtual uint8_t filter_ball_intensity_LOW ();

                // ultrasounds:

                // returns the distance in cm with a resolution of 2cm ( << 1 )
                virtual uint8_t find_ball_ultrasound_HIGH ();
                // returns the distance in cm % 256
                virtual uint8_t find_ball_ultrasound_LOW ();
                // returns the distance in cm with a resolution of 2cm ( << 1 )
                virtual uint8_t ultrasound_0_HIGH ();
                // returns the distance in cm % 256
                virtual uint8_t ultrasound_0_LOW ();
                // returns the distance in cm with a resolution of 2cm ( << 1 )
                virtual uint8_t ultrasound_1_HIGH ();
                // returns the distance in cm % 256
                virtual uint8_t ultrasound_1_LOW ();
                // returns the distance in cm with a resolution of 2cm ( << 1 )
                virtual uint8_t ultrasound_2_HIGH ();
                // returns the distance in cm % 256
                virtual uint8_t ultrasound_2_LOW ();
                // returns the distance in cm with a resolution of 2cm ( << 1 )
                virtual uint8_t ultrasound_3_HIGH ();
                // returns the distance in cm % 256
                virtual uint8_t ultrasound_3_LOW ();

                // returns the distance in cm with a resolution of 1cm
                virtual uint16_t find_ball_ultrasound_9bit ();
                // returns the distance in cm with a resolution of 1cm
                virtual uint16_t ultrasound_0_9bit ();
                // returns the distance in cm with a resolution of 1cm
                virtual uint16_t ultrasound_1_9bit ();
                // returns the distance in cm with a resolution of 1cm
                virtual uint16_t ultrasound_2_9bit ();
                // returns the distance in cm with a resolution of 1cm
                virtual uint16_t ultrasound_3_9bit ();

                // sets the upper limit of valid distance read (in cm) by ultrasound 0
                virtual void set_ultrasound_0_range (const uint16_t dist_in_cm);
                // sets the upper limit of valid distance read (in cm) by ultrasound 1
                virtual void set_ultrasound_1_range (const uint16_t dist_in_cm);
                // sets the upper limit of valid distance read (in cm) by ultrasound 2
                virtual void set_ultrasound_2_range (const uint16_t dist_in_cm);
                // sets the upper limit of valid distance read (in cm) by ultrasound 3
                virtual void set_ultrasound_3_range (const uint16_t dist_in_cm);

                // compass:

                // returns the direction of field_north with a resolution of 2degrees ( << 1 )
                virtual uint16_t compass_HIGH ();
                // returns the direction of field_north % 256
                virtual uint8_t compass_LOW ();
                // returns the direction of field_north with a resolution of 1degree
                virtual uint16_t compass_9bit ();
                // resets the field_north of the compass to current facing direction
                virtual void reset_compass ();
                // sets the current facing direction to an inputed direction
                virtual void set_compass_direction (const uint16_t direction);

                // dribbler:

                // sets the speed of the dribbler to (clockwise or anti-clockwise with full-power) or (0)
                virtual void set_dribbler_spd (const int8_t spd);
        };
    }
}

#endif // #ifndef UART_COM_H
#ifndef NINGOR_IR_H
#define NINGOR_IR_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#ifndef NC_PINNO
#define NC_PINNO 255
#endif // #ifndef NC_PINNO

namespace yh {
    namespace rec {
        class Ningor_ir_fast {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the pins connected to the corresponding eyes
                const uint8_t eyes_pins [12];
                // this function decides how to read an eye
                // you may override me to use an external ADC
                virtual inline uint16_t raw_read_one_eye (const uint8_t eye_idx) { return (uint16_t)analogRead(eyes_pins[eye_idx]); }
            public:
                // inits the pins of 12 eyes into the object using an array argument
                Ningor_ir_fast (const uint8_t *init_eyes_pin_array);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin ();
                // returns the reading of the eye requested
                virtual uint16_t read_one_eye (const uint8_t eye_idx);
                // returns the index of the max eye
                virtual uint8_t idx_of_max_eye ();
                // returns the reading of the max eye
                virtual uint16_t val_of_max_eye ();
                // returns the direction of the strongest ball with the mean of the max eye and its neighbouring 2 eyes
                virtual uint16_t get_ball_direction ();
        };
        class Ningor_ir : public Ningor_ir_fast {
            private:
                //
            protected:
                // pins that cannot be changed:
                // the reading of the eyes
                uint16_t eyes_reading [12];
                // the index of the max eye
                uint8_t eye_no_of_max_eye;
                // the direction of the ball
                uint16_t dir_of_ball;
                // refreshes the data of all 12 eyes
                void refresh_eyes ();
            public:
                // inits the pins of 12 eyes into the object using an array argument
                Ningor_ir (const uint8_t *init_eyes_pin_array);
                // returns the reading of the eye requested
                uint16_t read_one_eye (const uint8_t eye_idx);
                // returns the reading of the eye requested
                uint16_t read_one_eye (const uint8_t eye_idx, const bool refresh);
                // returns the index of the max eye
                uint8_t idx_of_max_eye ();
                // returns the index of the max eye
                uint8_t idx_of_max_eye (const bool refresh);
                // returns the reading of the max eye
                uint16_t val_of_max_eye ();
                // returns the reading of the max eye
                uint16_t val_of_max_eye (const bool refresh);
                // returns the direction of the strongest ball with the mean of the max eye and its neighbouring 2 eyes
                uint16_t get_ball_direction ();
                // returns the direction of the strongest ball with the mean of the max eye and its neighbouring 2 eyes
                uint16_t get_ball_direction (const bool refresh);
        };
        class Ningor_ir_filtered : public Ningor_ir {
            private:
                //
            protected:
                // the reading of the filtered eyes
                uint16_t eyes_reading_filtered [12];
                // the index of the max eye filtered
                uint8_t eye_no_of_max_eye_filtered;
                // the direction of the ball filtered
                uint16_t dir_of_ball_filtered;
                // refreshes the filtered data of all 12 eyes
                void refresh_eyes_filtered ();
            public:
                // inits the pins of 12 eyes into the object using an array argument
                Ningor_ir_filtered (const uint8_t *init_eyes_pin_array);
                // returns the reading of the eye requested with 833-microseconds filtered
                uint16_t read_one_eye_filtered (const uint8_t eye_idx);
                // returns the reading of the eye requested with 833-microseconds filtered
                uint16_t read_one_eye_filtered (const uint8_t eye_idx, const bool refresh);
                // returns the index of the max eye with 833-microseconds * 12 filtered
                uint8_t idx_of_max_eye_filtered ();
                // returns the index of the max eye with 833-microseconds * 12 filtered
                uint8_t idx_of_max_eye_filtered (const bool refresh);
                // returns the reading of the max eye with 833-microseconds * 12 filtered
                uint16_t val_of_max_eye_filtered ();
                // returns the reading of the max eye with 833-microseconds * 12 filtered
                uint16_t val_of_max_eye_filtered (const bool refresh);
                // returns the direction of the strongest ball with the mean of the max eye and its neighbouring 2 eyes with 833-microseconds * 12 filtered
                uint16_t get_ball_direction_filtered ();
                // returns the direction of the strongest ball with the mean of the max eye and its neighbouring 2 eyes with 833-microseconds * 12 filtered
                uint16_t get_ball_direction_filtered (const bool refresh);
        };
    }
}

#endif // #ifndef NINGOR_IR_H
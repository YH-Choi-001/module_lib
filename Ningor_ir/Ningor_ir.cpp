#ifndef NINGOR_IR_CPP
#define NINGOR_IR_CPP __DATE__ ", " __TIME__

#include "Ningor_ir.h"


yh::rec::Ningor_ir_fast::Ningor_ir_fast (const uint8_t *arr) :
    eyes_pins {
        arr[0], arr[1], arr[2], arr[3], arr[4], arr[5],
        arr[6], arr[7], arr[8], arr[9], arr[10], arr[11]
    }
{
    //
}

void yh::rec::Ningor_ir_fast::begin () {
    #define SETPINMODE(x) pinMode(eyes_pins[x], INPUT);
    SETPINMODE(0)SETPINMODE(1)SETPINMODE(2)SETPINMODE(3)
    SETPINMODE(4)SETPINMODE(5)SETPINMODE(6)SETPINMODE(7)
    SETPINMODE(8)SETPINMODE(9)SETPINMODE(10)SETPINMODE(11)
    #undef SETPINMODE
}

uint16_t yh::rec::Ningor_ir_fast::raw_read_one_eye (const uint8_t eye_idx) {
    return analogRead(eyes_pins[eye_idx]);
    // return 1023 - analogRead(eyes_pins[eye_idx]);
}

uint16_t yh::rec::Ningor_ir_fast::read_one_eye (const uint8_t eye_idx) {
    return raw_read_one_eye(eye_idx);
}

uint8_t yh::rec::Ningor_ir_fast::idx_of_max_eye () {
    uint8_t max_idx = 0;
    uint16_t max_val = raw_read_one_eye(0), temp = 0;
    #define LOGIC_CHECK(x) if((temp=raw_read_one_eye(x))>max_val) max_val=temp,max_idx=x;
                  LOGIC_CHECK(1)LOGIC_CHECK(2)LOGIC_CHECK(3)
    LOGIC_CHECK(4)LOGIC_CHECK(5)LOGIC_CHECK(6)LOGIC_CHECK(7)
    LOGIC_CHECK(8)LOGIC_CHECK(9)LOGIC_CHECK(10)LOGIC_CHECK(11)
    #undef LOGIC_CHECK
    return max_idx;
}

uint16_t yh::rec::Ningor_ir_fast::val_of_max_eye () {
    uint16_t max_val = raw_read_one_eye(0), temp = 0;
    #define LOGIC_CHECK(x) if((temp=raw_read_one_eye(x))>max_val) max_val=temp;
                  LOGIC_CHECK(1)LOGIC_CHECK(2)LOGIC_CHECK(3)
    LOGIC_CHECK(4)LOGIC_CHECK(5)LOGIC_CHECK(6)LOGIC_CHECK(7)
    LOGIC_CHECK(8)LOGIC_CHECK(9)LOGIC_CHECK(10)LOGIC_CHECK(11)
    #undef LOGIC_CHECK
    return max_val;
}

uint16_t yh::rec::Ningor_ir_fast::get_ball_direction () {
    uint16_t eyes_val [12];
    uint8_t max_idx = 0;
    uint16_t max_val = raw_read_one_eye(0);
    #define LOGIC_CHECK(x) if((eyes_val[x]=raw_read_one_eye(x))>max_val) max_val=eyes_val[x],max_idx=x;
                  LOGIC_CHECK(1)LOGIC_CHECK(2)LOGIC_CHECK(3)
    LOGIC_CHECK(4)LOGIC_CHECK(5)LOGIC_CHECK(6)LOGIC_CHECK(7)
    LOGIC_CHECK(8)LOGIC_CHECK(9)LOGIC_CHECK(10)LOGIC_CHECK(11)
    #undef LOGIC_CHECK
    // const uint16_t
    //     &right = eyes_val[(max_idx != 11) * (max_idx + 1)], // (eye_no_of_max_eye == 11) ? 0 : (eye_no_of_max_eye + 1)
    //     &left = eyes_val[max_idx ? (max_idx - 1) : 11];     // (eye_no_of_max_eye == 0) ? 11 : (eye_no_of_max_eye - 1)
    // const uint16_t dir_of_ball = 30 * max_idx +
    //     15 // magnifies the difference of 2 eyes in fraction to +/- 15 degrees
    //     * (right - left) // this difference automatically turns the offset to -ive when the ball bears left, but +ive when the ball bears right of max_eye
    //     / ( max_val - ((left < right) ? left : right) ); // the difference of max and (lower one between left and right)
    // return (dir_of_ball < 0) ? (dir_of_ball + 360) : dir_of_ball;
    const int16_t
        right = eyes_val[(max_idx != 11) * (max_idx + 1)], // (eye_no_of_max_eye == 11) ? 0 : (eye_no_of_max_eye + 1)
        left = eyes_val[max_idx ? (max_idx - 1) : 11];     // (eye_no_of_max_eye == 0) ? 11 : (eye_no_of_max_eye - 1)
    const int16_t fake_dir_of_ball = 30 * max_idx +
        15 // magnifies the difference of 2 eyes in fraction to +/- 15 degrees
        * (right - left) // this difference automatically turns the offset to -ive when the ball bears left, but +ive when the ball bears right of max_eye
        / ( max_val - ((left < right) ? left : right) ); // the difference of max and (lower one between left and right)
        return (fake_dir_of_ball < 0) ? (fake_dir_of_ball + 360) : fake_dir_of_ball;
}

yh::rec::Ningor_ir::Ningor_ir (const uint8_t *arr) :
    yh::rec::Ningor_ir_fast(arr), eye_no_of_max_eye(0), dir_of_ball(0)
{
    //
}

void yh::rec::Ningor_ir::refresh_eyes () {
    eye_no_of_max_eye = 0;
    #define LOGIC_CHECK(x) if((eyes_reading[x]=raw_read_one_eye(x))>eyes_reading[eye_no_of_max_eye]) eye_no_of_max_eye=x;
    LOGIC_CHECK(0)LOGIC_CHECK(1)LOGIC_CHECK(2)LOGIC_CHECK(3)
    LOGIC_CHECK(4)LOGIC_CHECK(5)LOGIC_CHECK(6)LOGIC_CHECK(7)
    LOGIC_CHECK(8)LOGIC_CHECK(9)LOGIC_CHECK(10)LOGIC_CHECK(11)
    #undef LOGIC_CHECK
}

uint16_t yh::rec::Ningor_ir::read_one_eye (const uint8_t eye_idx) {
    return eyes_reading[eye_idx] = raw_read_one_eye(eye_idx);
}


uint16_t yh::rec::Ningor_ir::read_one_eye (const uint8_t eye_idx, const bool refresh) {
    if (refresh) eyes_reading[eye_idx] = raw_read_one_eye(eye_idx);
    return eyes_reading[eye_idx];
}

uint8_t yh::rec::Ningor_ir::idx_of_max_eye () {
    refresh_eyes();
    return eye_no_of_max_eye;
}

uint8_t yh::rec::Ningor_ir::idx_of_max_eye (const bool refresh) {
    if (refresh) refresh_eyes();
    return eye_no_of_max_eye;
}

uint16_t yh::rec::Ningor_ir::val_of_max_eye () {
    refresh_eyes();
    return eyes_reading[eye_no_of_max_eye];
}

uint16_t yh::rec::Ningor_ir::val_of_max_eye (const bool refresh) {
    if (refresh) refresh_eyes();
    return eyes_reading[eye_no_of_max_eye];
}

uint16_t yh::rec::Ningor_ir::get_ball_direction () {
    // The math that Tony said they have developed for a long time,
    // which is very mature,
    // is presented below.
    refresh_eyes();
    const int16_t
        right = eyes_reading[(eye_no_of_max_eye != 11) * (eye_no_of_max_eye + 1)], // (eye_no_of_max_eye == 11) ? 0 : (eye_no_of_max_eye + 1)
        left = eyes_reading[eye_no_of_max_eye ? (eye_no_of_max_eye - 1) : 11];     // (eye_no_of_max_eye == 0) ? 11 : (eye_no_of_max_eye - 1)
    const int16_t fake_dir_of_ball = 30 * eye_no_of_max_eye +
        15 // magnifies the difference of 2 eyes in fraction to +/- 15 degrees
        * (right - left) // this difference automatically turns the offset to -ive when the ball bears left, but +ive when the ball bears right of max_eye
        / ( eyes_reading[eye_no_of_max_eye] - ((left < right) ? left : right) ); // the difference of max and (lower one between left and right)
        dir_of_ball = (fake_dir_of_ball < 0) ? (fake_dir_of_ball + 360) : fake_dir_of_ball;
    return dir_of_ball;
}

uint16_t yh::rec::Ningor_ir::get_ball_direction (const bool refresh) {
    // The math that Tony said they have developed for a long time,
    // which is very mature,
    // is presented below.
    if (refresh) {
        refresh_eyes();
        const int16_t
            right = eyes_reading[(eye_no_of_max_eye != 11) * (eye_no_of_max_eye + 1)], // (eye_no_of_max_eye == 11) ? 0 : (eye_no_of_max_eye + 1)
            left = eyes_reading[eye_no_of_max_eye ? (eye_no_of_max_eye - 1) : 11];     // (eye_no_of_max_eye == 0) ? 11 : (eye_no_of_max_eye - 1)
        const int16_t fake_dir_of_ball = 30 * eye_no_of_max_eye +
            15 // magnifies the difference of 2 eyes in fraction to +/- 15 degrees
            * (right - left) // this difference automatically turns the offset to -ive when the ball bears left, but +ive when the ball bears right of max_eye
            / ( eyes_reading[eye_no_of_max_eye] - ((left < right) ? left : right) ); // the difference of max and (lower one between left and right)
            dir_of_ball = (fake_dir_of_ball < 0) ? (fake_dir_of_ball + 360) : fake_dir_of_ball;
    }
    return dir_of_ball;
}

yh::rec::Ningor_ir_filtered::Ningor_ir_filtered (const uint8_t *arr) :
    yh::rec::Ningor_ir(arr), eye_no_of_max_eye_filtered(0), dir_of_ball_filtered(0)
{
    //
}

void yh::rec::Ningor_ir_filtered::refresh_eyes_filtered () {
    eye_no_of_max_eye_filtered = 0;
    #define LOGIC_CHECK(x) if((eyes_reading_filtered[x]=read_one_eye_filtered(x))>eyes_reading_filtered[eye_no_of_max_eye_filtered]) eye_no_of_max_eye_filtered=x;
    LOGIC_CHECK(0)LOGIC_CHECK(1)LOGIC_CHECK(2)LOGIC_CHECK(3)
    LOGIC_CHECK(4)LOGIC_CHECK(5)LOGIC_CHECK(6)LOGIC_CHECK(7)
    LOGIC_CHECK(8)LOGIC_CHECK(9)LOGIC_CHECK(10)LOGIC_CHECK(11)
    #undef LOGIC_CHECK
}

uint16_t yh::rec::Ningor_ir_filtered::read_one_eye_filtered (const uint8_t eye_idx) {
    uint16_t data_sampled [10];
    uint8_t eye_read = 0;
    const unsigned long init_time = micros();
    do {
        data_sampled[eye_read++] = raw_read_one_eye(eye_idx);
    } while ((micros() - init_time) < 833);
    uint8_t highest_sample_idx = 0; // index of the sample with the largest reading
    for (uint8_t pcsing_sample_idx = 0; pcsing_sample_idx < eye_read; pcsing_sample_idx++) {
        if (data_sampled[pcsing_sample_idx] > data_sampled[highest_sample_idx]) highest_sample_idx = pcsing_sample_idx;
    }
    return eyes_reading_filtered[eye_idx] = data_sampled[highest_sample_idx] - data_sampled[highest_sample_idx ? (highest_sample_idx - 1) : (eye_read - 1)];
}

uint16_t yh::rec::Ningor_ir_filtered::read_one_eye_filtered (const uint8_t eye_idx, const bool refresh) {
    if (refresh) {
        uint16_t data_sampled [10];
        uint8_t eye_read = 0;
        const unsigned long init_time = micros();
        do {
            data_sampled[eye_read++] = raw_read_one_eye(eye_idx);
        } while ((micros() - init_time) < 833);
        uint8_t highest_sample_idx = 0; // index of the sample with the largest reading
        for (uint8_t pcsing_sample_idx = 0; pcsing_sample_idx < eye_read; pcsing_sample_idx++) {
            if (data_sampled[pcsing_sample_idx] > data_sampled[highest_sample_idx]) highest_sample_idx = pcsing_sample_idx;
        }
        eyes_reading_filtered[eye_idx] = data_sampled[highest_sample_idx] - data_sampled[highest_sample_idx ? (highest_sample_idx - 1) : (eye_read - 1)];
    }
    return eyes_reading_filtered[eye_idx];
}

uint8_t yh::rec::Ningor_ir_filtered::idx_of_max_eye_filtered () {
    refresh_eyes_filtered();
    return eye_no_of_max_eye_filtered;
}

uint8_t yh::rec::Ningor_ir_filtered::idx_of_max_eye_filtered (const bool refresh) {
    if (refresh) refresh_eyes_filtered();
    return eye_no_of_max_eye_filtered;
}

uint16_t yh::rec::Ningor_ir_filtered::val_of_max_eye_filtered () {
    refresh_eyes_filtered();
    return eyes_reading_filtered[eye_no_of_max_eye_filtered];
}

uint16_t yh::rec::Ningor_ir_filtered::val_of_max_eye_filtered (const bool refresh) {
    if (refresh) refresh_eyes_filtered();
    return eyes_reading_filtered[eye_no_of_max_eye_filtered];
}

uint16_t yh::rec::Ningor_ir_filtered::get_ball_direction_filtered () {
    // The math that Tony said they have developed for a long time,
    // which is very mature,
    // is presented below.
    refresh_eyes_filtered();
    const int16_t
        right = eyes_reading_filtered[(eye_no_of_max_eye_filtered != 11) * (eye_no_of_max_eye_filtered + 1)], // (eye_no_of_max_eye == 11) ? 0 : (eye_no_of_max_eye + 1)
        left = eyes_reading_filtered[eye_no_of_max_eye_filtered ? (eye_no_of_max_eye_filtered - 1) : 11];     // (eye_no_of_max_eye == 0) ? 11 : (eye_no_of_max_eye - 1)
    const int16_t fake_dir_of_ball_filtered = 30 * eye_no_of_max_eye_filtered +
        15 // magnifies the difference of 2 eyes in fraction to +/- 15 degrees
        * (right - left) // this difference automatically turns the offset to -ive when the ball bears left, but +ive when the ball bears right of max_eye
        / ( eyes_reading_filtered[eye_no_of_max_eye_filtered] - ((left < right) ? left : right) ); // the difference of max and (lower one between left and right)
        dir_of_ball_filtered = (fake_dir_of_ball_filtered < 0) ? (fake_dir_of_ball_filtered + 360) : fake_dir_of_ball_filtered;
    return dir_of_ball_filtered;
}

uint16_t yh::rec::Ningor_ir_filtered::get_ball_direction_filtered (const bool refresh) {
    // The math that Tony said they have developed for a long time,
    // which is very mature,
    // is presented below.
    if (refresh) {
        refresh_eyes_filtered();
        const int16_t
            right = eyes_reading_filtered[(eye_no_of_max_eye_filtered != 11) * (eye_no_of_max_eye_filtered + 1)], // (eye_no_of_max_eye == 11) ? 0 : (eye_no_of_max_eye + 1)
            left = eyes_reading_filtered[eye_no_of_max_eye_filtered ? (eye_no_of_max_eye_filtered - 1) : 11];     // (eye_no_of_max_eye == 0) ? 11 : (eye_no_of_max_eye - 1)
        const int16_t fake_dir_of_ball_filtered = 30 * eye_no_of_max_eye_filtered +
            15 // magnifies the difference of 2 eyes in fraction to +/- 15 degrees
            * (right - left) // this difference automatically turns the offset to -ive when the ball bears left, but +ive when the ball bears right of max_eye
            / ( eyes_reading_filtered[eye_no_of_max_eye_filtered] - ((left < right) ? left : right) ); // the difference of max and (lower one between left and right)
            dir_of_ball_filtered = (fake_dir_of_ball_filtered < 0) ? (fake_dir_of_ball_filtered + 360) : fake_dir_of_ball_filtered;
    }
    return dir_of_ball_filtered;
}

#endif // #ifndef NINGOR_IR_CPP
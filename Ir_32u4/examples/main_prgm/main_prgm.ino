#include <Wire.h>

#include <Ir_32u4.h>

void rx (int);
void tx ();

volatile uint8_t max_eye_idx;
volatile uint16_t ball_dir;
volatile uint8_t max_eye_val;

volatile uint8_t filter_off;

void setup () {
    max_eye_idx = 0;
    ball_dir = 361;
    filter_off = 0;
    yh::rec::ir_32u4s::begin();
    Wire.begin('I'); // 'i' == 73 in decimal (ASCII)
    Wire.onReceive(rx);
    Wire.onRequest(tx);
}

void loop () {
    if (filter_off) { // get sun intensity
        uint8_t eyes [IR_CHANNEL];
        uint8_t temp_max_eye_idx = 0, temp_max_eye_val = 0;
        for (uint8_t i = 0; i < IR_CHANNEL; i++) {
            const uint8_t temp_eye = eyes[i] = yh::rec::ir_32u4s::get_sun_intensity(i);
            if (temp_eye > temp_max_eye_val) {
                temp_max_eye_idx = i;
                temp_max_eye_val = temp_eye;
            }
        }
        max_eye_idx = temp_max_eye_idx;
        max_eye_val = temp_max_eye_val;
        const int16_t
            l_eye = eyes[temp_max_eye_idx ? (IR_CHANNEL - 1) : (temp_max_eye_idx - 1)],
            r_eye = eyes[(temp_max_eye_idx == (IR_CHANNEL - 1)) ? 0 : (temp_max_eye_idx + 1)];
        int16_t angle =
            static_cast<uint8_t>(360 / IR_CHANNEL) * temp_max_eye_idx + static_cast<int8_t>(360 / IR_CHANNEL / 2) * (r_eye - l_eye) / (temp_max_eye_val - (l_eye < r_eye ? l_eye : r_eye));
        if (angle < 0)
            angle += 360;
        const uint8_t oldSREG = SREG;
        noInterrupts();
        ball_dir = angle;
        SREG = oldSREG;
    } else { // filter the IR signals
        for (uint8_t i = 0; i < IR_CHANNEL; i++) {
            yh::rec::ir_32u4s::update_ball_intensity(i);
        }
        #define eyes yh::rec::ir_32u4s::ball_intensity
        uint8_t temp_max_eye_idx = 0, temp_max_eye_val = eyes[0];
        for (uint8_t i = 1; i < IR_CHANNEL; i++) {
            const uint8_t temp_eye = eyes[i];
            if (temp_eye > temp_max_eye_val) {
                temp_max_eye_idx = i;
                temp_max_eye_val = temp_eye;
            }
        }
        max_eye_idx = temp_max_eye_idx;
        max_eye_val = temp_max_eye_val;
        const int16_t
            l_eye = eyes[temp_max_eye_idx ? (IR_CHANNEL - 1) : (temp_max_eye_idx - 1)],
            r_eye = eyes[(temp_max_eye_idx == (IR_CHANNEL - 1)) ? 0 : (temp_max_eye_idx + 1)];
        int16_t angle =
            static_cast<uint8_t>(360 / IR_CHANNEL) * temp_max_eye_idx + static_cast<int8_t>(360 / IR_CHANNEL / 2) * (r_eye - l_eye) / (max_eye_val - (l_eye < r_eye ? l_eye : r_eye));
        if (angle < 0)
            angle += 360;
        const uint8_t oldSREG = SREG;
        noInterrupts();
        ball_dir = angle;
        SREG = oldSREG;
    }
}

void rx (int buf_len) { // be aware this function is called in an isr
    // 'S' == enter sun finding mode (no-filter)
    // else enter ball-filtering mode (default)
    uint8_t final_flag_state = 0;
    while (Wire.available()) {
        if (Wire.read() == 'S') {
            final_flag_state = 1;
        } else {
            final_flag_state = 0;
        }
    }
    filter_off = final_flag_state;
}

void tx () { // be aware this function is called in an isr
    // 3-byte package: ir-direction high-byte, ir-direction low-byte, ir-intensity
    const uint16_t temp_ball_dir = ball_dir;
    Wire.write(static_cast<uint8_t>(temp_ball_dir >> 8));
    Wire.write(static_cast<uint8_t>(temp_ball_dir));
    Wire.write(max_eye_val);
}
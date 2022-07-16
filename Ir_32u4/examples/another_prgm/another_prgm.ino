#include <Wire.h>

void rx (int);
void tx ();

#define IR_CHANNEL 12
#define LED_OUTPUT 8

volatile uint8_t filter_on;

volatile uint8_t idx_of_sun_max_eye;
volatile uint8_t idx_of_filtered_max_eye;
volatile uint8_t sun_eyes [IR_CHANNEL];
volatile uint8_t filtered_eyes [IR_CHANNEL];
volatile uint16_t sun_ball_dir;
volatile uint16_t filtered_ball_dir;

volatile char tx_data_special_request;

#define BALL_ANGLE 'A'
#define BALL_INTEN 'B'
#define SET_FILTER_OFF 'E'
#define SET_FILTER_ON 'F'
#define INDEX_OF_MAX_EYE 'I'
#define BALL_CHANNEL_0 'O'
#define BALL_CHANNEL_1 'P'
#define BALL_CHANNEL_2 'Q'
#define BALL_CHANNEL_3 'R'
#define BALL_CHANNEL_4 'S'
#define BALL_CHANNEL_5 'T'
#define BALL_CHANNEL_6 'U'
#define BALL_CHANNEL_7 'V'
#define BALL_CHANNEL_8 'W'
#define BALL_CHANNEL_9 'X'
#define BALL_CHANNEL_10 'Y'
#define BALL_CHANNEL_11 'Z'

#define TX_DATA_DEFAULT_REGISTER BALL_ANGLE

#if defined(LED_OUTPUT) && (LED_OUTPUT > 0)
void select_led (const uint8_t idx) {
    //
}
#endif // #if defined(LED_OUTPUT) && (LED_OUTPUT > 0)

// this array should be arranged in the clockwise order of the IR-photodiodes connected to the 32u4 adc channels
static const uint8_t channel_selecting_order [12] = {
    0, 1, 4, 5, 6, 7, 13, 12, 11, 10, 9, 8
};

void setup () {
    // set pinMode of analog pins to INPUT
    DDRF &= ~(_BV(0) | _BV(1) | _BV(4) | _BV(5) | _BV(6) | _BV(7));
    DDRD &= ~(_BV(4) | _BV(6) | _BV(7));
    DDRB &= ~(_BV(4) | _BV(5) | _BV(6));
    ADCSRA |= (1 << ADIF); // clear the interrupt flag first
    // 3 lines below: select ADC channel 0, disable ADC auto trigger mode, select prescaler 128, enable ADC, start first conversion
    ADCSRB = (0 << MUX5) | (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
    ADMUX = (0 << REFS1) | (0 << REFS0) | (1 << ADLAR) | (0 << MUX4) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    #if LED_OUTPUT == 8
    DDRC |= (_BV(6) | _BV(7));
    DDRB = (_BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(7));
    DDRE |= _BV(6);
    #endif
    filter_on = 1;
    tx_data_special_request = TX_DATA_DEFAULT_REGISTER;
    Wire.begin('I'); // 'i' == 73 in decimal (ASCII)
    Wire.onReceive(rx);
    Wire.onRequest(tx);
}

uint16_t find_ball_angle (const int16_t l_eye, const int16_t max_eye, const int16_t r_eye, const uint8_t idx_of_given_max_eye) {
    int16_t angle =
        static_cast<uint8_t>(360 / IR_CHANNEL) * idx_of_given_max_eye + static_cast<int8_t>(360 / IR_CHANNEL / 2) * (r_eye - l_eye) / (max_eye - (l_eye < r_eye ? l_eye : r_eye));
    if (angle < 0)
        angle += 360;
    return angle;
}

uint8_t ball_angle_to_led (uint16_t angle) {
    angle += static_cast<uint8_t>(360 / LED_OUTPUT / 2);
    if (angle >= static_cast<uint16_t>(360))
        angle -= static_cast<uint16_t>(360);
    uint8_t led_idx = 0;
    while (angle >= static_cast<uint8_t>(LED_OUTPUT)) {
        angle -= static_cast<uint8_t>(LED_OUTPUT);
        led_idx++;
    }
    return led_idx;
}

void loop () {
    // get the delta readings of all 12 eyes
    uint8_t max_filtered_eye_idx = 0, max_sun_eye_idx = 0;
    for (uint8_t eye_idx = 0; eye_idx < IR_CHANNEL; eye_idx++) {
        // select new channel
        const uint8_t adc_channel = channel_selecting_order[eye_idx];
        ADMUX = (ADMUX & ~((1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0))) | ((adc_channel & 0b111) << MUX0);
        (adc_channel & 0x08) ? (ADCSRB |= (1 << MUX5)) : (ADCSRB &= ~(1 << MUX5));

        // start new A/D conversion
        ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
        // wait for A/D conversion to complete
        while (ADCSRA & (1 << ADSC)) {}
        // get ADC result
        uint8_t result = ADCH;
        uint8_t max = result, min = result;
        for (uint8_t loop = 0; loop < 7; loop++) {
            // start new A/D conversion
            ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
            if (result > max)
                max = result;
            if (result < min)
                min = result;
            __asm__ __volatile__ ("nop"::);
            // wait for A/D conversion to complete
            while (ADCSRA & (1 << ADSC)) {}
            // get ADC result
            result = ADCH;
        }
        if (result > max)
            max = result;
        if (result < min)
            min = result;
        const uint8_t delta_of_eye = max - min;
        filtered_eyes[eye_idx] = delta_of_eye;
        if (delta_of_eye > filtered_eyes[max_filtered_eye_idx])
            max_filtered_eye_idx = eye_idx;
        sun_eyes[eye_idx] = max;
        if (max > sun_eyes[max_sun_eye_idx]) {
            max_sun_eye_idx = eye_idx;
        }
    }
    idx_of_filtered_max_eye = max_filtered_eye_idx;
    idx_of_sun_max_eye = max_sun_eye_idx;
    filtered_ball_dir = find_ball_angle(
        filtered_eyes[max_filtered_eye_idx ? (max_filtered_eye_idx - 1) : (IR_CHANNEL - 1)],
        filtered_eyes[max_filtered_eye_idx],
        filtered_eyes[max_filtered_eye_idx == (IR_CHANNEL - 1) ? 0 : (max_filtered_eye_idx + 1)],
        max_filtered_eye_idx
    );
    sun_ball_dir = find_ball_angle(
        sun_eyes[max_sun_eye_idx ? (max_sun_eye_idx - 1) : (IR_CHANNEL - 1)],
        sun_eyes[max_sun_eye_idx],
        sun_eyes[max_sun_eye_idx == (IR_CHANNEL - 1) ? 0 : (max_sun_eye_idx + 1)],
        max_sun_eye_idx
    );
    #if defined(LED_OUTPUT) && (LED_OUTPUT > 0)
    if (filter_on) {
        select_led(ball_angle_to_led(filtered_ball_dir));
    } else {
        select_led(ball_angle_to_led(sun_ball_dir));
    }
    #endif
}

void rx (int buf_len) {
    while (Wire.available()) {
        char buf = Wire.read();
        if (buf == SET_FILTER_OFF) {
            filter_on = 0;
        } else if (buf == SET_FILTER_ON) {
            filter_on = 1;
        } else {
            tx_data_special_request = buf;
        }
    }
}

void tx () {
    const uint8_t filter_on_flag = filter_on;
    if (filter_on_flag) {
        switch (tx_data_special_request) {
            case BALL_ANGLE:
                {
                    const uint16_t temp_ball_dir = filtered_ball_dir;
                    Wire.write(temp_ball_dir >> 8);
                    Wire.write(temp_ball_dir & 0xff);
                }
            case BALL_INTEN:
                Wire.write(filtered_eyes[idx_of_filtered_max_eye]);
            case INDEX_OF_MAX_EYE:
                Wire.write(idx_of_filtered_max_eye);
                break;

            #define STRUCTURE(c) \
            case BALL_CHANNEL_##c: Wire.write(filtered_eyes[c]);
            STRUCTURE(0)
            STRUCTURE(1)
            STRUCTURE(2)
            STRUCTURE(3)
            STRUCTURE(4)
            STRUCTURE(5)
            STRUCTURE(6)
            STRUCTURE(7)
            STRUCTURE(8)
            STRUCTURE(9)
            STRUCTURE(10)
            STRUCTURE(11)
            #undef STRUCTURE
                break;
        }
    } else {
        switch (tx_data_special_request) {
            case BALL_ANGLE:
                {
                    const uint16_t temp_ball_dir = sun_ball_dir;
                    Wire.write(temp_ball_dir >> 8);
                    Wire.write(temp_ball_dir & 0xff);
                }
            case BALL_INTEN:
                Wire.write(sun_eyes[idx_of_sun_max_eye]);
            case INDEX_OF_MAX_EYE:
                Wire.write(idx_of_sun_max_eye);
                break;

            #define STRUCTURE(c) \
            case BALL_CHANNEL_##c: Wire.write(sun_eyes[c]);
            STRUCTURE(0)
            STRUCTURE(1)
            STRUCTURE(2)
            STRUCTURE(3)
            STRUCTURE(4)
            STRUCTURE(5)
            STRUCTURE(6)
            STRUCTURE(7)
            STRUCTURE(8)
            STRUCTURE(9)
            STRUCTURE(10)
            STRUCTURE(11)
            #undef STRUCTURE
                break;
        }
    }
    tx_data_special_request = TX_DATA_DEFAULT_REGISTER;
}
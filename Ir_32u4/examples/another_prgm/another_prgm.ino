#include <Wire.h>

void rx (int);
void tx ();

#define IR_CHANNEL 12
#define LED_OUTPUT 8

volatile uint8_t idx_of_max_eye;
uint8_t eyes [IR_CHANNEL];
volatile uint16_t ball_dir;

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
    Wire.begin('I'); // 'i' == 73 in decimal (ASCII)
    Wire.onReceive(rx);
    Wire.onRequest(tx);
}

// this array should be arranged in the clockwise order of the IR-photodiodes connected to the 32u4 adc channels
static const uint8_t channel_selecting_order [12] = {
    0, 1, 4, 5, 6, 7, 8, 10, 11, 12, 13, 9
};

void loop () {
    // get the delta readings of all 12 eyes
    uint8_t max_eye_idx = 0;
    for (uint8_t eye_idx = 0; eye_idx < IR_CHANNEL; eye_idx++) {
        // select new channel
        const uint8_t adc_channel = channel_selecting_order[eye_idx];
        ADMUX = (ADMUX & ~((1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0))) | ((adc_channel & 0b111) << MUX0);
        #if defined(ADCSRB) && defined(MUX5)
        (adc_channel & 0x08) ? (ADCSRB |= (1 << MUX5)) : (ADCSRB &= ~(1 << MUX5));
        #endif // #if defined(ADCSRB) && defined(MUX5)

        uint8_t max = 0, min = 255;
        for (uint8_t loop = 0; loop < 8; loop++) {
            // start new A/D conversion
            ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
            // wait for A/D conversion to complete
            while (ADCSRA & (1 << ADSC)) {}
            // get ADC result
            const uint8_t result = ADCH;
            if (result > max)
                max = result;
            if (result < min)
                min = result;
        }
        const uint8_t delta_of_eye = max - min;
        eyes[eye_idx] = delta_of_eye;
        if (delta_of_eye > eyes[max_eye_idx])
            max_eye_idx = eye_idx;
    }
    idx_of_max_eye = max_eye_idx;
    const int16_t
        l_eye = eyes[max_eye_idx ? (IR_CHANNEL - 1) : (max_eye_idx - 1)],
        r_eye = eyes[(max_eye_idx == (IR_CHANNEL - 1)) ? 0 : (max_eye_idx + 1)];
    int16_t angle =
        static_cast<uint8_t>(360 / IR_CHANNEL) * max_eye_idx + static_cast<int8_t>(360 / IR_CHANNEL / 2) * (r_eye - l_eye) / (eyes[max_eye_idx] - (l_eye < r_eye ? l_eye : r_eye));
    if (angle < 0)
        angle += 360;
    const uint8_t oldSREG = SREG;
    noInterrupts();
    ball_dir = angle;
    SREG = oldSREG;
    #if defined(LED_OUTPUT) && (LED_OUTPUT > 0)
    uint16_t temp_ball_dir = ball_dir;
    temp_ball_dir += static_cast<uint8_t>(360 / LED_OUTPUT / 2);
    if (temp_ball_dir >= static_cast<uint16_t>(360))
        temp_ball_dir -= static_cast<uint16_t>(360);
    {
        uint8_t led_idx = 0;
        while (temp_ball_dir >= static_cast<uint8_t>(LED_OUTPUT)) {
            temp_ball_dir -= static_cast<uint8_t>(LED_OUTPUT);
            led_idx++;
        }
        led_idx;
    }
    #endif
}

enum tx_data_t {
    STD = 0, // standard 3-byte package: ir-direction high-byte, ir-direction low-byte, ir-intensity
    BALL_ANGLE, // 2-byte package: ir-direction high-byte, ir-direciton low-byte
    BALL_INTEN, // 1-byte package: ir-intensity
    MAX_EYE_IDX, // 1-byte package: index of max eye
    CHAN_READING // 1-byte package: ir-intensity of a single channel
};

uint8_t tx_data_special_request;

void rx (int buf_len) {
    if (Wire.available()) {
        switch (Wire.read()) {
            case 0:
                tx_data_special_request = STD;
                break;
            case 'A':
                tx_data_special_request = BALL_ANGLE;
                break;
            case 'B':
                tx_data_special_request = BALL_INTEN;
                break;
            case 'I':
                tx_data_special_request = MAX_EYE_IDX;
                break;
            case 'b':
                tx_data_special_request = CHAN_READING;
                if (Wire.available()) {
                    tx_data_special_request |= (Wire.read() << 4);
                }
                break;
            default:
                tx_data_special_request = STD;
                break;
        }
    }
    while (Wire.available()) { // clear the buffer
        Wire.read();
    }
}

void tx () {
    switch (tx_data_special_request) {
        case STD:
            // standard 3-byte package: ir-direction high-byte, ir-direction low-byte, ir-intensity
            const uint16_t temp_ball_dir = ball_dir;
            Wire.write(static_cast<uint8_t>(temp_ball_dir >> 8));
            Wire.write(static_cast<uint8_t>(temp_ball_dir & 0xff));
            Wire.write(eyes[idx_of_max_eye]);
            break;
        case BALL_ANGLE:
            const uint16_t temp_ball_dir = ball_dir;
            Wire.write(static_cast<uint8_t>(temp_ball_dir >> 8));
            Wire.write(static_cast<uint8_t>(temp_ball_dir & 0xff));
            break;
        case BALL_INTEN:
            Wire.write(eyes[idx_of_max_eye]);
            break;
        case MAX_EYE_IDX:
            Wire.write(idx_of_max_eye);
            break;
        // case CHAN_READING:
        default:
            const uint8_t request_channel = (tx_data_special_request >> 4);
            if (request_channel >= IR_CHANNEL) {
                uint8_t *eyes_ptr = (&(eyes[0]));
                for (uint8_t i = 0; i < IR_CHANNEL; i++) {
                    Wire.write(*eyes_ptr);
                    eyes_ptr++;
                }
            } else {
                Wire.write(eyes[request_channel]);
            }
            break;
    }
    tx_data_special_request = STD;
}
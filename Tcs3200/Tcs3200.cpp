#ifndef TCS3200_CPP
#define TCS3200_CPP __DATE__ ", " __TIME__

#include "Tcs3200.h"

yh::rec::Tcs3200::Tcs3200 (const Tcs3200 &init_obj) :
    s0_pin(init_obj.s0_pin),
    s1_pin(init_obj.s1_pin),
    s2_pin(init_obj.s2_pin),
    s3_pin(init_obj.s3_pin),
    out_pin(init_obj.out_pin)
{
    //
}

yh::rec::Tcs3200::Tcs3200 (const uint8_t init_s0, const uint8_t init_s1, const uint8_t init_s2, const uint8_t init_s3, const uint8_t init_out) :
    s0_pin(init_s0),
    s1_pin(init_s1),
    s2_pin(init_s2),
    s3_pin(init_s3),
    out_pin(init_out)
{
    //
}

void yh::rec::Tcs3200::begin () {
    pinMode(s0_pin, OUTPUT);
    pinMode(s1_pin, OUTPUT);
    pinMode(s2_pin, OUTPUT);
    pinMode(s3_pin, OUTPUT);
    pinMode(out_pin, INPUT);
}

void yh::rec::Tcs3200::select_frequency (const uint8_t freq) {
    switch (freq) {
        case 2:
            digitalWrite(s0_pin, LOW);
            digitalWrite(s1_pin, HIGH);
            break;
        case 20:
            digitalWrite(s0_pin, HIGH);
            digitalWrite(s1_pin, LOW);
            break;
        case 100:
            digitalWrite(s0_pin, HIGH);
            digitalWrite(s1_pin, HIGH);
            break;
        case 0:
        default:
            digitalWrite(s0_pin, LOW);
            digitalWrite(s1_pin, LOW);
            break;
    }
}

void yh::rec::Tcs3200::select_filter (const filter_t colour) {
    digitalWrite(s2_pin, ((colour & 0x02) ? HIGH : LOW));
    digitalWrite(s3_pin, ((colour & 0x01) ? HIGH : LOW));
}

uint16_t yh::rec::Tcs3200::read_single_colour () {
    uint16_t time = pulseIn(out_pin, ((digitalRead(s0_pin) == HIGH) ? LOW : HIGH), 2000);
    if (time == 0)
        time = 900;
    return constrain(map(time, 900, 100, 0, 255), 0, 255);
}

void yh::rec::Tcs3200::read_rgb (uint8_t *const red, uint8_t *const green, uint8_t *const blue) {
    digitalWrite(s2_pin, LOW);
    digitalWrite(s3_pin, LOW);
    if (red) (*red) = read_single_colour();
    digitalWrite(s3_pin, HIGH);
    delayMicroseconds(100);
    if (blue) (*blue) = read_single_colour();
    digitalWrite(s2_pin, HIGH);
    delayMicroseconds(100);
    if (green) (*green) = read_single_colour();
}

void yh::rec::Tcs3200::read_hsv (uint16_t *const hue, uint8_t *const sat, uint8_t *const val) {
    uint8_t rgb [3];
    read_rgb(&(rgb[0]), &(rgb[1]), &(rgb[2]));

    int16_t hue_temp; uint8_t sat_temp, val_temp; // hue == 120 * dom_col + (val[dom+1] - val[dom-1]) / delta, sat = 255 * delta / max, val = max

    uint8_t max = 0, min = 0;

    if (rgb[1] > rgb[0])
        max = 1;
    if (rgb[2] > rgb[max])
        max = 2;

    if (rgb[1] < rgb[0])
        min = 1;
    if (rgb[2] < rgb[min])
        min = 2;
    
    val_temp = rgb[max];
    const uint8_t delta = val_temp - rgb[min];
    if (delta == 0) { // if val_temp == 0, delta == 0
        hue_temp = 0; // nan as it is black or grey
        sat_temp = 0;
    } else {
        hue_temp = 120 * max + (static_cast<int16_t>(rgb[max == 2 ? 0 : max + 1]) - static_cast<int16_t>(rgb[max ? max - 1 : 2])) / delta;
        sat_temp = 255 * delta / val_temp;
    }
    if (hue) (*hue) = ((hue_temp < 0) ? (hue_temp + 360) : hue_temp);
    if (sat) (*sat) = sat_temp;
    if (val) (*val) = val_temp;
}

// if s < 10 && v > 150
bool yh::rec::Tcs3200::seeing_white () {
    uint8_t sat, val;
    read_hsv(NULL, &sat, &val);
    return (sat < 10) && (val > 150);
}

#endif // #ifndef TCS3200_CPP
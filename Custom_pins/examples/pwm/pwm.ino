#include <Custom_pins.h>

void setup () {
    pinMode(11, OUTPUT);
}

void loop () {
    static uint8_t val = 0;
    val += 5;
    if (val > 250) val = 0;
    analogWrite(11, val);
    delay(50);
}
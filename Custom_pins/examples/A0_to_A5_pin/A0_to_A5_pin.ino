#include <Custom_pins.h>

void setup() {
    yh::rec::Custom_pins::begin_analog_read();
    Serial.begin(9600);
}

void loop() {
    Serial.print(yh::rec::Custom_pins::analog_read(A0));
    Serial.print('\t');
    Serial.print(yh::rec::Custom_pins::analog_read(A1));
    Serial.print('\t');
    Serial.print(yh::rec::Custom_pins::analog_read(A2));
    Serial.print('\t');
    Serial.print(yh::rec::Custom_pins::analog_read(A3));
    Serial.print('\t');
    Serial.print(yh::rec::Custom_pins::analog_read(A4));
    Serial.print('\t');
    Serial.print(yh::rec::Custom_pins::analog_read(A5));
    Serial.print('\n');
}
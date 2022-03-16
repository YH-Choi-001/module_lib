#include <Custom_pins.h>

void setup() {
    yh::rec::custom_pins::begin_analog_read();
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    Serial.begin(9600);
}

void loop() {
    Serial.print(analogRead(A0));
    Serial.print('\t');
    Serial.print(analogRead(A1));
    Serial.print('\t');
    Serial.print(analogRead(A2));
    Serial.print('\t');
    Serial.print(analogRead(A3));
    Serial.print('\t');
    Serial.print(analogRead(A4));
    Serial.print('\t');
    Serial.print(analogRead(A5));
    Serial.print('\n');
}
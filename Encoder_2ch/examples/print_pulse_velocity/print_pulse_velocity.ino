#include <Btn7971b.h>
#include <Encoder_2ch.h>

yh::rec::Btn7971b motor (5, 13);
yh::rec::Encoder_2ch_pulse encoder (A0, A1);

void setup () {
    Serial.begin(9600);
    encoder.begin();
    motor.begin();
    motor.set_spd(80);
}

void loop () {
    // Serial.println(encoder.get_spd_simple());
    Serial.println(encoder.get_spd_mean());
}
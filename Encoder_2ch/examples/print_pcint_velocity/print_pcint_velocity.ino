#include <Encoder_2ch.h>
// #include <Timer_int.h>
#include <Pcint.h>

yh::rec::Encoder_2ch_ext_int encoder_L (8, 9);
yh::rec::Encoder_2ch_ext_int encoder_R (A0, A1);

const int L_spd_pin = 10;
const int R_spd_pin = 11;
const int L_dir_pin = 12;
const int R_dir_pin = 13;

double L_spd;
double R_spd;

void set_L_spd (const int16_t spd) {
    digitalWrite(L_dir_pin, spd > 0 ? HIGH : LOW);
    analogWrite(L_spd_pin, abs(constrain(spd, -255, 255)));
}

void set_R_spd (const int16_t spd) {
    digitalWrite(R_dir_pin, spd > 0 ? HIGH : LOW);
    analogWrite(R_spd_pin, abs(constrain(spd, -255, 255)));
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    // setup_timer_3A_interrupt(84);
    L_spd = 100;
    R_spd = 100;
    set_L_spd(L_spd);
    set_R_spd(R_spd);
    enable_pcint_on_specific_pin(8);
    enable_pcint_on_specific_pin(9);
    enable_pcint_on_specific_pin(A0);
    enable_pcint_on_specific_pin(A1);
    encoder_L.begin();
    encoder_R.begin();
}

ISR (PCINT0_vect) {
    encoder_L.isr_individual_sensor_routine();
}

ISR (PCINT1_vect) {
    encoder_R.isr_individual_sensor_routine();
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.print("Left velocity: ");
    Serial.print(encoder_L.get_instantaneous_velocity());
    Serial.print("\tRight velocity: ");
    Serial.print(encoder_R.get_instantaneous_velocity());
    Serial.print('\n');
}
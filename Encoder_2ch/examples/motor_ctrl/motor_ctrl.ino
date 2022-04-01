#include <pid.h>
#include <Encoder_2ch.h>
#include <Timer_int.h>

yh::rec::Encoder_2ch encoder_L (2, 5, 600);
yh::rec::Encoder_2ch encoder_R (3, 6, 600);

yh::rec::Pid pid_L (1, 0, 0);
yh::rec::Pid pid_R (1, 0, 0);

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
    Serial.begin(9600);
    setup_timer_3A_interrupt(84);
    L_spd = 100;
    R_spd = 100;
    set_L_spd(L_spd);
    set_R_spd(R_spd);
}

ISR (TIMER3_COMPA_vect) {
    encoder_L.isr_individual_sensor_routine();
    encoder_R.isr_individual_sensor_routine();
}

void loop() {
    // put your main code here, to run repeatedly:
    // Serial.println(encoder_L.get_instantaneous_velocity());
    // Serial.println(encoder_R.get_instantaneous_velocity());
    static unsigned long prev_time = micros();
    const unsigned long time_diff = micros() - prev_time;

    // L_spd += pid_L.update_corr(100, encoder_L.get_instantaneous_velocity(), time_diff);
    // R_spd += pid_R.update_corr(100, encoder_R.get_instantaneous_velocity(), time_diff);

    const long L_actual_spd = encoder_L.get_instantaneous_velocity();
    const long R_actual_spd = encoder_R.get_instantaneous_velocity();
    if (L_actual_spd > 108) {
        L_spd -= 0.003;
    } else if (L_actual_spd < 92) {
        L_spd += 0.003;
    }
    if (R_actual_spd > 108) {
        R_spd -= 0.003;
    } else if (R_actual_spd < 92) {
        R_spd += 0.003;
    }

    set_L_spd(L_spd);
    set_R_spd(R_spd);
    // prev_time += time_diff;
}
#include <Wire.h>

#include <Btn7971b.h>
#include <Encoder_2ch.h>
#include <Pcint.h>

yh::rec::Btn7971b motors [4] = {
    yh::rec::Btn7971b(3, 2),
    yh::rec::Btn7971b(5, 10),
    yh::rec::Btn7971b(6, 12),
    yh::rec::Btn7971b(11, 13)
};

yh::rec::Encoder_2ch_ext_int encoders [4] = {
    yh::rec::Encoder_2ch_ext_int(A2, A3),
    yh::rec::Encoder_2ch_ext_int(4, 7),
    yh::rec::Encoder_2ch_ext_int(8, 9),
    yh::rec::Encoder_2ch_ext_int(A0, A1)
};

ISR(PCINT0_vect) {
    encoders[2].isr_individual_sensor_routine();
}

ISR(PCINT1_vect) {
    encoders[0].isr_individual_sensor_routine();
    encoders[3].isr_individual_sensor_routine();
}

ISR(PCINT2_vect) {
    encoders[1].isr_individual_sensor_routine();
}

void rx (int);

void setup () {
    enable_pcint_on_specific_pin(A2);
    enable_pcint_on_specific_pin(A3);
    enable_pcint_on_specific_pin(4);
    enable_pcint_on_specific_pin(7);
    enable_pcint_on_specific_pin(8);
    enable_pcint_on_specific_pin(9);
    enable_pcint_on_specific_pin(A0);
    enable_pcint_on_specific_pin(A1);
    motors[0].set_spd(100);
    motors[2].set_spd(-100);
    // Wire.begin('M'); // 'M' == 77 in decimal (ASCII)
    // Wire.onReceive(rx);




    Serial.begin(9600);
}

void loop () {
    Serial.print(encoders[0].get_instantaneous_velocity());
    Serial.print('\t');
    Serial.print(encoders[2].get_instantaneous_velocity());
    Serial.print('\n');
}

// void rx (int len) {
//     while (Wire.available()) {
//         const uint8_t val = Wire.read();
//     }
// }
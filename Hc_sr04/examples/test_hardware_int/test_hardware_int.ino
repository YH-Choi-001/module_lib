// Please do the following:
// connect Vcc on HC-SR04 to 5V on arduino
// connect Gnd on HC-SR04 to GND on arduino
// connect trig pin on HC-SR04 to pin 2 on arduino
// connect echo pin on HC-SR04 to pin 3 on arduino

#include <Hc_sr04.h>

yh::rec::Hc_sr04_ext_int uts (2, 3);

void pin3_change_isr () {
    uts.isr_individual_sensor_routine();
}

void setup () {
    Serial.begin(9600);
    uts.begin();
    attachInterrupt(digitalPinToInterrupt(3), pin3_change_isr, CHANGE);
}

void loop () {
    Serial.println(uts.polling_update());
}
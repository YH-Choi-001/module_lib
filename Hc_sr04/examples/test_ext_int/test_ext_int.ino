#include <Hc_sr04.h>

yh::rec::Hc_sr04_ext_int uts (2, 3);

void pin3_fall_isr () {
    uts.isr_individual_sensor_routine();
}

void setup () {
    Serial.begin(9600);
    uts.begin();
    attachInterrupt(digitalPinToInterrupt(3), pin3_fall_isr, FALLING);
}

void loop () {
    Serial.println(uts.polling_update());
}
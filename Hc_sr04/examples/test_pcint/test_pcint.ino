// Please do the following:
// connect Vcc on HC-SR04 to 5V on arduino
// connect Gnd on HC-SR04 to GND on arduino
// connect trig pin on HC-SR04 to pin 2 on arduino
// connect echo pin on HC-SR04 to pin 3 on arduino

#include <Hc_sr04.h>

yh::rec::Hc_sr04_ext_int uts (2, 3);

ISR(PCINT2_vect) {
    uts.isr_individual_sensor_routine();
}

void setup_pcint (const uint8_t pin) {
    *digitalPinToPCMSK(pin) |= (1 << digitalPinToPCMSKbit(pin));  // enable pin
    // the respective bit of PCIFR register is set to 1 when a pin in the port changes
    PCICR |= (1 << digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

void setup () {
    Serial.begin(9600);
    uts.begin();
    setup_pcint(3);
}

void loop () {
    Serial.println(uts.polling_update());
}
// Please do the following:
// connect Vcc on HC-SR04 to 5V on arduino
// connect Gnd on HC-SR04 to GND on arduino
// connect trig pin on HC-SR04 to pin 4 on arduino
// connect echo pin on HC-SR04 to pin 5 on arduino

#include <Hc_sr04.h>

yh::rec::Hc_sr04_fast hc_sr04 (4, 5);

void setup () {
    Serial.begin(9600);
    hc_sr04.begin();
}

void loop () {
    Serial.print("distance read: ");
    Serial.print(hc_sr04.read_dist_cm());
    Serial.println(" cm");
}
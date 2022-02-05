// Please do the following:
// connect Vcc on US-016 to 5V on arduino
// connect Gnd on US-016 to GND on arduino
// connect range pin on US-016 to pin 52 on arduino
// connect out pin on US-016 to pin A15 on arduino

#include <Us_016.h>

yh::rec::Us_016_fast us_016 (52, A15);

void setup () {
    Serial.begin(9600);
    us_016.begin();
}

void loop () {
    Serial.print("distance read: ");
    Serial.print(us_016.read_dist_cm());
    Serial.println(" cm");
}
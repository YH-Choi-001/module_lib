// Please do the following:
// connect the Vcc of nin gor's ir to 5V on arduino
// connect the Gnd of nin gor's ir to GND on arduino
// connect the analog signal to A0 on arduino

#include <Sen_1930.h>

yh::rec::Sen_1930_fast grayscale (A0);

void setup () {
    Serial.begin(9600);
    grayscale.begin();
    grayscale.set_lower_limit_of_white(700);
}

void loop () {

    // read the brightness of the ground using raw_read_gry_value() method
    Serial.print("brightness: ");
    Serial.print(grayscale.raw_read_gry_value());

    // check if touching the white line using touch_white() method
    Serial.print(", max eye: ");
    Serial.print(grayscale.touch_white());
}
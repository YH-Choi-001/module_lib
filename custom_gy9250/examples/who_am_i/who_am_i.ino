// Please do the following:
// connect Vcc on GY-521 to 3.3V on arduino
// connect Gnd on GY-521 to GND on arduino
// connect SDA on GY-521 to SDA on arduino
// connect SCL on GY-521 to SCL on arduino

#include <custom_gy521.h>

Custom_gy521 gy521 (0x68); // change argument to 0x69 if AD0 is HIGH

void setup () {

    // sets baud rate to 9600
    Serial.begin(9600);

    // sets the GY-521 chip to desired settings
    gy521.begin();

}

void loop () {

    Serial.print("0x");
    Serial.println(gy521.who_am_i(), HEX);

    delay(3000);

}
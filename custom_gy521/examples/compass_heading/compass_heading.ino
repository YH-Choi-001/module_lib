// Please do the following:
// connect Vcc on GY-521 to 3.3V on arduino
// connect Gnd on GY-521 to GND on arduino
// connect SDA on GY-521 to SDA on arduino
// connect SCL on GY-521 to SCL on arduino
// connect signal pin of button to pin 3 on arduino

#include <custom_gy521.h>

Custom_gy521 gy521 (0x68); // change argument to 0x69 if AD0 is HIGH

const uint8_t reset_compass_button_pin = 3;

void setup () {

    // sets baud rate to 9600
    Serial.begin(9600);

    // sets the GY-521 chip to desired settings
    gy521.begin();

}

void loop () {

    // prints out the current heading of the chip
    Serial.println(gy521.update_yaw());

    // the compass reset button is pressed, reset the current heading to 0
    if (digitalRead(reset_compass_button_pin)) gy521.yaw = 0;

}
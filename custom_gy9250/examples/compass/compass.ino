// Please do the following:
// connect Vcc on GY-9250 to 3.3V on arduino
// connect Gnd on GY-9250 to GND on arduino
// connect SDA on GY-9250 to SDA on arduino
// connect SCL on GY-9250 to SCL on arduino

#include <custom_gy9250.h>

Custom_gy9250 gy9250 (0x68); // change argument to 0x69 if AD0 is HIGH

// optional: connect a signal pin of a button to pin 4
const uint8_t button_pin = 4;

void setup () {

    // set the pinMode of button to INPUT
    pinMode(button_pin, INPUT);

    // sets baud rate to 9600
    Serial.begin(9600);

    // sets the GY-9250 chip to desired settings
    gy9250.begin();

    // you must call this function to enable AK8963 magnetometer inside the MPU-9250 chip
    gy9250.enable_ext_i2c_slave_sensors();

    // sets the AK8963 magnetometer to desired settings
    gy9250.mag.begin();
    for (uint16_t i = 0; i < 2000; i++) {
        gy9250.mag.single_calibrate();
        Serial.print(i/20);
        Serial.println('%');
    }
}

void loop () {

    // prints out the heading of the chip
    Serial.println(gy9250.mag.get_heading());
    // if button is pressed, reset the heading of the chip
    if (digitalRead(button_pin) == HIGH) gy9250.mag.reset_heading();

}
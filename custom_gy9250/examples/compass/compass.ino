// Please do the following:
// connect Vcc on GY-9250 to 3.3V on arduino
// connect Gnd on GY-9250 to GND on arduino
// connect SDA on GY-9250 to SDA on arduino
// connect SCL on GY-9250 to SCL on arduino

#include <custom_gy9250.h>

Custom_gy9250 gy9250 (0x68); // change argument to 0x69 if AD0 is HIGH

void setup () {

    // sets baud rate to 9600
    Serial.begin(9600);

    // sets the GY-9250 chip to desired settings
    gy9250.begin();
    gy9250.enable_ext_i2c_slave_sensors();
    gy9250.mag.begin();
 }

void loop () {

    // prints out the atan2 of y, x of the chip
    gy9250.mag.update_adjusted();
    Serial.println(atan2(gy9250.mag.adj_y, gy9250.mag.adj_x) / M_PI * 180);

}
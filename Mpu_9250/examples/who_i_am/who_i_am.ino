// Please do the following:
// connect Vcc on GY-9250 to 3.3V on arduino
// connect Gnd on GY-9250 to GND on arduino
// connect SDA on GY-9250 to SDA on arduino
// connect SCL on GY-9250 to SCL on arduino

#include <Mpu_9250.h>

yh::rec::Mpu_9250 gy9250 (0x68); // change argument to 0x69 if AD0 is HIGH

void setup () {

    // sets baud rate to 9600
    Serial.begin(9600);

    // sets the GY-9250 chip to desired settings
    gy9250.begin();

    // you must call this function to enable AK8963 magnetometer inside the MPU-9250 chip
    gy9250.enable_ext_i2c_slave_sensors();

    // sets the AK8963 magnetometer to desired settings
    gy9250.mag.begin();

}

void loop () {

    Serial.print("0x");
    Serial.println(gy9250.mag.who_i_am(), HEX);

    delay(3000);

}
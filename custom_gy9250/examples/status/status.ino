// Please do the following:
// connect Vcc on GY-9250 to 3.3V on arduino
// connect Gnd on GY-9250 to GND on arduino
// connect SDA on GY-9250 to SDA on arduino
// connect SCL on GY-9250 to SCL on arduino

#include <custom_gy9250.h>

Custom_gy9250 gy9250 (0x68); // change argument to 0x69 if AD0 is HIGH

void update_progress () {
    static uint16_t progress = 0;
    Serial.println((progress++) / 400.0);
}

void setup () {

    // sets baud rate to 9600
    Serial.begin(9600);

    // sets the GY-9250 chip to desired settings
    gy9250.begin();
    gy9250.enable_ext_i2c_slave_sensors();
    gy9250.mag.begin();

    // calibrate the gyroscope
    gy9250.cal_gyro(400, update_progress);

 }

void loop () {

    // prints out the current values of the chip
    gy9250.mag.update_adjusted();
    Serial.print(gy9250.mag.adj_x);
    Serial.print('\t');
    Serial.print(gy9250.mag.adj_y);
    Serial.print('\t');
    Serial.print(gy9250.mag.adj_z);
    Serial.print('\n');

}
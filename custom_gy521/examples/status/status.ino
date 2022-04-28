// Please do the following:
// connect Vcc on GY-521 to 3.3V on arduino
// connect Gnd on GY-521 to GND on arduino
// connect SDA on GY-521 to SDA on arduino
// connect SCL on GY-521 to SCL on arduino

#include <custom_gy521.h>

Custom_gy521 gy521 (0x68); // change argument to 0x69 if AD0 is HIGH

void update_progress () {
    static uint16_t progress = 0;
    Serial.println((progress++) / 400.0);
}

void setup () {

    // sets baud rate to 9600
    Serial.begin(9600);

    // sets the GY-521 chip to desired settings
    gy521.begin();

    // calibrate the gyroscope
    gy521.cal_gyro(400, update_progress);

}

void loop () {

    // prints out the current heading of the chip
    // calls Custom_gy521::update_gyro() method without arguments
    // in order to use calibrated values for readings from Custom_gy521.cal_gyro()
    gy521.update_gyro();
    Serial.print(gy521.d_roll);
    Serial.print('\t');
    Serial.print(gy521.d_pitch);
    Serial.print('\t');
    Serial.print(gy521.d_yaw);
    Serial.print("\t\t\t");
    Serial.print(gy521.get_roll());
    Serial.print('\t');
    Serial.print(gy521.get_pitch());
    Serial.print('\t');
    Serial.print(gy521.get_yaw());
    Serial.print('\n');

}
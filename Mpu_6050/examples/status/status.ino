// Please do the following:
// connect Vcc on GY-521 to 3.3V on arduino
// connect Gnd on GY-521 to GND on arduino
// connect SDA on GY-521 to SDA on arduino
// connect SCL on GY-521 to SCL on arduino

#include <Mpu_6050.h>

yh::rec::Mpu_6050 mpu (0x68); // change argument to 0x69 if AD0 is HIGH

void update_progress () {
    static uint16_t progress = 0;
    Serial.println((progress++) / 400.0);
}

void setup () {

    // sets baud rate to 9600
    Serial.begin(9600);

    // sets the GY-521 chip to desired settings
    mpu.begin();

    // calibrate the gyroscope
    mpu.cal_gyro(400, update_progress);

}

void loop () {

    // prints out the current heading of the chip
    // calls Custom_gy521::update_gyro() method without arguments
    // in order to use calibrated values for readings from Custom_gy521.cal_gyro()
    mpu.update_gyro();
    Serial.print(mpu.d_roll);
    Serial.print('\t');
    Serial.print(mpu.d_pitch);
    Serial.print('\t');
    Serial.print(mpu.d_yaw);
    Serial.print("\t\t\t");
    Serial.print(mpu.get_roll());
    Serial.print('\t');
    Serial.print(mpu.get_pitch());
    Serial.print('\t');
    Serial.print(mpu.get_yaw());
    Serial.print('\n');

}
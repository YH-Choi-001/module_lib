// Please do the following:
// connect Vcc     on GY-6500 to 3.3V on arduino
// connect Gnd     on GY-6500 to GND  on arduino
// connect SDA/SDI on GY-6500 to MOSI on arduino
// connect SCL/SCK on GY-6500 to SCK  on arduino
// connect AD0/SDO on GY-6500 to MISO on arduino
// connect nCS     on GY-6500 to pin 10 on arduino

#include <Mpu_6500.h>

yh::rec::Mpu_6500_default_SPI mpu (10); // input the CS pin

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

    // mpu.update_gyro_isr() takes 24 - 28 µs
    // mpu.update_gyro(mpu.update_gyro_isr()) takes 1400 - 1600 µs

    // update the gyroscope
    mpu.update_gyro(mpu.update_gyro_isr());
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
    // prints out the current heading of the chip
    Serial.print(mpu.get_yaw());
    Serial.print('\n');

}
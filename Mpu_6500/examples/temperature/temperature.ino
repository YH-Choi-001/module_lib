// Please do the following:
// connect Vcc     on GY-6500 to 3.3V on arduino
// connect Gnd     on GY-6500 to GND  on arduino
// connect SDA/SDI on GY-6500 to MOSI on arduino
// connect SCL/SCK on GY-6500 to SCK  on arduino
// connect AD0/SDO on GY-6500 to MISO on arduino
// connect nCS     on GY-6500 to pin 10 on arduino

#include <Mpu_6500.h>

yh::rec::Mpu_6500_default_SPI mpu (10); // input the CS pin

void setup () {

    // sets baud rate to 9600
    Serial.begin(9600);

    // sets the GY-521 chip to desired settings
    mpu.begin();

}

void loop () {

    Serial.println(mpu.get_temp());

}
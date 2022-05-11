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

    //
    Serial.println("We are going to generate the calibrated values for your gyroscope.");
    Serial.println("Please be aware that every chip has different calibrated values.");
    Serial.println("You should calibrate every chip before usage, otherwise you will get unusable values.\n");
    delay(5000);
    Serial.println("Now, please lay down and rest your gy521 / gy6500 / gy9250 chip.\n");
    delay(3000);
    Serial.println("Please make sure no one nearby is going to touch the chip, or even just kick or knock the table.\n");
    delay(3000);
    Serial.println("Please send something through the serial monitor after you have setup the environment for calibration mentioned above.\n");

    while (Serial.available() < 1) {
        // wait
        delayMicroseconds(1000);
    }

    Serial.println("Calibration is going to begin.");
    Serial.println("It may take around 6 minutes and 40 seconds.");
    Serial.print('\n');

    // calibrate the gyroscope
    mpu.cal_gyro(0xfffff);

    Serial.println("Calibration is completed. You may replace gy521.cal_gyro() with the code below.");
    Serial.println("After pasting these code into your own program, you don't need to call gy521.cal_gyro() method in void setup() anymore.\n");
    Serial.print("gy521.corr_roll = ");
    Serial.print(mpu.corr_roll, 20);
    Serial.println(";");
    Serial.print("gy521.corr_pitch = ");
    Serial.print(mpu.corr_pitch, 20);
    Serial.println(";");
    Serial.print("gy521.corr_yaw = ");
    Serial.print(mpu.corr_yaw, 20);
    Serial.println(";\n\n\n\n\n");
}

void loop () {}
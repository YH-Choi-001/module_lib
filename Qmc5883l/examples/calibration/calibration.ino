#include <Qmc5883l.h>

yh::rec::Qmc5883l compass;

void setup () {
    // begin using Serial
    Serial.begin(9600);

    // begin using the compass
    compass.begin();

    Serial.print("Calibration will begin in 5 seconds.\n");
    Serial.print("Rotate the compass along the z-axis.\n");
    delay(1000);
    Serial.print("4\n");
    delay(1000);
    Serial.print("3\n");
    delay(1000);
    Serial.print("2\n");
    delay(1000);
    Serial.print("1\n");
    delay(1000);
    Serial.print("Calibration now begins.\n");

    // sample some data
    compass.compass_cal();

    Serial.print("Calibration completed.\n");
    Serial.print("Copy the code below and paste it in your void setup() function.\n");

    Serial.print("    compass.x_range = ");
    Serial.print(compass.x_range);
    Serial.print(";\n");

    Serial.print("    compass.x_min = ");
    Serial.print(compass.x_min);
    Serial.print(";\n");

    Serial.print("    compass.y_range = ");
    Serial.print(compass.y_range);
    Serial.print(";\n");

    Serial.print("    compass.y_min = ");
    Serial.print(compass.y_min);
    Serial.print(";\n");

    Serial.print("    compass.z_range = ");
    Serial.print(compass.z_range);
    Serial.print(";\n");

    Serial.print("    compass.z_min = ");
    Serial.print(compass.z_min);
    Serial.print(";\n");
}

void loop () {
    delayMicroseconds(16);
}
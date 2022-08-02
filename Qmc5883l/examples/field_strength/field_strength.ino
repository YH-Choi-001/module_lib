#include <Qmc5883l.h>

yh::rec::Qmc5883l compass;

void setup () {
    // begin using Serial
    Serial.begin(9600);

    // begin using the compass
    compass.begin();
}

void loop () {
    // update the compass
    compass.update();

    // print the field strength of x-axis in uT
    Serial.print("x: ");
    Serial.print(compass.get_uT_x());
    // print the field strength of y-axis in uT
    Serial.print("\ty: ");
    Serial.print(compass.get_uT_y());
    // print the field strength of z-axis in uT
    Serial.print("\tz: ");
    Serial.print(compass.get_uT_z());
    Serial.print('\n');
}
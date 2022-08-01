#include <Qmc5883l.h>

yh::rec::Qmc5883l compass;

void setup () {
    // begin using Serial
    Serial.begin(9600);

    // begin using the compass
    compass.begin();

    // paste the calibrated values extracted from calibration example here
    compass.x_range = 0;
    compass.x_min = 0;
    compass.y_range = 0;
    compass.y_min = 0;

}

void loop () {
    // print the current heading of the compass
    Serial.println(compass.get_heading());

    if (Serial.available()) { // if the computer sends something to the Arduino,
        // take the current heading of compass as 0 (North)
        compass.reset_heading();

        // empty the Serial receiving buffer
        Serial.read();
    }
}
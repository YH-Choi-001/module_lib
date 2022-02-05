// Please do the following:
// connect one pwm input on Btn7971b to pin 4 on arduino
// connect the other pwm input on Btn7971b to pin 5 on arduino

#include <Btn7971b.h>

yh::rec::Btn7971b motor(4, 5);

void setup () {
    Serial.begin(9600);
    motor.begin();
}

void loop () {

    // a flag that indicates whether the motor should increase speed or decrease speed
    static bool increasing_trend = true;

    Serial.print("current speed: ");

    if (increasing_trend) {
        if (motor.get_spd() < 255) {
            Serial.println(motor += 5);
        } else {
            increasing_trend = false;
        }
    } else {
        if (motor.get_spd() > -255) {
            Serial.println(motor -= 5);
        } else {
            increasing_trend = true;
        }
    }
}
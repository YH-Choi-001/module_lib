// Please do the following:
// connect the IN1 pin on Btn7971b to pin 6 on arduino
// connect the IN2 pin on Btn7971b to pin 7 on arduino

#include <Btn7971b.h>

// first pin must support PWM, second pin need not support PWM
yh::rec::Btn7971b motor(6, 7);

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
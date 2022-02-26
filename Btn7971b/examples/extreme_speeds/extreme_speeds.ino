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

    // extreme of positive speed
    motor.set_spd(255);
    Serial.print("current speed: ");
    Serial.println(motor.get_spd());
    delay(2000);

    // stop
    motor.set_spd(0);
    Serial.print("current speed: ");
    Serial.println(motor.get_spd());
    delay(2000);

    // extreme of negative speed
    motor.set_spd(-255);
    Serial.print("current speed: ");
    Serial.println(motor.get_spd());
    delay(2000);

    // stop
    motor.set_spd(0);
    Serial.print("current speed: ");
    Serial.println(motor.get_spd());
    delay(2000);
}
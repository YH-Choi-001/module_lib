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
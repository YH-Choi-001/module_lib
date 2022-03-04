#ifndef BTN7971B_CPP
#define BTN7971B_CPP __DATE__ ", " __TIME__

#include "Btn7971b.h"

yh::rec::Btn7971b::Btn7971b (const Btn7971b &init_obj) :
    pwm_pin(init_obj.pwm_pin), dir_pin(init_obj.dir_pin), speed(0), slowest_spd(-255), fastest_spd(255)
{
    pwm_pin_mask = digitalPinToBitMask(pwm_pin);
    pwm_pin_output_register = portOutputRegister(digitalPinToPort(pwm_pin));
    pwm_pin_timer = digitalPinToTimer(pwm_pin);
    dir_pin_mask = digitalPinToBitMask(dir_pin);
    dir_pin_output_register = portOutputRegister(digitalPinToPort(dir_pin));
}

yh::rec::Btn7971b::Btn7971b (const uint8_t init_pwm_pin, const uint8_t init_dir_pin) :
    pwm_pin(init_pwm_pin), dir_pin(init_dir_pin), speed(0), slowest_spd(-255), fastest_spd(255)
{
    pwm_pin_mask = digitalPinToBitMask(pwm_pin);
    pwm_pin_output_register = portOutputRegister(digitalPinToPort(pwm_pin));
    pwm_pin_timer = digitalPinToTimer(pwm_pin);
    dir_pin_mask = digitalPinToBitMask(dir_pin);
    dir_pin_output_register = portOutputRegister(digitalPinToPort(dir_pin));
}

void yh::rec::Btn7971b::begin () {
    pinMode(pwm_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    (*pwm_pin_output_register) &= (~pwm_pin_mask); // this line replaces digitalWrite(pwm_pin, LOW);
    (*dir_pin_output_register) &= (~dir_pin_mask); // this line replaces digitalWrite(dir_pin, LOW);
}

void yh::rec::Btn7971b::set_spd (const int16_t input_speed) {
    speed = constrain(input_speed, slowest_spd, fastest_spd);
    const bool dir_pin_state = (input_speed < 0);
    // the line below replaces digitalWrite(dir_pin, dir_pin_state);
    dir_pin_state ? ((*dir_pin_output_register) |= dir_pin_mask) : ((*dir_pin_output_register) &= (~dir_pin_mask));

    analogWrite(pwm_pin, dir_pin_state ? (speed & 0xff) - 1 : speed);
    // a cheating act: do not attempt to reform this unless you know
    // how negative numbers are stored in the 2's complement

    // note that when the input_spd is 0, the (input_spd < 0) ensures dir_pin_state is LOW
    // then the analogWrite writes 0
    // since both pins are completely LOW, the H-bridge circuit is opened, and the motor is stopped
}

void yh::rec::Btn7971b::stop_motor () {
    // turnOffPWM(pwm_pin_timer);
    // (*pwm_pin_output_register) &= (~pwm_pin_mask); // this line replaces digitalWrite(pwm_pin, LOW);
    // some Arduino boards have DAC, which do not use PWM to generate fake analog outputs
    // use the IDE-support function to ensure this is not buggy
    analogWrite(pwm_pin, 0);
    (*dir_pin_output_register) &= (~dir_pin_mask); // this line replaces digitalWrite(dir_pin, LOW);
}

int16_t yh::rec::Btn7971b::get_spd () {
    return speed;
}

int16_t yh::rec::Btn7971b::operator += (const int16_t increase) {
    speed += increase;
    speed = constrain(speed, slowest_spd, fastest_spd);
    const bool dir_pin_state = (speed < 0);
    // this line replaces digitalWrite(dir_pin, dir_pin_state);
    dir_pin_state ? ((*dir_pin_output_register) |= dir_pin_mask) : ((*dir_pin_output_register) &= (~dir_pin_mask));
    analogWrite(pwm_pin, dir_pin_state ? (speed & 0xff) - 1 : speed);
    return speed;
}

int16_t yh::rec::Btn7971b::operator -= (const int16_t decrease) {
    speed -= decrease;
    speed = constrain(speed, slowest_spd, fastest_spd);
    const bool dir_pin_state = (speed < 0);
    // this line replaces digitalWrite(dir_pin, dir_pin_state);
    dir_pin_state ? ((*dir_pin_output_register) |= dir_pin_mask) : ((*dir_pin_output_register) &= (~dir_pin_mask));
    analogWrite(pwm_pin, dir_pin_state ? (speed & 0xff) - 1 : speed);
    return speed;
}

int16_t yh::rec::Btn7971b::operator ++ () {
    if (speed < fastest_spd) {
        speed++;
        const bool dir_pin_state = (speed < 0);
        // this line replaces digitalWrite(dir_pin, dir_pin_state);
        dir_pin_state ? ((*dir_pin_output_register) |= dir_pin_mask) : ((*dir_pin_output_register) &= (~dir_pin_mask));
        analogWrite(pwm_pin, dir_pin_state ? (speed & 0xff) - 1 : speed);
    }
    return speed;
}

int16_t yh::rec::Btn7971b::operator -- () {
    if (speed > slowest_spd) {
        speed--;
        const bool dir_pin_state = (speed < 0);
        // this line replaces digitalWrite(dir_pin, dir_pin_state);
        dir_pin_state ? ((*dir_pin_output_register) |= dir_pin_mask) : ((*dir_pin_output_register) &= (~dir_pin_mask));
        analogWrite(pwm_pin, dir_pin_state ? (speed & 0xff) - 1 : speed);
    }
    return speed;
}

#undef Btn7971b

#endif // #ifndef BTN7971B_CPP
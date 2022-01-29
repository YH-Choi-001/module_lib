#ifndef BTN7971B_CPP
#define BTN7971B_CPP __DATE__ ", " __TIME__

#include "Btn7971b.h"

yh::rec::Btn7971b::Btn7971b (const uint8_t init_pwm_1_pin, const uint8_t init_pwm_2_pin) :
    pwm_1_pin(init_pwm_1_pin), pwm_2_pin(init_pwm_2_pin), speed(0), slowest_spd(-255), fastest_spd(255)
{
    pwm_1_pin_mask = digitalPinToBitMask(pwm_1_pin);
    pwm_1_pin_output_port = portOutputRegister(digitalPinToPort(pwm_1_pin));
    pwm_2_pin_mask = digitalPinToBitMask(pwm_2_pin);
    pwm_2_pin_output_port = portOutputRegister(digitalPinToPort(pwm_2_pin));
}

yh::rec::Btn7971b::Btn7971b (const uint16_t init_pwm_1_and_2_pin) :
    pwm_1_pin(init_pwm_1_and_2_pin >> 8), pwm_2_pin(init_pwm_1_and_2_pin & 0xff), speed(0), slowest_spd(-255), fastest_spd(255)
{
    pwm_1_pin_mask = digitalPinToBitMask(pwm_1_pin);
    pwm_1_pin_output_port = portOutputRegister(digitalPinToPort(pwm_1_pin));
    pwm_2_pin_mask = digitalPinToBitMask(pwm_2_pin);
    pwm_2_pin_output_port = portOutputRegister(digitalPinToPort(pwm_2_pin));
}

inline void yh::rec::Btn7971b::begin () {
    pinMode(pwm_1_pin, OUTPUT);
    pinMode(pwm_2_pin, OUTPUT);
    (*pwm_1_pin_output_port) &= ~pwm_1_pin_mask; // this line replaces digitalWrite(pwm_1_pin, LOW);
    (*pwm_2_pin_output_port) &= ~pwm_2_pin_mask; // this line replaces digitalWrite(pwm_2_pin, LOW);
}

#define SET_SPD(high_pin,pwm_pin,absolute_spd) {(*pwm_##high_pin##_pin_output_port)|=pwm_##high_pin##_pin_mask;analogWrite(pwm_##pwm_pin##_pin,absolute_spd);}

// set the motor to positive speed with the magnitude of the speed
#define SET_POS_SPD(spd) SET_SPD(1,2,spd)
// set the motor to negative speed with the magnitude of the speed
#define SET_NEG_SPD(spd) SET_SPD(2,1,spd)

inline void yh::rec::Btn7971b::set_spd (const int16_t input_spd) {
    speed = constrain(input_spd, slowest_spd, fastest_spd);
    if (speed > 0) {
        SET_POS_SPD(speed);
    } else if (speed < 0) {
        SET_NEG_SPD(-speed);
    } else {
        stop_motor();
    }
}

inline void yh::rec::Btn7971b::stop_motor () {
    (*pwm_2_pin_output_port) &= ~pwm_2_pin_mask; // this line replaces digitalWrite(pwm_2_pin, LOW);
}

inline int16_t yh::rec::Btn7971b::get_spd () {
    return speed;
}

inline int16_t yh::rec::Btn7971b::operator += (const int16_t increase) {
    speed += increase;
    speed = constrain(speed, slowest_spd, fastest_spd);
    if (speed > 0) {
        SET_POS_SPD(speed);
    } else if (speed < 0) {
        SET_NEG_SPD(-speed);
    } else {
        stop_motor();
    }
    return speed;
}

inline int16_t yh::rec::Btn7971b::operator -= (const int16_t decrease) {
    speed -= decrease;
    speed = constrain(speed, slowest_spd, fastest_spd);
    if (speed > 0) {
        SET_POS_SPD(speed);
    } else if (speed < 0) {
        SET_NEG_SPD(-speed);
    } else {
        stop_motor();
    }
    return speed;
}

inline int16_t yh::rec::Btn7971b::operator ++ () {
    if (speed < fastest_spd) {
        speed++;
        if (speed > 0) {
            SET_POS_SPD(speed);
        } else if (speed < 0) {
            SET_NEG_SPD(-speed);
        } else {
            stop_motor();
        }
    }
    return speed;
}

inline int16_t yh::rec::Btn7971b::operator -- () {
    if (speed > slowest_spd) {
        speed--;
        if (speed > 0) {
            SET_POS_SPD(speed);
        } else if (speed < 0) {
            SET_NEG_SPD(-speed);
        } else {
            stop_motor();
        }
    }
    return speed;
}

#undef SET_SPD
#undef SET_POS_SPD
#undef SET_NEG_SPD

#endif // #ifndef BTN7971B_CPP
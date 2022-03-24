// written by YH Choi
// look for the latest version on https://github.com/YH-Choi-001/module_lib/blob/main/Btn7971b/Btn7971b.cpp

#ifndef BTN7971B_CPP
#define BTN7971B_CPP __DATE__ ", " __TIME__

#include "Btn7971b.h"

yh::rec::Btn7971b::Btn7971b (const Btn7971b &init_obj) :
    pwm_pin(init_obj.pwm_pin), dir_pin(init_obj.dir_pin), speed(0), slowest_spd(-255), fastest_spd(255)
{
    pwm_pin_mask = digitalPinToBitMask(pwm_pin);
    pwm_pin_output_register = portOutputRegister(digitalPinToPort(pwm_pin));
    dir_pin_mask = digitalPinToBitMask(dir_pin);
    dir_pin_output_register = portOutputRegister(digitalPinToPort(dir_pin));
    pwm_pin_timer = digitalPinToTimer(pwm_pin);
    timer_identification();
}

yh::rec::Btn7971b::Btn7971b (const uint8_t init_pwm_pin, const uint8_t init_dir_pin) :
    pwm_pin(init_pwm_pin), dir_pin(init_dir_pin), speed(0), slowest_spd(-255), fastest_spd(255)
{
    pwm_pin_mask = digitalPinToBitMask(pwm_pin);
    pwm_pin_output_register = portOutputRegister(digitalPinToPort(pwm_pin));
    dir_pin_mask = digitalPinToBitMask(dir_pin);
    dir_pin_output_register = portOutputRegister(digitalPinToPort(dir_pin));
    pwm_pin_timer = digitalPinToTimer(pwm_pin);
    timer_identification();
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

    analog_write_pwm_pin(dir_pin_state ? (speed & 0xff) - 1 : speed);
    // a cheating act: do not attempt to reform this unless you know
    // how negative numbers are stored in the 2's complement

    // note that when the input_spd is 0, the (input_spd < 0) ensures dir_pin_state is LOW
    // then the analogWrite writes 0
    // since both pins are completely LOW, the H-bridge circuit is opened, and the motor is stopped
}

void yh::rec::Btn7971b::stop_motor () {
    (*timer_A_reg) &= (~timer_pwm_bit_mask);                 // off PWM
    (*pwm_pin_output_register) &= (~pwm_pin_mask);           // write LOW
    (*dir_pin_output_register) &= (~dir_pin_mask);           // write LOW
}

void yh::rec::Btn7971b::full_spd_forwards () {
    (*timer_A_reg) &= (~timer_pwm_bit_mask);                 // off PWM
    (*pwm_pin_output_register) |= pwm_pin_mask;              // write HIGH
    (*dir_pin_output_register) &= (~dir_pin_mask);           // write LOW
}

void yh::rec::Btn7971b::full_spd_backwards () {
    (*timer_A_reg) &= (~timer_pwm_bit_mask);                 // off PWM
    (*pwm_pin_output_register) &= (~pwm_pin_mask);           // write LOW
    (*dir_pin_output_register) |= dir_pin_mask;              // write HIGH
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
    analog_write_pwm_pin(dir_pin_state ? (speed & 0xff) - 1 : speed);
    return speed;
}

int16_t yh::rec::Btn7971b::operator -= (const int16_t decrease) {
    speed -= decrease;
    speed = constrain(speed, slowest_spd, fastest_spd);
    const bool dir_pin_state = (speed < 0);
    // this line replaces digitalWrite(dir_pin, dir_pin_state);
    dir_pin_state ? ((*dir_pin_output_register) |= dir_pin_mask) : ((*dir_pin_output_register) &= (~dir_pin_mask));
    analog_write_pwm_pin(dir_pin_state ? (speed & 0xff) - 1 : speed);
    return speed;
}

int16_t yh::rec::Btn7971b::operator ++ () {
    if (speed < fastest_spd) {
        speed++;
        const bool dir_pin_state = (speed < 0);
        // this line replaces digitalWrite(dir_pin, dir_pin_state);
        dir_pin_state ? ((*dir_pin_output_register) |= dir_pin_mask) : ((*dir_pin_output_register) &= (~dir_pin_mask));
        analog_write_pwm_pin(dir_pin_state ? (speed & 0xff) - 1 : speed);
    }
    return speed;
}

int16_t yh::rec::Btn7971b::operator -- () {
    if (speed > slowest_spd) {
        speed--;
        const bool dir_pin_state = (speed < 0);
        // this line replaces digitalWrite(dir_pin, dir_pin_state);
        dir_pin_state ? ((*dir_pin_output_register) |= dir_pin_mask) : ((*dir_pin_output_register) &= (~dir_pin_mask));
        analog_write_pwm_pin(dir_pin_state ? (speed & 0xff) - 1 : speed);
    }
    return speed;
}

inline void yh::rec::Btn7971b::analog_write_pwm_pin (const uint8_t val) {
    if (timer_pwm_bit_mask) {
        // pwm is available on this pin
        if (!val) {
            (*timer_A_reg) &= (~timer_pwm_bit_mask);        // off PWM
            (*pwm_pin_output_register) &= (~pwm_pin_mask);  // write LOW
        } else if (val == 255) {
            (*timer_A_reg) &= (~timer_pwm_bit_mask);        // off PWM
            (*pwm_pin_output_register) |= pwm_pin_mask;     // write HIGH
        } else {
            (*timer_A_reg) |= timer_pwm_bit_mask;           // on PWM
            (*compare_match_reg) = val;                     // write PWM value
        }
    } else {
        // pwm is not available on this pin
        if (val > 127) (*pwm_pin_output_register) |= pwm_pin_mask; // write HIGH
        else (*pwm_pin_output_register) &= (~pwm_pin_mask);        // write LOW
    }
}

void yh::rec::Btn7971b::timer_identification () {
    // copied from Custom_pins.h, which is also written by me
    switch(pwm_pin_timer)
    {
        // XXX fix needed for atmega8
        #if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
        case TIMER0A:
            timer_A_reg = &TCCR0;
            timer_pwm_bit_mask = (1 << COM00);
            compare_match_reg = &OCR0;
            break;
        #endif

        #if defined(TCCR0A) && defined(COM0A1)
        case TIMER0A:
            timer_A_reg = &TCCR0A;
            timer_pwm_bit_mask = (1 << COM0A1);
            compare_match_reg = &OCR0A;
            break;
        #endif

        #if defined(TCCR0A) && defined(COM0B1)
        case TIMER0B:
            timer_A_reg = &TCCR0A;
            timer_pwm_bit_mask = (1 << COM0B1);
            compare_match_reg = &OCR0B;
            break;
        #endif

        #if defined(TCCR1A) && defined(COM1A1)
        case TIMER1A:
            timer_A_reg = &TCCR1A;
            timer_pwm_bit_mask = (1 << COM1A1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR1A);
            break;
        #endif

        #if defined(TCCR1A) && defined(COM1B1)
        case TIMER1B:
            timer_A_reg = &TCCR1A;
            timer_pwm_bit_mask = (1 << COM1B1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR1B);
            break;
        #endif

        #if defined(TCCR1A) && defined(COM1C1)
        case TIMER1C:
            timer_A_reg = &TCCR1A;
            timer_pwm_bit_mask = (1 << COM1C1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR1C);
            break;
        #endif

        #if defined(TCCR2) && defined(COM21)
        case TIMER2:
            timer_A_reg = &TCCR2;
            timer_pwm_bit_mask = (1 << COM21);
            compare_match_reg = &OCR2;
            break;
        #endif

        #if defined(TCCR2A) && defined(COM2A1)
        case TIMER2A:
            timer_A_reg = &TCCR2A;
            timer_pwm_bit_mask = (1 << COM2A1);
            compare_match_reg = &OCR2A;
            break;
        #endif

        #if defined(TCCR2A) && defined(COM2B1)
        case TIMER2B:
            timer_A_reg = &TCCR2A;
            timer_pwm_bit_mask = (1 << COM2B1);
            compare_match_reg = &OCR2B;
            break;
        #endif

        #if defined(TCCR3A) && defined(COM3A1)
        case TIMER3A:
            timer_A_reg = &TCCR3A;
            timer_pwm_bit_mask = (1 << COM3A1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR3A);
            break;
        #endif

        #if defined(TCCR3A) && defined(COM3B1)
        case TIMER3B:
            timer_A_reg = &TCCR3A;
            timer_pwm_bit_mask = (1 << COM3B1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR3B);
            break;
        #endif

        #if defined(TCCR3A) && defined(COM3C1)
        case TIMER3C:
            timer_A_reg = &TCCR3A;
            timer_pwm_bit_mask = (1 << COM3C1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR3C);
            break;
        #endif

        #if defined(TCCR4A)
        case TIMER4A:
            #if defined(COM4A0)		// only used on 32U4
            cbi(TCCR4A, COM4A0);
            #endif
            timer_A_reg = &TCCR4A;
            timer_pwm_bit_mask = (1 << COM4A1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR4A);
            break;
        #endif
        
        #if defined(TCCR4A) && defined(COM4B1)
        case TIMER4B:
            timer_A_reg = &TCCR4A;
            timer_pwm_bit_mask = (1 << COM4B1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR4B);
            break;
        #endif

        #if defined(TCCR4A) && defined(COM4C1)
        case TIMER4C:
            timer_A_reg = &TCCR4A;
            timer_pwm_bit_mask = (1 << COM4C1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR4C);
            break;
        #endif
            
        #if defined(TCCR4C) && defined(COM4D1)
        case TIMER4D:
            #if defined(COM4D0)		// only used on 32U4
            cbi(TCCR4C, COM4D0);
            #endif
            timer_A_reg = &TCCR4C;
            timer_pwm_bit_mask = (1 << COM4D1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR4D);
            break;
        #endif


        #if defined(TCCR5A) && defined(COM5A1)
        case TIMER5A:
            timer_A_reg = &TCCR5A;
            timer_pwm_bit_mask = (1 << COM5A1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR5A);
            break;
        #endif

        #if defined(TCCR5A) && defined(COM5B1)
        case TIMER5B:
            timer_A_reg = &TCCR5A;
            timer_pwm_bit_mask = (1 << COM5B1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR5B);
            break;
        #endif

        #if defined(TCCR5A) && defined(COM5C1)
        case TIMER5C:
            timer_A_reg = &TCCR5A;
            timer_pwm_bit_mask = (1 << COM5C1);
            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR5C);
            break;
        #endif

        case NOT_ON_TIMER:
        default:
            timer_A_reg = NULL;
            timer_pwm_bit_mask = 0;
            compare_match_reg = NULL;
    }
}

#endif // #ifndef BTN7971B_CPP
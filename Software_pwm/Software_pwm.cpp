#ifndef SOFTWARE_PWM_CPP
#define SOFTWARE_PWM_CPP

#include "Software_pwm.h"

yh::rec::Software_pwm *pwm_ptr;

yh::rec::Software_pwm::Software_pwm () :
    count_of_pins(0), pins(NULL), port_output_regs(NULL), masks(NULL), values(NULL), begin_locking(false)
{
    if (pwm_ptr == NULL) // if this is first declaration
        pwm_ptr = this;
}

yh::rec::Software_pwm::~Software_pwm () {
    if (pins != NULL)
        free(pins);
    if (port_output_regs != NULL)
        free(port_output_regs);
    if (masks != NULL)
        free(masks);
    if (values != NULL)
        free(values);
    count_of_pins = 0;
}

void yh::rec::Software_pwm::attach_pin (const uint8_t pin) {
    if (begin_locking) return; // if begin is called, lock this function
    if (count_of_pins) {
        // there are already pins registered in the object
        uint8_t temp_pins [count_of_pins];
        for (uint8_t i = 0; i < count_of_pins; i++) {
            temp_pins[i] = pins[i];
        }
        free(pins);
        pins = (uint8_t *)calloc(count_of_pins + 1, sizeof(*pins));
        for (uint8_t i = 0; i < count_of_pins; i++) {
            pins[i] = temp_pins[i];
        }
    } else {
        // the first time
        pins = (uint8_t *)calloc(count_of_pins + 1, sizeof(*pins));
    }
    pins[count_of_pins] = pin;
    count_of_pins++;
}

void yh::rec::Software_pwm::begin () {
    port_output_regs = (volatile uint8_t **)calloc(count_of_pins, sizeof(*port_output_regs));
    masks = (uint8_t *)calloc(count_of_pins, sizeof(*masks));
    values = (uint8_t *)calloc(count_of_pins, sizeof(*values));
    for (uint8_t i = 0; i < count_of_pins; i++) {
        port_output_regs[i] = portOutputRegister( digitalPinToPort(pins[i]) );
        masks[i] = digitalPinToBitMask(pins[i]);
        values[i] = 0;
        pinMode(pins[i], OUTPUT);
    }
    // WGM set to 0b0001 for 8-bit fast PWM (not phase-correct PWM)
    TCCR1A &= ~(1 << WGM11);
    TCCR1A |=  (1 << WGM10);
    TCCR1B = (1 << CS10); // set TCCR1B register to prescaler: 1 // default prescaler is 64
    TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupts
    begin_locking = true;
}

int8_t yh::rec::Software_pwm::analog_write (const uint8_t pin, const uint8_t val) {
    for (uint8_t i = 0; i < count_of_pins; i++) {
        if (pins[i] == pin) {
            noInterrupts();
            values[i] = val;
            interrupts();
            break;
        }
    }
}

void yh::rec::Software_pwm::timer_update_isr (void) {
    static uint8_t tick = 0;
    if (!tick) {
        // tick == 0
        for (uint8_t i = 0; i < count_of_pins; i++) {
            if ((values)[i])
                // it contains non-zero value
                // raise the pin
                *(port_output_regs[i]) |= (masks[i]);
            else
                // lower the pin
                *(port_output_regs[i]) &= ~(masks[i]);
        }
    } else if (tick != 255) { // prevent 255-valued pins will be lowered
        // tick != 0
        for (uint8_t i = 0; i < count_of_pins; i++) {
            if (tick == (values[i]))
                // tick is larger than the requested value
                // lower the pin
                *(port_output_regs[i]) &= ~(masks[i]);
        }
    }
    tick++;
}

ISR (TIMER1_OVF_vect) {
    pwm_ptr->timer_update_isr();
}

#endif // #ifndef SOFTWARE_PWM_CPP
#ifndef ATTACH_PCINT_CPP
#define ATTACH_PCINT_CPP

#include "Attach_pcint.h"

//                      /TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\ 
// --------------------<==================== implementation ====================>--------------------
//                      \______________________________________________________/ 



#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)

// define LOW 0
// #define CHANGE 1
// #define FALLING 2
// #define RISING 3
// supports 4 state: LOW, CHANGE, FALLING, RISING

void (*pcint_falling_ptr[3][8])(void);
void (*pcint_rising_ptr[3][8])(void);

uint8_t pcint_trig_rising [3] = {0, 0, 0};
uint8_t pcint_trig_falling [3] = {0, 0, 0};

uint8_t prev_PINx_state [3] = {0, 0, 0};

namespace yh { namespace rec { namespace attach_pcints { namespace pvt {
void empty_function (void) { return; } // really for private use only
} } } }

void attach_pcint (const uint8_t pcint_no, void (*pcint_isr)(void), const uint8_t state) {
    const uint8_t PCINT_GROUP = (pcint_no >> 3);
    const uint8_t PCINT_BITMASK = (1 << (pcint_no & 0b111));
    switch (state) {
        case LOW:
            pcint_trig_rising[PCINT_GROUP] &= ~PCINT_BITMASK;
            pcint_trig_falling[PCINT_GROUP] &= ~PCINT_BITMASK;
            pcint_falling_ptr[PCINT_GROUP][pcint_no & 0b111] = pcint_isr;
            pcint_rising_ptr[PCINT_GROUP][pcint_no & 0b111] = yh::rec::attach_pcints::pvt::empty_function;
            break;
        case CHANGE:
            pcint_trig_rising[PCINT_GROUP] |= PCINT_BITMASK;
            pcint_trig_falling[PCINT_GROUP] |= PCINT_BITMASK;
            pcint_rising_ptr[PCINT_GROUP][pcint_no & 0b111] = pcint_falling_ptr[PCINT_GROUP][pcint_no & 0b111] = pcint_isr;
            break;
        case FALLING:
            pcint_trig_rising[PCINT_GROUP] &= ~PCINT_BITMASK;
            pcint_trig_falling[PCINT_GROUP] |= PCINT_BITMASK;
            pcint_falling_ptr[PCINT_GROUP][pcint_no & 0b111] = pcint_isr;
            pcint_rising_ptr[PCINT_GROUP][pcint_no & 0b111] = yh::rec::attach_pcints::pvt::empty_function;
            break;
        case RISING:
            pcint_trig_rising[PCINT_GROUP] |= PCINT_BITMASK;
            pcint_trig_falling[PCINT_GROUP] &= ~PCINT_BITMASK;
            pcint_falling_ptr[PCINT_GROUP][pcint_no & 0b111] = yh::rec::attach_pcints::pvt::empty_function;
            pcint_rising_ptr[PCINT_GROUP][pcint_no & 0b111] = pcint_isr;
            break;
    }
    enable_pcint_num(pcint_no);
}


void attach_pcint (const uint8_t pcint_no, void (*pcint_rising_isr)(void), void (*pcint_falling_isr)(void)) {
    const uint8_t PCINT_GROUP = (pcint_no >> 3);
    const uint8_t PCINT_BITMASK = (1 << (pcint_no & 0b111));
    pcint_trig_rising[PCINT_GROUP] |= PCINT_BITMASK;
    pcint_trig_falling[PCINT_GROUP] |= PCINT_BITMASK;
    pcint_rising_ptr[PCINT_GROUP][pcint_no & 0b111] = pcint_rising_isr;
    pcint_falling_ptr[PCINT_GROUP][pcint_no & 0b111] = pcint_falling_isr;
    enable_pcint_num(pcint_no);
}

void detach_pcint (const uint8_t pcint_no) {
    disable_pcint_num(pcint_no);
    pcint_falling_ptr[pcint_no >> 3][pcint_no & 0b111] = yh::rec::attach_pcints::pvt::empty_function;
}

#endif

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega

#ifdef PCINT0_vect
ISR (PCINT0_vect) {
    #define PCINT_GROUP (0)
    #define PINx_for_GROUP (PINB)
    #define PCMSKx_for_GROUP (PCMSK0)
    static uint8_t prev_PINx_state = PINx_for_GROUP;
    const uint8_t curr_PINx_state = PINx_for_GROUP;
    const uint8_t changed_pins = prev_PINx_state ^ curr_PINx_state;
    for (uint8_t i = 0, mask = 1; i < 8; i++, mask <<= 1) {
        if (mask & changed_pins & PCMSKx_for_GROUP) {
            if (mask & curr_PINx_state & pcint_trig_rising[PCINT_GROUP]) {
                pcint_rising_ptr[PCINT_GROUP][i]();
            } else if (mask & prev_PINx_state & pcint_trig_falling[PCINT_GROUP]) {
                pcint_falling_ptr[PCINT_GROUP][i]();
            } else {
                while (!(PINx_for_GROUP & mask)) {
                    pcint_falling_ptr[PCINT_GROUP][i]();
                }
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT0_vect is undefined but Attach_pcint.h is included
#endif // #ifdef PCINT0_vect

#ifdef PCINT1_vect
ISR (PCINT1_vect) {
    #define PCINT_GROUP (1)
    // WARNING: PCINT1 takes PE0 pin as 0th pin of the PCINT GROUP, and takes PJ0 to PJ6 as 1th to 6th pin of the PCINT GROUP
    // They are not of the same port
    // #define PINx_for_GROUP ((PINE & 0x01) | (PINJ << 1)) // PE0 for 0, PJ0 to PJ6 for 1 to 7
    const uint8_t PINx_for_GROUP = ((PINE & 0x01) | (PINJ << 1)); // PE0 for 0, PJ0 to PJ6 for 1 to 7
    #define PCMSKx_for_GROUP (PCMSK1)
    static uint8_t prev_PINx_state = PINx_for_GROUP;
    const uint8_t curr_PINx_state = PINx_for_GROUP;
    const uint8_t changed_pins = prev_PINx_state ^ curr_PINx_state;
    for (uint8_t i = 0, mask = 1; i < 8; i++, mask <<= 1) {
        if (mask & changed_pins & PCMSKx_for_GROUP) {
            if (mask & curr_PINx_state & pcint_trig_rising[PCINT_GROUP]) {
                pcint_rising_ptr[PCINT_GROUP][i]();
            } else if (mask & prev_PINx_state & pcint_trig_falling[PCINT_GROUP]) {
                pcint_falling_ptr[PCINT_GROUP][i]();
            } else {
                while (!(((PINE & 0x01) | (PINJ << 1)) & mask)) {
                    pcint_falling_ptr[PCINT_GROUP][i]();
                }
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT1_vect is undefined but Attach_pcint.h is included
#endif // #ifdef PCINT1_vect

#ifdef PCINT2_vect
ISR (PCINT2_vect) {
    #define PCINT_GROUP (2)
    #define PINx_for_GROUP (PINK)
    #define PCMSKx_for_GROUP (PCMSK2)
    static uint8_t prev_PINx_state = PINx_for_GROUP;
    const uint8_t curr_PINx_state = PINx_for_GROUP;
    const uint8_t changed_pins = prev_PINx_state ^ curr_PINx_state;
    for (uint8_t i = 0, mask = 1; i < 8; i++, mask <<= 1) {
        if (mask & changed_pins & PCMSKx_for_GROUP) {
            if (mask & curr_PINx_state & pcint_trig_rising[PCINT_GROUP]) {
                pcint_rising_ptr[PCINT_GROUP][i]();
            } else if (mask & prev_PINx_state & pcint_trig_falling[PCINT_GROUP]) {
                pcint_falling_ptr[PCINT_GROUP][i]();
            } else {
                while (!(PINx_for_GROUP & mask)) {
                    pcint_falling_ptr[PCINT_GROUP][i]();
                }
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT2_vect is undefined but Attach_pcint.h is included
#endif // #ifdef PCINT2_vect

#endif // #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) // Arduino Uno or Nano

#ifdef PCINT0_vect
ISR (PCINT0_vect) {
    #define PCINT_GROUP (0)
    #define PINx_for_GROUP (PINB)
    #define PCMSKx_for_GROUP (PCMSK0)
    static uint8_t prev_PINx_state = PINx_for_GROUP;
    const uint8_t curr_PINx_state = PINx_for_GROUP;
    const uint8_t changed_pins = prev_PINx_state ^ curr_PINx_state;
    for (uint8_t i = 0, mask = 1; i < 8; i++, mask <<= 1) {
        if (mask & changed_pins & PCMSKx_for_GROUP) {
            if (mask & curr_PINx_state & pcint_trig_rising[PCINT_GROUP]) {
                pcint_rising_ptr[PCINT_GROUP][i]();
            } else if (mask & prev_PINx_state & pcint_trig_falling[PCINT_GROUP]) {
                pcint_falling_ptr[PCINT_GROUP][i]();
            } else {
                while (!(PINx_for_GROUP & mask)) {
                    pcint_falling_ptr[PCINT_GROUP][i]();
                }
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT0_vect is undefined but Attach_pcint.h is included
#endif // #ifdef PCINT0_vect

#ifdef PCINT1_vect
ISR (PCINT1_vect) {
    #define PCINT_GROUP (1)
    #define PINx_for_GROUP (PINC)
    #define PCMSKx_for_GROUP (PCMSK1)
    static uint8_t prev_PINx_state = PINx_for_GROUP;
    const uint8_t curr_PINx_state = PINx_for_GROUP;
    const uint8_t changed_pins = prev_PINx_state ^ curr_PINx_state;
    for (uint8_t i = 0, mask = 1; i < 8; i++, mask <<= 1) {
        if (mask & changed_pins & PCMSKx_for_GROUP) {
            if (mask & curr_PINx_state & pcint_trig_rising[PCINT_GROUP]) {
                pcint_rising_ptr[PCINT_GROUP][i]();
            } else if (mask & prev_PINx_state & pcint_trig_falling[PCINT_GROUP]) {
                pcint_falling_ptr[PCINT_GROUP][i]();
            } else {
                while (!(PINx_for_GROUP & mask)) {
                    pcint_falling_ptr[PCINT_GROUP][i]();
                }
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT1_vect is undefined but Attach_pcint.h is included
#endif // #ifdef PCINT1_vect

#ifdef PCINT2_vect
ISR (PCINT2_vect) {
    #define PCINT_GROUP (2)
    #define PINx_for_GROUP (PIND)
    #define PCMSKx_for_GROUP (PCMSK2)
    static uint8_t prev_PINx_state = PINx_for_GROUP;
    const uint8_t curr_PINx_state = PINx_for_GROUP;
    const uint8_t changed_pins = prev_PINx_state ^ curr_PINx_state;
    for (uint8_t i = 0, mask = 1; i < 8; i++, mask <<= 1) {
        if (mask & changed_pins & PCMSKx_for_GROUP) {
            if (mask & curr_PINx_state & pcint_trig_rising[PCINT_GROUP]) {
                pcint_rising_ptr[PCINT_GROUP][i]();
            } else if (mask & prev_PINx_state & pcint_trig_falling[PCINT_GROUP]) {
                pcint_falling_ptr[PCINT_GROUP][i]();
            } else {
                while (!(PINx_for_GROUP & mask)) {
                    pcint_falling_ptr[PCINT_GROUP][i]();
                }
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT2_vect is undefined but Attach_pcint.h is included
#endif // #ifdef PCINT2_vect

#endif // #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) // Arduino Uno or Nano

#endif // #ifndef ATTACH_PCINT_CPP
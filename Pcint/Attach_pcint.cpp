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

void (*pcint_isr_ptr[3][8])(void);

uint8_t pcint_trig_state_high [3] = {0, 0, 0};
uint8_t pcint_trig_state_low [3] = {0, 0, 0};

uint8_t prev_PINx_state [3] = {0, 0, 0};

// namespace yh { namespace rec { namespace attach_pcints { namespace pvt {
void empty_function (void) { return; } // really for private use only
// } } } }

void attach_pcint (const uint8_t pcint_no, void (*pcint_isr)(void), const uint8_t state) {
    const uint8_t PCINT_GROUP = (pcint_no >> 3);
    const uint8_t PCINT_BITMASK = (1 << (pcint_no & 0b111));
    (state & 0x02) ? (pcint_trig_state_high[PCINT_GROUP] |= PCINT_BITMASK) : (pcint_trig_state_high[PCINT_GROUP] &= ~PCINT_BITMASK);
    (state & 0x01) ? (pcint_trig_state_low[PCINT_GROUP] |= PCINT_BITMASK) : (pcint_trig_state_low[PCINT_GROUP] &= ~PCINT_BITMASK);
    pcint_isr_ptr[PCINT_GROUP][pcint_no & 0b111] = pcint_isr;
    enable_pcint_num(pcint_no);
}

void detach_pcint (const uint8_t pcint_no) {
    disable_pcint_num(pcint_no);
    pcint_isr_ptr[pcint_no >> 3][pcint_no & 0b111] = /*yh::rec::attach_pcints::pvt::*/empty_function;
}

#endif

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega

#ifdef PCINT0_vect
ISR (PCINT0_vect) {
    #define PCINT_GROUP (0)
    #define PINx_for_GROUP (PINB)
    static uint8_t prev_PINx_state = PINx_for_GROUP;
    const uint8_t changed_pins = PINx_for_GROUP ^ prev_PINx_state;
    for (uint8_t i = 0; i < 8; i++) {
        const uint8_t mask = (1 << i);
        if (changed_pins & PCMSK0 & mask) {
            switch
            (
                ((pcint_trig_state_high[PCINT_GROUP] & mask) ? 0b10 : 0b00)
                |
                ((pcint_trig_state_low[PCINT_GROUP] & mask)  ? 0b01 : 0b00)
            )
            {
                case LOW:
                    while (!(PINx_for_GROUP & mask)) {
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    }
                    break;
                case CHANGE:
                    pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case FALLING:
                    if (prev_PINx_state & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case RISING:
                    if (PINx_for_GROUP & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                default:
                    break;
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT0_vect is undefined but Attach_pcint_mega.h is included
#endif // #ifdef PCINT0_vect

#ifdef PCINT1_vect
ISR (PCINT1_vect) {
    #define PCINT_GROUP (1)
    // WARNING: PCINT1 takes PE0 pin as 0th pin of the PCINT GROUP, and takes PJ0 to PJ6 as 1th to 6th pin of the PCINT GROUP
    // They are not of the same port
    // #define PINx_for_GROUP ((PINE & 0x80) | (PINJ >> 1)) // PE0 for 0, PJ0 to PJ6 for 1 to 7
    const uint8_t PINx_for_GROUP = ((PINE & 0x80) | (PINJ >> 1)); // PE0 for 0, PJ0 to PJ6 for 1 to 7
    static uint8_t prev_pcint_group_state = PINx_for_GROUP;
    const uint8_t changed_pins = PINx_for_GROUP ^ prev_PINx_state;
    for (uint8_t i = 0; i < 8; i++) {
        const uint8_t mask = (1 << i);
        if (changed_pins & PCMSK1 & mask) {
            switch
            (
                ((pcint_trig_state_high[PCINT_GROUP] & mask) ? 0b10 : 0b00)
                |
                ((pcint_trig_state_low[PCINT_GROUP] & mask)  ? 0b01 : 0b00)
            )
            {
                case LOW:
                    while (!( ((PINE & 0x80) | (PINJ >> 1)) & mask )) {
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    }
                    break;
                case CHANGE:
                    pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case FALLING:
                    if (prev_PINx_state & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case RISING:
                    if (PINx_for_GROUP & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                default:
                    break;
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT1_vect is undefined but Attach_pcint_mega.h is included
#endif // #ifdef PCINT1_vect

#ifdef PCINT2_vect
ISR (PCINT2_vect) {
    #define PCINT_GROUP (2)
    #define PINx_for_GROUP (PINK)
    static uint8_t prev_PINx_state = PINx_for_GROUP;
    const uint8_t changed_pins = PINx_for_GROUP ^ prev_PINx_state;
    for (uint8_t i = 0; i < 8; i++) {
        const uint8_t mask = (1 << i);
        if (changed_pins & PCMSK2 & mask) {
            switch
            (
                ((pcint_trig_state_high[PCINT_GROUP] & mask) ? 0b10 : 0b00)
                |
                ((pcint_trig_state_low[PCINT_GROUP] & mask)  ? 0b01 : 0b00)
            )
            {
                case LOW:
                    while (!(PINx_for_GROUP & mask)) {
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    }
                    break;
                case CHANGE:
                    pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case FALLING:
                    if (prev_PINx_state & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case RISING:
                    if (PINx_for_GROUP & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                default:
                    break;
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT2_vect is undefined but Attach_pcint_mega.h is included
#endif // #ifdef PCINT0_vect

#endif // #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) // Arduino Uno or Nano

#ifdef PCINT0_vect
ISR (PCINT0_vect) {
    #define PCINT_GROUP (0)
    #define PINx_for_GROUP (PINB)
    static uint8_t prev_PINx_state = PINx_for_GROUP;
    const uint8_t changed_pins = PINx_for_GROUP ^ prev_PINx_state;
    for (uint8_t i = 0; i < 8; i++) {
        const uint8_t mask = (1 << i);
        if (changed_pins & PCMSK0 & mask) {
            switch
            (
                ((pcint_trig_state_high[PCINT_GROUP] & mask) ? 0b10 : 0b00)
                |
                ((pcint_trig_state_low[PCINT_GROUP] & mask)  ? 0b01 : 0b00)
            )
            {
                case LOW:
                    while (!(PINx_for_GROUP & mask)) {
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    }
                    break;
                case CHANGE:
                    pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case FALLING:
                    if (prev_PINx_state & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case RISING:
                    if (PINx_for_GROUP & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                default:
                    break;
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT0_vect is undefined but Attach_pcint_uno.h is included
#endif // #ifdef PCINT0_vect

#ifdef PCINT1_vect
ISR (PCINT1_vect) {
    #define PCINT_GROUP (1)
    #define PINx_for_GROUP (PINC)
    static uint8_t prev_PINx_state = PINx_for_GROUP;
    const uint8_t changed_pins = PINx_for_GROUP ^ prev_PINx_state;
    for (uint8_t i = 0; i < 8; i++) {
        const uint8_t mask = (1 << i);
        if (changed_pins & PCMSK1 & mask) {
            switch
            (
                ((pcint_trig_state_high[PCINT_GROUP] & mask) ? 0b10 : 0b00)
                |
                ((pcint_trig_state_low[PCINT_GROUP] & mask)  ? 0b01 : 0b00)
            )
            {
                case LOW:
                    while (!(PINx_for_GROUP & mask)) {
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    }
                    break;
                case CHANGE:
                    pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case FALLING:
                    if (prev_PINx_state & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case RISING:
                    if (PINx_for_GROUP & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                default:
                    break;
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT1_vect is undefined but Attach_pcint_uno.h is included
#endif // #ifdef PCINT1_vect

#ifdef PCINT2_vect
ISR (PCINT2_vect) {
    #define PCINT_GROUP (2)
    #define PINx_for_GROUP (PIND)
    static uint8_t prev_PINx_state = PINx_for_GROUP;
    const uint8_t changed_pins = PINx_for_GROUP ^ prev_PINx_state;
    for (uint8_t i = 0; i < 8; i++) {
        const uint8_t mask = (1 << i);
        if (changed_pins & PCMSK2 & mask) {
            // this pin has changed
            switch
            (
                ((pcint_trig_state_high[PCINT_GROUP] & mask) ? 0b10 : 0b00)
                |
                ((pcint_trig_state_low[PCINT_GROUP] & mask)  ? 0b01 : 0b00)
            )
            {
                case LOW:
                    while (!(PINx_for_GROUP & mask)) {
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    }
                    break;
                case CHANGE:
                    pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case FALLING:
                    if (prev_PINx_state & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                case RISING:
                    if (PINx_for_GROUP & mask)
                        pcint_isr_ptr[PCINT_GROUP][i]();
                    break;
                default:
                    break;
            }
        }
    }
    prev_PINx_state = PINx_for_GROUP;
    #undef PCINT_GROUP
    #undef PINx_for_GROUP
}
#else
#error PCINT2_vect is undefined but Attach_pcint_uno.h is included
#endif // #ifdef PCINT0_vect

#endif // #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) // Arduino Uno or Nano



#endif // #ifndef ATTACH_PCINT_CPP
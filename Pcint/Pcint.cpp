#ifndef PCINT_CPP
#define PCINT_CPP __DATE__ ", " __TIME__

#include "Pcint.h"

void enable_pcint_on_specific_pin (const uint8_t pin) {
    // Pin Change Interrupt - single pin Mask
    (*digitalPinToPCMSK(pin)) |= (1 << digitalPinToPCMSKbit(pin));  // enable specific pin in the PCINT group to trigger the interrupt
    // Pin Change Interrupt Flag Register
    // the respective bit of PCIFR register is set to 1 when a pin in the port changes
    PCIFR &= ~(1 << digitalPinToPCICRbit(pin)); // clear previous interrupts
    // Pin Change Interrupt Control Register
    PCICR |= (1 << digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

void disable_pcint_on_specific_pin (const uint8_t pin) {
    // Pin Change Interrupt - single pin Mask
    (*digitalPinToPCMSK(pin)) &= ~(1 << digitalPinToPCMSKbit(pin));  // disable specific pin in the PCINT group to trigger the interrupt
    if (!(*digitalPinToPCMSK(pin))) {
        (*digitalPinToPCICR(pin)) &= ~(1 << digitalPinToPCICRbit(pin)); // disable interrupt for the group
    }
}

void enable_pcint_num (const uint8_t pcint_num) {
    // switch (pcint_num / 8) {
    switch (pcint_num >> 3) {
        #ifdef PCMSK0
        case 0:
            PCMSK0 |= (1 << pcint_num);
            PCICR |= (1 << 0);
            break;
        #endif
        #ifdef PCMSK1
        case 1:
            PCMSK1 |= (1 << pcint_num);
            PCICR |= (1 << 1);
            break;
        #endif
        #ifdef PCMSK2
        case 2:
            PCMSK2 |= (1 << pcint_num);
            PCICR |= (1 << 2);
            break;
        #endif
        default:
            break;
    }
}

void disable_pcint_num (const uint8_t pcint_num) {
    // switch (pcint_num / 8) {
    switch (pcint_num >> 3) {
        #ifdef PCMSK0
        case 0:
            PCMSK0 &= ~(1 << pcint_num);
            if (!PCMSK0)
                PCICR &= ~(1 << 0);
            break;
        #endif
        #ifdef PCMSK1
        case 1:
            PCMSK1 &= ~(1 << pcint_num);
            if (!PCMSK1)
                PCICR &= ~(1 << 1);
            break;
        #endif
        #ifdef PCMSK2
        case 2:
            PCMSK2 &= ~(1 << pcint_num);
            if (!PCMSK2)
                PCICR &= ~(1 << 2);
            break;
        #endif
        default:
            break;
    }
}

#endif // #ifndef PCINT_CPP
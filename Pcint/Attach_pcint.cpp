#ifndef ATTACH_PCINT_CPP
#define ATTACH_PCINT_CPP

#include "Attach_pcint.h"

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)

namespace {
    void empty_function (void) { return; }
}

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
    pcint_isr_ptr[pcint_no >> 3][pcint_no & 0b111] = empty_function;
}

#endif

#endif // #ifndef ATTACH_PCINT_CPP
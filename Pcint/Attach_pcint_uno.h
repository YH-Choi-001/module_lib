#ifdef ATTACH_PCINT_H

#ifndef ATTACH_PCINT_UNO_H
#define ATTACH_PCINT_UNO_H

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
                // ((pcint_trig_state_high[PCINT_GROUP] & mask) ? 0b10 : 0b00)
                // |
                // ((pcint_trig_state_low[PCINT_GROUP] & mask)  ? 0b01 : 0b00)
                ((pcint_trig_state_high[PCINT_GROUP] & mask) << 1)
                |
                (pcint_trig_state_low[PCINT_GROUP] & mask)
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
                // ((pcint_trig_state_high[PCINT_GROUP] & mask) ? 0b10 : 0b00)
                // |
                // ((pcint_trig_state_low[PCINT_GROUP] & mask)  ? 0b01 : 0b00)
                ((pcint_trig_state_high[PCINT_GROUP] & mask) << 1)
                |
                (pcint_trig_state_low[PCINT_GROUP] & mask)
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
                // ((pcint_trig_state_high[PCINT_GROUP] & mask) ? 0b10 : 0b00)
                // |
                // ((pcint_trig_state_low[PCINT_GROUP] & mask)  ? 0b01 : 0b00)
                ((pcint_trig_state_high[PCINT_GROUP] & mask) << 1)
                |
                (pcint_trig_state_low[PCINT_GROUP] & mask)
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

#endif // #ifndef ATTACH_PCINT_UNO_H

#endif // #ifdef ATTACH_PCINT_H
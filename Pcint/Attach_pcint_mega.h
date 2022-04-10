#ifdef ATTACH_PCINT_H

#ifndef ATTACH_PCINT_MEGA_H
#define ATTACH_PCINT_MEGA_H

// uint8_t pin_to_pcint_no (const uint8_t pin_no) {
//     if (pin_no >= 50 && pin_no <= 53) {
//         return 53 - pin_no; // PCINT 00 - 03
//     }
//     if (pin_no >= 10 && pin_no <= 13) {
//         return pin_no - 6; // PCINT 04 - 07
//     }
//     if (pin_no == 0) {
//         return 8; // PCINT 08
//     }
//     if (pin_no >= 14 && pin_no <= 15) {
//         return 24 - pin_no; // PCINT 09 - 10
//     }
//     if (pin_no >= 62 && pin_no <= 69) {
//         return pin_no - 46; // PCINT 16 - 23
//     }
//     return 255;
// }

#define pin_to_pcint_no(pin_no) \
( \
    (!(pin_no)) ? (8) : \
    ( \
        ( ((pin_no) >= 50) && ((pin_no) <= 53) ) ? (53 - (pin_no)) : \
        ( \
            ( ((pin_no) >= 10) && ((pin_no) <= 13) ) ? ((pin_no) - 6) : \
            ( \
                ( ((pin_no) >= 14) && ((pin_no) <= 15) ) ? (24 - (pin_no)) : \
                ( \
                    ( ((pin_no) >= 62) && ((pin_no) <= 69) ) ? ((pin_no) - 46) : \
                    (255) \
                ) \
            ) \
        ) \
    ) \
)

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
                // ((pcint_trig_state_high[PCINT_GROUP] & mask) ? 0b10 : 0b00)
                // |
                // ((pcint_trig_state_low[PCINT_GROUP] & mask)  ? 0b01 : 0b00)
                ((pcint_trig_state_high[PCINT_GROUP] & mask) << 1)
                |
                (pcint_trig_state_low[PCINT_GROUP] & mask)
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
#error PCINT2_vect is undefined but Attach_pcint_mega.h is included
#endif // #ifdef PCINT0_vect

#endif // #ifndef ATTACH_PCINT_MEGA_H

#endif // #ifdef ATTACH_PCINT_H
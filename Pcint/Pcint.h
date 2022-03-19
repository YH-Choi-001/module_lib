#ifndef PCINT_H
#define PCINT_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

void enable_pcint_on_specific_pin (const uint8_t pin) {
    // Pin Change Interrupt - single pin Mask
    (*digitalPinToPCMSK(pin)) |= (1 << digitalPinToPCMSKbit(pin));  // enable specific pin in the PCINT group to trigger the interrupt
    // Pin Change Interrupt Flag Register
    // the respective bit of PCIFR register is set to 1 when a pin in the port changes
    PCIFR |= (1 << digitalPinToPCICRbit(pin)); // clear previous interrupts
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

// void empty_function (void) { return ; }

// #define PCINT_GROUP_AVAILABLE 3

// void (*rising_pin_int_ptr  [PCINT_GROUP_AVAILABLE][8])(void);
// void (*falling_pin_int_ptr [PCINT_GROUP_AVAILABLE][8])(void);
// void (*change_pin_int_ptr  [PCINT_GROUP_AVAILABLE][8])(void);

// static uint8_t enable_rising_pin_int  [PCINT_GROUP_AVAILABLE];
// static uint8_t enable_falling_pin_int [PCINT_GROUP_AVAILABLE];
// static uint8_t enable_change_pin_int  [PCINT_GROUP_AVAILABLE];

// volatile uint8_t *pcint_pin_input_reg [PCINT_GROUP_AVAILABLE][8];
// uint8_t pcint_pin_bit_mask [PCINT_GROUP_AVAILABLE][8];

// static uint8_t prev_pin_state [PCINT_GROUP_AVAILABLE];

// void attach_pcint (const uint8_t pin, void(*func_ptr)(void), const uint8_t state) {
//     volatile uint8_t *pin_pcmsk = digitalPinToPCMSK(pin);
//     const uint8_t pin_pcmsk_bit = digitalPinToPCMSKbit(pin);
//     uint8_t pcint_group = 255;
//     switch (pin_pcmsk) {

//         #ifdef PCMSK0
//         case &PCMSK0:
//             // This pin provides pin change interrupt feature
//             pcint_group = 0;
//             break;
//         #endif

//         #ifdef PCMSK1
//         case &PCMSK1:
//             // This pin provides pin change interrupt feature
//             pcint_group = 0;
//             break;
//         #endif

//         #ifdef PCMSK2
//         case &PCMSK2:
//             // This pin provides pin change interrupt feature
//             pcint_group = 0;
//             break;
//         #endif

//         case (uint8_t *)0:
//         default:
//             // This pin does not provide pin change interrupt feature
//             pcint_group = 255;
//             break;

//     }
//     if (pcint_group != 255) {
//         pcint_pin_input_reg[pcint_group][pin_pcmsk_bit] = portInputRegister(digitalPinToPort(pin));
//         pcint_pin_bit_mask[pcint_group][pin_pcmsk_bit] = digitalPinToBitMask(pin);
//         switch (state) {
//             case RISING:
//                 rising_pin_int_ptr[pcint_group][pin_pcmsk_bit] = func_ptr;
//                 enable_rising_pin_int[pcint_group] |= (1 << pin_pcmsk_bit);
//                 break;
//             case FALLING:
//                 falling_pin_int_ptr[pcint_group][pin_pcmsk_bit] = func_ptr;
//                 enable_falling_pin_int[pcint_group] |= (1 << pin_pcmsk_bit);
//                 break;
//             case CHANGE:
//                 change_pin_int_ptr[pcint_group][pin_pcmsk_bit] = func_ptr;
//                 enable_change_pin_int[pcint_group] |= (1 << pin_pcmsk_bit);
//                 break;
//         }
//     }
//     if ( (*(pcint_pin_input_reg[pcint_group][pin_pcmsk_bit])) & (pcint_pin_bit_mask[pcint_group][pin_pcmsk_bit]) ) {
//         prev_pin_state[pcint_group] |= (1 << pin_pcmsk_bit);
//     } else {
//         prev_pin_state[pcint_group] &= ~(1 << pin_pcmsk_bit);
//     }
//     enable_pcint_on_specific_pin(pin);
// }

// void detach_pcint (const uint8_t pin) {
//     disable_pcint_on_specific_pin(pin);
//     volatile uint8_t *pin_pcmsk = digitalPinToPCMSK(pin);
//     const uint8_t pin_pcmsk_bit = digitalPinToPCMSKbit(pin);
//     uint8_t pcint_group = 255;
//     switch (pin_pcmsk) {

//         #ifdef PCMSK0
//         case &PCMSK0:
//             // This pin provides pin change interrupt feature
//             pcint_group = 0;
//             break;
//         #endif

//         #ifdef PCMSK1
//         case &PCMSK1:
//             // This pin provides pin change interrupt feature
//             pcint_group = 0;
//             break;
//         #endif

//         #ifdef PCMSK2
//         case &PCMSK2:
//             // This pin provides pin change interrupt feature
//             pcint_group = 0;
//             break;
//         #endif

//         case (uint8_t *)0:
//         default:
//             // This pin does not provide pin change interrupt feature
//             pcint_group = 255;
//             break;

//     }
//     if (pcint_group != 255) {
//         enable_rising_pin_int[pcint_group]  &= ~(1 << pin_pcmsk_bit);
//         enable_falling_pin_int[pcint_group] &= ~(1 << pin_pcmsk_bit);
//         enable_change_pin_int[pcint_group]  &= ~(1 << pin_pcmsk_bit);
//     }
// }

// #ifdef PCMSK0
// ISR (PCINT0_vect) {
//     const uint8_t pcint_group = 0;
//     // first, check which pin's state is changed
//     // then, check against the enable_xxxxxx_pin_int flag if we need to call the attached ISR
//     uint8_t curr_pin_state = 0;
//     for (uint8_t i = 0; i < 8; i++) {
//         if ( (*(pcint_pin_input_reg[pcint_group][i])) & (pcint_pin_bit_mask[pcint_group][i]) ) {
//             curr_pin_state |= (1 << i);
//         } else {
//             curr_pin_state &= ~(1 << i);
//         }
//         if ( curr_pin_state & (~prev_pin_state) & (1 << i) ) {
//             // pin is raised
//             if (enable_change_pin_int[pcint_group] & (1 << i)) {
//                 change_pin_int_ptr[pcint_group][i]();
//             } else if (enable_rising_pin_int[pcint_group] & (1 << i)) {
//                 rising_pin_int_ptr[pcint_group][i]();
//             }
//         } else if ( (~curr_pin_state) & prev_pin_state & (1 << i) ) {
//             // pin is lowered
//             if (enable_change_pin_int[pcint_group] & (1 << i)) {
//                 change_pin_int_ptr[pcint_group][i]();
//             } else if (enable_falling_pin_int[pcint_group] & (1 << i)) {
//                 falling_pin_int_ptr[pcint_group][i]();
//             }
//         }
//     }
//     prev_pin_state[pcint_group] = curr_pin_state;
// }
// #endif

// #ifdef PCMSK1
// ISR (PCINT1_vect) {
//     const uint8_t pcint_group = 1;
//     // first, check which pin's state is changed
//     // then, check against the enable_xxxxxx_pin_int flag if we need to call the attached ISR
//     uint8_t curr_pin_state = 0;
//     for (uint8_t i = 0; i < 8; i++) {
//         if ( (*(pcint_pin_input_reg[pcint_group][i])) & (pcint_pin_bit_mask[pcint_group][i]) ) {
//             curr_pin_state |= (1 << i);
//         } else {
//             curr_pin_state &= ~(1 << i);
//         }
//         if ( curr_pin_state & (~prev_pin_state) & (1 << i) ) {
//             // pin is raised
//             if (enable_change_pin_int[pcint_group] & (1 << i)) {
//                 change_pin_int_ptr[pcint_group][i]();
//             } else if (enable_rising_pin_int[pcint_group] & (1 << i)) {
//                 rising_pin_int_ptr[pcint_group][i]();
//             }
//         } else if ( (~curr_pin_state) & prev_pin_state & (1 << i) ) {
//             // pin is lowered
//             if (enable_change_pin_int[pcint_group] & (1 << i)) {
//                 change_pin_int_ptr[pcint_group][i]();
//             } else if (enable_falling_pin_int[pcint_group] & (1 << i)) {
//                 falling_pin_int_ptr[pcint_group][i]();
//             }
//         }
//     }
//     prev_pin_state[pcint_group] = curr_pin_state;
// }
// #endif


// #ifdef PCMSK2
// ISR (PCINT2_vect) {
//     const uint8_t pcint_group = 2;
//     // first, check which pin's state is changed
//     // then, check against the enable_xxxxxx_pin_int flag if we need to call the attached ISR
//     uint8_t curr_pin_state = 0;
//     for (uint8_t i = 0; i < 8; i++) {
//         if ( (*(pcint_pin_input_reg[pcint_group][i])) & (pcint_pin_bit_mask[pcint_group][i]) ) {
//             curr_pin_state |= (1 << i);
//         } else {
//             curr_pin_state &= ~(1 << i);
//         }
//         if ( curr_pin_state & (~prev_pin_state) & (1 << i) ) {
//             // pin is raised
//             if (enable_change_pin_int[pcint_group] & (1 << i)) {
//                 change_pin_int_ptr[pcint_group][i]();
//             } else if (enable_rising_pin_int[pcint_group] & (1 << i)) {
//                 rising_pin_int_ptr[pcint_group][i]();
//             }
//         } else if ( (~curr_pin_state) & prev_pin_state & (1 << i) ) {
//             // pin is lowered
//             if (enable_change_pin_int[pcint_group] & (1 << i)) {
//                 change_pin_int_ptr[pcint_group][i]();
//             } else if (enable_falling_pin_int[pcint_group] & (1 << i)) {
//                 falling_pin_int_ptr[pcint_group][i]();
//             }
//         }
//     }
//     prev_pin_state[pcint_group] = curr_pin_state;
// }
// #endif

#endif // #ifndef PCINT_H
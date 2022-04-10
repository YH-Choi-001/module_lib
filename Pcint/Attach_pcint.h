#ifndef ATTACH_PCINT_H
#define ATTACH_PCINT_H

#include "Pcint.h"

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) // Mega or Uno or Nano

    // define LOW 0
    // #define CHANGE 1
    // #define FALLING 2
    // #define RISING 3
    // supports 4 state: LOW, CHANGE, FALLING, RISING

    #define pin_to_pcint_no(pin_no) ((digitalPinToPCICRbit(pin_no) << 3) + digitalPinToPCMSKbit(pin_no)) // will be redefined in Arduino MEGA deviation

    void attach_pcint (const uint8_t pcint_no, void (*pcint_isr)(void), const uint8_t state);
    void detach_pcint (const uint8_t pcint_no);

#else

    #warning This Arduino board is not supported by Attach_pcint.h, but supported by Pcint.h.
    #warning Attach_pcint.h officially supports Arduino classic Nano, Uno, Mega family boards only.
    #warning Please self-define the ISR of all PCINTs.

#endif

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega

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
                    (0) \
                ) \
            ) \
        ) \
    ) \
)


#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) // Arduino Uno or Nano

    // nothing to be implemented here

#endif

#endif // #ifndef ATTACH_PCINT_H
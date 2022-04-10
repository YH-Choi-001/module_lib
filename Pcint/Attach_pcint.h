#ifndef ATTACH_PCINT_H
#define ATTACH_PCINT_H

#include "Pcint.h"

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)

    // define LOW 0
    // #define CHANGE 1
    // #define FALLING 2
    // #define RISING 3
    // supports 4 state: LOW, CHANGE, FALLING, RISING

    void (*pcint_isr_ptr[3][8])(void);

    uint8_t pcint_trig_state_high [3] = {0, 0, 0};
    uint8_t pcint_trig_state_low [3] = {0, 0, 0};

    #define pin_to_pcint_no(pin_no) ((digitalPinToPCICRbit(pin_no) << 3) + digitalPinToPCMSKbit(pin_no)) // will be redefined in Arduino MEGA deviation

    void attach_pcint (const uint8_t pcint_no, void (*pcint_isr)(void), const uint8_t state);
    void detach_pcint (const uint8_t pcint_no);

#endif

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega

    #include "Attach_pcint_mega.h"

#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) // Arduino Uno or Nano

    #include "Attach_pcint_uno.h"

#else

    #warning this Arduino board is not be supported by Attach_pcint.h, but supported by Pcint.h
    #warning Attach_pcint.h officially supports Arduino classic Nano, Uno, Mega family boards only
    #warning please self-define the ISR of the PCINTs

#endif


#endif // #ifndef ATTACH_PCINT_H
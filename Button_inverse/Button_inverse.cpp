#ifndef BUTTON_INVERSE_CPP
#define BUTTON_INVERSE_CPP __DATE__ ", " __TIME__

#include <Arduino.h>

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include "Button_inverse_classic.cpp"
#else
#include "Button_inverse_universal.cpp"
#endif

#endif // #ifndef BUTTON_INVERSE_CPP
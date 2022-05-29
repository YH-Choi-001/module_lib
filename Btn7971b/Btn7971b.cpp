#ifndef BTN7971B_CPP
#define BTN7971B_CPP __DATE__ ", " __TIME__

#include <Arduino.h>

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include "Btn7971b_classic.cpp"
#else
#include "BTN7971b_universal.cpp"
#endif

#endif // #ifndef BTN7971B_CPP
#ifndef BTN7971B_H
#define BTN7971B_H __DATE__ ", " __TIME__

#include <Arduino.h>

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include "Btn7971b_classic.h"
#else
#include "BTN7971b_universal.h"
#endif

#endif // #ifndef BTN7971B_H
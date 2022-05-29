#ifndef BUTTON_INVERSE_H
#define BUTTON_INVERSE_H __DATE__ ", " __TIME__

#include <Arduino.h>

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include "Button_inverse_classic.h"
#else
#include "Button_inverse_universal.h"
#endif

#endif // #ifndef BUTTON_INVERSE_H
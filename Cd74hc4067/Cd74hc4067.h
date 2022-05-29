#ifndef CD74HC4067_H
#define CD74HC4067_H __DATE__ ", " __TIME__

#include <Arduino.h>

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include "Cd74hc4067_classic.h"
#else
#include "Cd74hc4067_universal.h"
#endif

#endif // #ifndef CD74HC4067_H
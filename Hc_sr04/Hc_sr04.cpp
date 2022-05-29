#ifndef HC_SR04_CPP
#define HC_SR04_CPP __DATE__ ", " __TIME__

#include <Arduino.h>

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include "Hc_sr04_classic.cpp"
#else
#include "Hc_sr04_universal.cpp"
#endif

#endif //#ifndef HC_SR04_CPP
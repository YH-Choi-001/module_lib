#ifndef HC_SR04_H
#define HC_SR04_H __DATE__ ", " __TIME__

#include <Arduino.h>

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include "Hc_sr04_classic.h"
#else
#include "Hc_sr04_universal.h"
#endif

#endif //#ifndef HC_SR04_H
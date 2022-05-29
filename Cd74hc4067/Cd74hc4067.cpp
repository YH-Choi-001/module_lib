#ifndef CD74HC4067_CPP
#define CD74HC4067_CPP  __DATE__ ", " __TIME__

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include "Cd74hc4067_classic.cpp"
#else
#include "Cd74hc4067_universal.cpp"
#endif

#endif // #ifndef CD74HC4067_CPP
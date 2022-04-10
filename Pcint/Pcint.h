#ifndef PCINT_H
#define PCINT_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

void enable_pcint_on_specific_pin (const uint8_t pin);

void disable_pcint_on_specific_pin (const uint8_t pin);

// You may call me directly if you know the number of the PCINT of the specific pin
void enable_pcint_num (const uint8_t pcint_num);

// You may call me directly if you know the number of the PCINT of the specific pin
void disable_pcint_num (const uint8_t pcint_num);

#endif // #ifndef PCINT_H
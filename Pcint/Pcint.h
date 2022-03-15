#ifndef PCINT_H
#define PCINT_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

void setup_pcint (const uint8_t pin) {
    *digitalPinToPCMSK(pin) |= (1 << digitalPinToPCMSKbit(pin));  // enable pin
    // the respective bit of PCIFR register is set to 1 when a pin in the port changes
    PCIFR |= (1 << digitalPinToPCICRbit(pin)); // clear previous interrupts
    PCICR |= (1 << digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

#endif // #ifndef PCINT_H
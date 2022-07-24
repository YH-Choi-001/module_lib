#ifndef PCINT_H
#define PCINT_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
namespace rec {
namespace pcints {

void enable_pcint_on_specific_pin (const uint8_t pin);

void disable_pcint_on_specific_pin (const uint8_t pin);

// You may call me directly if you know the number of the PCINT of the specific pin
// This function enables a specific pin on the chip to trigger the interrupt only.
// Please define the ISR for the corresponding PCINT group, or you may consider using Attach_pcint.h.
void enable_pcint_num (const uint8_t pcint_num);

// You may call me directly if you know the number of the PCINT of the specific pin
// This function disables a specific pin on the chip to further trigger the interrupt.
void disable_pcint_num (const uint8_t pcint_num);

}
}
}

using yh::rec::pcints::enable_pcint_on_specific_pin;
using yh::rec::pcints::disable_pcint_on_specific_pin;
using yh::rec::pcints::enable_pcint_num;
using yh::rec::pcints::disable_pcint_num;

#endif // #ifndef PCINT_H
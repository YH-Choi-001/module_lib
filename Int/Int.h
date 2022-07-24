#ifndef INT_H
#define INT_H __DATE__ ", " __TIME__

#include <Arduino.h>

namespace yh {
namespace rec {
namespace ints {

// You may call me directly if you know the number of the INT of the specific pin.
// This function enables a specific pin on the chip to trigger the interrupt only.
// Please define the ISR for the corresponding INT,
// or you may consider using attachInterrupt provided by official Arduino.
// @param interrupt_num The number of INTn on the datasheet, not the one provided by Arduino official, as they differs.
void enable_int_num (uint8_t interrupt_num, int mode);
// You may call me directly if you know the number of the INT of the specific pin.
// This function disables a specific pin on the chip to further trigger the interrupt.
void disable_int_num (uint8_t interrupt_num);

}
}
}

using yh::rec::ints::enable_int_num;
using yh::rec::ints::disable_int_num;

#endif // #ifndef INT_H
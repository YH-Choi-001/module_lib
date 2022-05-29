#ifdef BUTTON_INVERSE_CPP

// This library is proved to be working on 6 Feb 2022 at 19:24

#ifndef BUTTON_INVERSE_CLASSIC_CPP
#define BUTTON_INVERSE_CLASSIC_CPP __DATE__ ", " __TIME__

#include "button_inverse.h"

yh::rec::Button_inverse::Button_inverse (const uint8_t init_button_signal_pin) :
    button_signal_pin(init_button_signal_pin)
{
    //
}

#endif // #ifndef BUTTON_INVERSE_CLASSIC_CPP

#endif // #ifdef BUTTON_INVERSE_CPP
#ifndef INT_CPP
#define INT_CPP __DATE__ ", " __TIME__

#include "Int.h"


void yh::rec::ints::enable_int_num (uint8_t interrupt_num, int mode) {
    switch (interrupt_num) {
#if defined(EICRA) && defined(EICRB) && defined(EIMSK)
    // unifies AVR_AT90USBXX2, AVR_ATmegaXXU2, AVR_ATmegaXXU4 and AVR_ATmega1280/2560 series
    case 0:
        EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
        EIMSK |= (1 << INT0);
        break;
    case 1:
        EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
        EIMSK |= (1 << INT1);
        break;
    case 2:
        EICRA = (EICRA & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
        EIMSK |= (1 << INT2);
        break;
    case 3:
        EICRA = (EICRA & ~((1 << ISC30) | (1 << ISC31))) | (mode << ISC30);
        EIMSK |= (1 << INT3);
        break;
    case 4:
        EICRB = (EICRB & ~((1 << ISC40) | (1 << ISC41))) | (mode << ISC40);
        EIMSK |= (1 << INT4);
        break;
    case 5:
        EICRB = (EICRB & ~((1 << ISC50) | (1 << ISC51))) | (mode << ISC50);
        EIMSK |= (1 << INT5);
        break;
    case 6:
        EICRB = (EICRB & ~((1 << ISC60) | (1 << ISC61))) | (mode << ISC60);
        EIMSK |= (1 << INT6);
        break;
    case 7:
        EICRB = (EICRB & ~((1 << ISC70) | (1 << ISC71))) | (mode << ISC70);
        EIMSK |= (1 << INT7);
        break;
#else		
    case 0:
    #if defined(EICRA) && defined(ISC00) && defined(EIMSK)
        EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
        EIMSK |= (1 << INT0);
    #elif defined(MCUCR) && defined(ISC00) && defined(GICR)
        MCUCR = (MCUCR & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
        GICR |= (1 << INT0);
    #elif defined(MCUCR) && defined(ISC00) && defined(GIMSK)
        MCUCR = (MCUCR & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
        GIMSK |= (1 << INT0);
    #else
        #error enable_int_num not finished for this CPU (case 0)
    #endif
        break;

    case 1:
    #if defined(EICRA) && defined(ISC10) && defined(ISC11) && defined(EIMSK)
        EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
        EIMSK |= (1 << INT1);
    #elif defined(MCUCR) && defined(ISC10) && defined(ISC11) && defined(GICR)
        MCUCR = (MCUCR & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
        GICR |= (1 << INT1);
    #elif defined(MCUCR) && defined(ISC10) && defined(GIMSK) && defined(GIMSK)
        MCUCR = (MCUCR & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
        GIMSK |= (1 << INT1);
    #else
        #warning enable_int_num may need some more work for this cpu (case 1)
    #endif
        break;
    
    case 2:
    #if defined(EICRA) && defined(ISC20) && defined(ISC21) && defined(EIMSK)
        EICRA = (EICRA & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
        EIMSK |= (1 << INT2);
    #elif defined(MCUCR) && defined(ISC20) && defined(ISC21) && defined(GICR)
        MCUCR = (MCUCR & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
        GICR |= (1 << INT2);
    #elif defined(MCUCR) && defined(ISC20) && defined(GIMSK) && defined(GIMSK)
        MCUCR = (MCUCR & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
        GIMSK |= (1 << INT2);
    #endif
        break;
#endif
    }
}

void yh::rec::ints::disable_int_num (uint8_t interrupt_num) {
    // Disable the interrupt.  (We can't assume that interruptNum is equal
    // to the number of the EIMSK bit to clear, as this isn't true on the 
    // ATmega8.  There, INT0 is 6 and INT1 is 7.)
    switch (interrupt_num) {
#if defined(EICRA) && defined(EICRB) && defined(EIMSK)
    case 0:
        EIMSK &= ~(1 << INT0);
        break;
    case 1:
        EIMSK &= ~(1 << INT1);
        break;
    case 2:
        EIMSK &= ~(1 << INT2);
        break;
    case 3:
        EIMSK &= ~(1 << INT3);
        break;
    case 4:
        EIMSK &= ~(1 << INT4);
        break;
    case 5:
        EIMSK &= ~(1 << INT5);
        break;
    case 6:
        EIMSK &= ~(1 << INT6);
        break;
    case 7:
        EIMSK &= ~(1 << INT7);
        break;
#else
    case 0:
    #if defined(EIMSK) && defined(INT0)
        EIMSK &= ~(1 << INT0);
    #elif defined(GICR) && defined(ISC00)
        GICR &= ~(1 << INT0); // atmega32
    #elif defined(GIMSK) && defined(INT0)
        GIMSK &= ~(1 << INT0);
    #else
        #error disable_int_num not finished for this cpu
    #endif
        break;

    case 1:
    #if defined(EIMSK) && defined(INT1)
        EIMSK &= ~(1 << INT1);
    #elif defined(GICR) && defined(INT1)
        GICR &= ~(1 << INT1); // atmega32
    #elif defined(GIMSK) && defined(INT1)
        GIMSK &= ~(1 << INT1);
    #else
        #warning disable_int_num may need some more work for this cpu (case 1)
    #endif
        break;
      
    case 2:
    #if defined(EIMSK) && defined(INT2)
        EIMSK &= ~(1 << INT2);
    #elif defined(GICR) && defined(INT2)
        GICR &= ~(1 << INT2); // atmega32
    #elif defined(GIMSK) && defined(INT2)
        GIMSK &= ~(1 << INT2);
    #elif defined(INT2)
        #warning disable_int_num may need some more work for this cpu (case 2)
    #endif
        break;       
#endif
    }
}


#endif // #ifndef INT_CPP
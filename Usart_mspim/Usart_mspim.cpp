#ifndef USART_MSPIM_CPP
#define USART_MSPIM_CPP __DATE__ ", " __TIME__

#include "Usart_mspim.h"

yh::rec::Usart_mspim USPI;

void yh::rec::Usart_mspim::begin () {
    // must be zero before enabling the transmitter
    UBRR0 = 0;
    UCSR0A = (1 << TXC0);  // any old transmit now complete (clear the bit by writing 1 to the bit location)
    // pinMode (XCK, OUTPUT);   // set XCK pin as output to enable master mode
    UCSR0C = (1 << UMSEL00) | (1 << UMSEL01);  // Master SPI mode, default SPI mode 0 and MSBFIRST
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // transmit enable and receive enable, RX and TX and DR buf empty interrupts are disabled
    // must be done last, see page 206
    UBRR0 = F_CPU / 2 / 4000000 - 1; // clock frequency = default 4MHz
}

void yh::rec::Usart_mspim::end () {
    UCSR0A = 0;
    UCSR0B = 0;
    UCSR0C = 0;
    UBRR0 = 0;
}

#endif // #ifndef USART_MSPIM_CPP
#ifndef USART_MSPIM_CPP
#define USART_MSPIM_CPP __DATE__ ", " __TIME__

#include "Usart_mspim.h"

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) // Arduino Uno
yh::rec::Usart_mspim USPI (&UBRR0, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0, &DDRD, (1 << 4));
#endif

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega
yh::rec::Usart_mspim USPI (&UBRR0, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0, &DDRE, (1 << 2));
yh::rec::Usart_mspim USPI1 (&UBRR1, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1, &DDRD, (1 << 5));
yh::rec::Usart_mspim USPI2 (&UBRR2, &UCSR2A, &UCSR2B, &UCSR2C, &UDR2, &DDRH, (1 << 2));
yh::rec::Usart_mspim USPI3 (&UBRR3, &UCSR3A, &UCSR3B, &UCSR3C, &UDR3, &DDRJ, (1 << 2));
#endif

void yh::rec::Usart_mspim::begin () {
    // must be zero before enabling the transmitter
    (*ubrrn) = 0;
    (*ucsrna) = (1 << TXC0);  // any old transmit now complete (clear the bit by writing 1 to the bit location)
    (*xckn_port_ddr) |= xckn_port_bit_mask;  // set XCK pin as output to enable master mode
    (*ucsrnc) = (1 << UMSEL00) | (1 << UMSEL01);  // Master SPI mode, default SPI mode 0 and MSBFIRST
    (*ucsrnb) = (1 << TXEN0) | (1 << RXEN0);  // transmit enable and receive enable, RX and TX and DR buf empty interrupts are disabled
    // must be done last, see page 206
    (*ubrrn) = F_CPU / 2 / 4000000 - 1; // clock frequency = default 4MHz
}

void yh::rec::Usart_mspim::end () {
    (*ucsrna) = 0;
    (*ucsrnb) = 0;
    (*ucsrnc) = 0;
    (*ubrrn) = 0;
}

#endif // #ifndef USART_MSPIM_CPP
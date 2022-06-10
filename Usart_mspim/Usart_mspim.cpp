#ifndef USART_MSPIM_CPP
#define USART_MSPIM_CPP __DATE__ ", " __TIME__

#include "Usart_mspim.h"

#if defined(__AVR_ATmega32U4__) // Arduino Micro or Leonardo
yh::rec::USART_MSPIM_Class USPI1 (&UBRR1, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1, &DDRD, (1 << 5));
#endif // #if defined(__AVR_ATmega32U4__) // Arduino Micro or Leonardo

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) // Arduino Uno
yh::rec::USART_MSPIM_Class USPI (&UBRR0, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0, &DDRD, (1 << 4));
#endif // #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) // Arduino Uno

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega
yh::rec::USART_MSPIM_Class USPI (&UBRR0, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0, &DDRE, (1 << 2));
yh::rec::USART_MSPIM_Class USPI1 (&UBRR1, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1, &DDRD, (1 << 5));
yh::rec::USART_MSPIM_Class USPI2 (&UBRR2, &UCSR2A, &UCSR2B, &UCSR2C, &UDR2, &DDRH, (1 << 2));
yh::rec::USART_MSPIM_Class USPI3 (&UBRR3, &UCSR3A, &UCSR3B, &UCSR3C, &UDR3, &DDRJ, (1 << 2));
#endif

#endif // #ifndef USART_MSPIM_CPP
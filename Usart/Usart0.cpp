#ifndef USART0_CPP
#define USART0_CPP __DATE__ ", " __TIME__

#include "Usart.h"

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) // Arduino Uno
//                      UBRRn,  UCSRnA,  UCSRnB,  UCSRnC,  UDRn,  DDRxck, MSKxck,  DDRtx, MSKtx,    DDRrx, MSKrx
yh::rec::Usart usart  (&UBRR0, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0, &DDRD, (1 << 4) ); //, &DDRD, (1 << 1), &DDRD, (1 << 0));
#endif // #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) // Arduino Uno

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega
//                      UBRRn,  UCSRnA,  UCSRnB,  UCSRnC,  UDRn,  DDRxck, MSKxck,  DDRtx, MSKtx,    DDRrx, MSKrx
yh::rec::Usart usart  (&UBRR0, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0, &DDRE, (1 << 2) ); //, &DDRE, (1 << 1), &DDRE, (1 << 0));
#endif

#if defined(USART_RX_vect)
  ISR(USART_RX_vect)
#elif defined(USART0_RX_vect)
  ISR(USART0_RX_vect)
#elif defined(USART_RXC_vect)
  ISR(USART_RXC_vect) // ATmega8
#else
  #error "Don't know what the Data Received vector is called for Serial"
#endif
{
    usart.rx_isr();
}

#if defined(UART0_UDRE_vect)
ISR(UART0_UDRE_vect)
#elif defined(UART_UDRE_vect)
ISR(UART_UDRE_vect)
#elif defined(USART0_UDRE_vect)
ISR(USART0_UDRE_vect)
#elif defined(USART_UDRE_vect)
ISR(USART_UDRE_vect)
#else
  #error "Don't know what the Data Register Empty vector is called for Serial"
#endif
{
    usart.tx_ddr_empty_isr();
}

#endif // #ifndef USART0_CPP
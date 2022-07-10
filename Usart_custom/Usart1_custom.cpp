#ifndef USART1_CUSTOM_CPP
#define USART1_CUSTOM_CPP __DATE__ ", " __TIME__

#include "Usart_custom.h"

#if defined(__AVR_ATmega32U4__) // Arduino Micro or Leonardo
#warning The only XCK1 pin on the Arduino Micro (or Leonardo) is broken out as the TX LED pin.
#warning If you insist to use USART_MSPIM, use Arduino Uno instead.
//                      UBRRn,  UCSRnA,  UCSRnB,  UCSRnC,  UDRn,  DDRxck, MSKxck,  DDRtx, MSKtx,    DDRrx, MSKrx
yh::rec::Usart usart1 (&UBRR1, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1, &DDRD, (1 << 5) ); //, &DDRD, (1 << 3), &DDRD, (1 << 2));
#endif // #if defined(__AVR_ATmega32U4__) // Arduino Micro or Leonardo

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega
//                      UBRRn,  UCSRnA,  UCSRnB,  UCSRnC,  UDRn,  DDRxck, MSKxck,  DDRtx, MSKtx,    DDRrx, MSKrx
yh::rec::Usart usart1 (&UBRR1, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1, &DDRD, (1 << 5) ); //, &DDRD, (1 << 3), &DDRD, (1 << 2));
#endif

#if defined(UART1_RX_vect)
    ISR(UART1_RX_vect) { usart1.rx_isr(); }
#elif defined(USART1_RX_vect)
    ISR(USART1_RX_vect) { usart1.rx_isr(); }
#else
    // #error "Don't know what the Data Received vector is called for Serial"
#endif

#if defined(UART1_UDRE_vect)
    ISR(UART1_UDRE_vect) { usart1.tx_ddr_empty_isr(); }
#elif defined(USART1_UDRE_vect)
    ISR(USART1_UDRE_vect) { usart1.tx_ddr_empty_isr(); }
#else
    // #error "Don't know what the Data Register Empty vector is called for Serial"
#endif

#endif // #ifndef USART1_CUSTOM_CPP
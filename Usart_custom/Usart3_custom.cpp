#ifndef USART3_CUSTOM_CPP
#define USART3_CUSTOM_CPP __DATE__ ", " __TIME__

#include "Usart_custom.h"

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega
//                      UBRRn,  UCSRnA,  UCSRnB,  UCSRnC,  UDRn,  DDRxck, MSKxck,  DDRtx, MSKtx,    DDRrx, MSKrx
yh::rec::Usart usart3 (&UBRR3, &UCSR3A, &UCSR3B, &UCSR3C, &UDR3, &DDRJ, (1 << 2) ); //, &DDRJ, (1 << 1), &DDRJ, (1 << 0));
#endif

ISR(USART3_RX_vect)
{
    usart3.rx_isr();
}

ISR(USART3_UDRE_vect)
{
    usart3.tx_ddr_empty_isr();
}

#endif // #ifndef USART3_CUSTOM_CPP
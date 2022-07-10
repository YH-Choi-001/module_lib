#ifndef USART2_CUSTOM_CPP
#define USART2_CUSTOM_CPP __DATE__ ", " __TIME__

#include "Usart_custom.h"

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega
//                      UBRRn,  UCSRnA,  UCSRnB,  UCSRnC,  UDRn,  DDRxck, MSKxck,  DDRtx, MSKtx,    DDRrx, MSKrx
yh::rec::Usart usart2 (&UBRR2, &UCSR2A, &UCSR2B, &UCSR2C, &UDR2, &DDRH, (1 << 2) ); //, &DDRH, (1 << 1), &DDRH, (1 << 0));
#endif

ISR(USART2_RX_vect)
{
    usart2.rx_isr();
}

ISR(USART2_UDRE_vect)
{
    usart2.tx_ddr_empty_isr();
}

#endif // #ifndef USART2_CUSTOM_CPP
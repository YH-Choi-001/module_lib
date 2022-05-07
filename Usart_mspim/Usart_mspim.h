// see datasheet of ATmega1280 | ATmega2560
// https://ww1.microchip.com/downloads/en/devicedoc/atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf#G14.1002704

#ifndef USART_MSPIM_H
#define USART_MSPIM_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <SPI.h>

// UCSRnA:

// 0:

#ifndef UCSR0A
#define UCSR0A UCSR0A
#endif

#ifndef RXC0
#define RXC0 7
#endif

#ifndef TXC0
#define TXC0 6
#endif

#ifndef UDRE0
#define UDRE0 5
#endif

// 1:

#ifndef UCSR1A
#define UCSR1A UCSR1A
#endif

#ifndef RXC1
#define RXC1 7
#endif

#ifndef TXC1
#define TXC1 6
#endif

#ifndef UDRE1
#define UDRE1 5
#endif

// 2:

#ifndef UCSR2A
#define UCSR2A UCSR2A
#endif

#ifndef RXC2
#define RXC2 7
#endif

#ifndef TXC2
#define TXC2 6
#endif

#ifndef UDRE2
#define UDRE2 5
#endif

// 3:

#ifndef UCSR3A
#define UCSR3A UCSR3A
#endif

#ifndef RXC3
#define RXC3 7
#endif

#ifndef TXC3
#define TXC3 6
#endif

#ifndef UDRE3
#define UDRE3 5
#endif

// UCSRnB:

// 0:

#ifndef UCSR0B
#define UCSR0B UCSR0B
#endif

#ifndef RXCIE0
#define RXCIE0 7
#endif

#ifndef TXCIE0
#define TXCIE0 6
#endif

#ifndef UDRIE
#define UDRIE 5
#endif

#ifndef UDRIE0
#define UDRIE0 5
#endif

#ifndef RXEN0
#define RXEN0 4
#endif

#ifndef TXEN0
#define TXEN0 3
#endif

// 1:

#ifndef UCSR1B
#define UCSR1B UCSR1B
#endif

#ifndef RXCIE1
#define RXCIE1 7
#endif

#ifndef TXCIE1
#define TXCIE1 6
#endif

#ifndef UDRIE
#define UDRIE 5
#endif

#ifndef UDRIE1
#define UDRIE1 5
#endif

#ifndef RXEN1
#define RXEN1 4
#endif

#ifndef TXEN1
#define TXEN1 3
#endif

// 2:

#ifndef UCSR2B
#define UCSR2B UCSR2B
#endif

#ifndef RXCIE2
#define RXCIE2 7
#endif

#ifndef TXCIE2
#define TXCIE2 6
#endif

#ifndef UDRIE
#define UDRIE 5
#endif

#ifndef UDRIE2
#define UDRIE2 5
#endif

#ifndef RXEN2
#define RXEN2 4
#endif

#ifndef TXEN2
#define TXEN2 3
#endif

// 3:

#ifndef UCSR3B
#define UCSR3B UCSR3B
#endif

#ifndef RXCIE3
#define RXCIE3 7
#endif

#ifndef TXCIE3
#define TXCIE3 6
#endif

#ifndef UDRIE
#define UDRIE 5
#endif

#ifndef UDRIE3
#define UDRIE3 5
#endif

#ifndef RXEN3
#define RXEN3 4
#endif

#ifndef TXEN3
#define TXEN3 3
#endif

// UCSRnC:

// 0:

#ifndef UCSR0C
#define UCSR0C UCSR0C
#endif

#ifndef UMSEL01
#define UMSEL01 7
#endif

#ifndef UMSEL00
#define UMSEL00 6
#endif

#ifndef UDORD0
#define UDORD0 2
#endif

#ifndef UCPHA0
#define UCPHA0 1
#endif

#ifndef UCPOL0
#define UCPOL0 0
#endif

// 1:

#ifndef UCSR1C
#define UCSR1C UCSR1C
#endif

#ifndef UMSEL11
#define UMSEL11 7
#endif

#ifndef UMSEL10
#define UMSEL10 6
#endif

#ifndef UDORD1
#define UDORD1 2
#endif

#ifndef UCPHA1
#define UCPHA1 1
#endif

#ifndef UCPOL1
#define UCPOL1 0
#endif

// 2:

#ifndef UCSR2C
#define UCSR2C UCSR2C
#endif

#ifndef UMSEL21
#define UMSEL21 7
#endif

#ifndef UMSEL20
#define UMSEL20 6
#endif

#ifndef UDORD2
#define UDORD2 2
#endif

#ifndef UCPHA2
#define UCPHA2 1
#endif

#ifndef UCPOL2
#define UCPOL2 0
#endif

// 3:

#ifndef UCSR3C
#define UCSR3C UCSR3C
#endif

#ifndef UMSEL31
#define UMSEL31 7
#endif

#ifndef UMSEL30
#define UMSEL30 6
#endif

#ifndef UDORD3
#define UDORD3 2
#endif

#ifndef UCPHA3
#define UCPHA3 1
#endif

#ifndef UCPOL3
#define UCPOL3 0
#endif

// UBRRn:

// 0:

#ifndef UBRR0
#define UBRR0 UBRR0
#endif

// 1:

#ifndef UBRR1
#define UBRR1 UBRR1
#endif

// 2:

#ifndef UBRR2
#define UBRR2 UBRR2
#endif

// 3:

#ifndef UBRR3
#define UBRR3 UBRR3
#endif

// UDRn:

// 0:

#ifndef UDR0
#define UDR0 UDR0
#endif

// 1:

#ifndef UDR1
#define UDR1 UDR1
#endif

// 2:

#ifndef UDR2
#define UDR2 UDR2
#endif

// 3:

#ifndef UDR3
#define UDR3 UDR3
#endif


namespace yh {
    namespace rec {
        class Usart_mspim;
    }
}

class USART_MSPIM_Settings {
    private:
        uint16_t ubrrn;
        uint8_t ucsrnc;
        friend class yh::rec::Usart_mspim;
    public:
        USART_MSPIM_Settings (uint32_t clock, uint8_t bitOrder, uint8_t dataMode) __attribute__((__always_inline__)) {
            ubrrn = F_CPU / 2 / clock - 1;
            switch (dataMode) {
                case SPI_MODE0: // CPOL == 0, CPHA == 0
                    ucsrnc = 0;
                    break;
                case SPI_MODE1: // CPOL == 0, CPHA == 1
                    ucsrnc = (1 << UCPHA0);
                    break;
                case SPI_MODE2: // CPOL == 1, CPHA == 0
                    ucsrnc = (1 << UCPOL0);
                    break;
                case SPI_MODE3: // CPOL == 1, CPHA == 1
                    ucsrnc = (1 << UCPOL0) | (1 << UCPHA0);
                    break;
                default:
                    ucsrnc = 0;
                    break;
            }
            if (bitOrder == LSBFIRST)
                ucsrnc |= (1 << UDORD0);
        }
};

namespace yh {
    namespace rec {
        class Usart_mspim {
            private:
                //
            protected:
                // USART MSPIM n baud rate register
                volatile uint8_t *const ubrrn;
                // USART MSPIM n control and signal register A
                volatile uint8_t *const ucsrna;
                // USART MSPIM n control and signal register B
                volatile uint8_t *const ucsrnb;
                // USART MSPIM n control and signal register C
                volatile uint8_t *const ucsrnc;
                // USART MSPIM n data register
                volatile uint8_t *const udrn;
                // the data direction register of the port of XCKn pin
                volatile uint8_t *const xckn_port_ddr;
                // the bit mask of XCKn pin in its port
                const uint8_t xckn_port_bit_mask;
            public:
                // default constructor
                Usart_mspim (
                    volatile uint8_t *const init_ubrrn,
                    volatile uint8_t *const init_ucsrna,
                    volatile uint8_t *const init_ucsrnb,
                    volatile uint8_t *const init_ucsrnc,
                    volatile uint8_t *const init_udrn,
                    volatile uint8_t *const init_xckn_port_ddr,
                    const uint8_t init_xckn_port_bit_mask
                ) __attribute__((__always_inline__))
                :
                    ubrrn(init_ubrrn),
                    ucsrna(init_ucsrna),
                    ucsrnb(init_ucsrnb),
                    ucsrnc(init_ucsrnc),
                    udrn(init_udrn),
                    xckn_port_ddr(init_xckn_port_ddr),
                    xckn_port_bit_mask(init_xckn_port_bit_mask)
                {
                    //
                }
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                // You should call pinMode(4, OUTPUT); by yourself, to maintain compatibility
                void begin ();
                // disables the USART_MSPIM bus
                void end ();
                // This function is used to configure correct settings.
                inline void beginTransaction (USART_MSPIM_Settings settings) {
                    // wait for transmitter buffer empty
                    while (!((*ucsrna) & (1 << UDRE0))) {}
                    // wait for data-transmitting complete
                    while (!((*ucsrna) & (1 << TXC0))) {}
                    uint8_t oldSREG = SREG;
                    noInterrupts();
                    (*ucsrnc) = settings.ucsrnc;
                    (*ubrrn) = settings.ubrrn;
                    SREG = oldSREG;
                }
                // releases the USART_MSPIM bus to others
                inline void endTransaction () {
                    //
                }
                // Write to the USART_MSPIM bus (Tx pin) and also receive (Rx pin)
                inline uint8_t transfer (const uint8_t data) {
                    // wait for transmitter buffer empty
                    while (!((*ucsrna) & (1 << UDRE0))) {}
                    // send byte
                    (*udrn) = data;
                    asm volatile ("nop");
                    // wait for data-receiving complete
                    while (!((*ucsrna) & (1 << RXC0))) {}
                    const uint8_t received_data = (*udrn);
                    // wait for data-transmitting complete
                    while (!((*ucsrna) & (1 << TXC0))) {}
                    (*ucsrna) |= (1 << TXC0); // clear the bit manually
                    return received_data;
                }
                inline void transfer (uint8_t *const buf, const uint8_t len) {
                    for (uint8_t i = 0; i < len; i++) {
                        // wait for transmitter buffer empty
                        while (!((*ucsrna) & (1 << UDRE0))) {}
                        // send byte
                        (*udrn) = buf[i];
                        asm volatile ("nop");
                        // wait for data-receiving complete
                        while (!((*ucsrna) & (1 << RXC0))) {}
                        buf[i] = (*udrn);
                    }
                    // wait for data-transmitting complete
                    while (!((*ucsrna) & (1 << TXC0))) {}
                    (*ucsrna) |= (1 << TXC0); // clear the bit manually
                }
                // This function is deprecated.  New applications should use
                // beginTransaction() to configure USART_MSPIM settings.
                inline void setBitOrder(uint8_t bitOrder) {
                    if (bitOrder == LSBFIRST) (*ucsrnc) |= (1 << UDORD0);
                    else (*ucsrnc) &= ~(1 << UDORD0);
                }
                // This function is deprecated.  New applications should use
                // beginTransaction() to configure USART_MSPIM settings.
                inline void setDataMode(uint8_t dataMode) {
                    switch (dataMode) {
                        case SPI_MODE0: // CPOL == 0, CPHA == 0
                            (*ucsrnc) &= ~(1 << UCPOL0);
                            (*ucsrnc) &= ~(1 << UCPHA0);
                            break;
                        case SPI_MODE1: // CPOL == 0, CPHA == 1
                            (*ucsrnc) &= ~(1 << UCPOL0);
                            (*ucsrnc) |= (1 << UCPHA0);
                            break;
                        case SPI_MODE2: // CPOL == 1, CPHA == 0
                            (*ucsrnc) |= (1 << UCPOL0);
                            (*ucsrnc) &= ~(1 << UCPHA0);
                            break;
                        case SPI_MODE3: // CPOL == 1, CPHA == 1
                            (*ucsrnc) |= (1 << UCPOL0);
                            (*ucsrnc) |= (1 << UCPHA0);
                            break;
                        default:
                            (*ucsrnc) &= ~(1 << UCPOL0);
                            (*ucsrnc) &= ~(1 << UCPHA0);
                            break;
                    }
                }
                // This function is deprecated.  New applications should use
                // beginTransaction() to configure USART_MSPIM settings.
                inline void setClockDivider (const uint8_t clockDiv) {
                    uint8_t freq = 0;
                    // UBRR0 = F_CPU / 2 / freq - 1;
                    switch (clockDiv) {
                        case SPI_CLOCK_DIV2:
                            (*ubrrn) = 2 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV4:
                            (*ubrrn) = 4 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV8:
                            (*ubrrn) = 8 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV16:
                            (*ubrrn) = 16 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV32:
                            (*ubrrn) = 32 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV64:
                            (*ubrrn) = 64 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV128:
                            (*ubrrn) = 128 / 2 - 1;
                            break;
                        default:
                            (*ubrrn) = 128 / 2 - 1;
                            break;
                    }
                }
        };
    }
}

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega
#warning None of the XCKn pins on the Arduino Mega board are broken out.
#warning If you insist to use USART_MSPIM, use Arduino Uno instead.
#endif // #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
extern yh::rec::Usart_mspim USPI;
#endif

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
extern yh::rec::Usart_mspim USPI;
extern yh::rec::Usart_mspim USPI1;
extern yh::rec::Usart_mspim USPI2;
extern yh::rec::Usart_mspim USPI3;
#endif

#endif // #ifndef USART_MSPIM_H
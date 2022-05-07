// see datasheet of ATmega1280 | ATmega2560
// https://ww1.microchip.com/downloads/en/devicedoc/atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf#G14.1002704

#ifndef USART_MSPIM_H
#define USART_MSPIM_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <SPI.h>

namespace yh {
    namespace rec {
        class Usart_mspim;
    }
}

class USART_MSPIM_Settings {
    private:
        uint8_t ubrrn, ucsrnc;
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
                //
            public:
                // default constructor
                Usart_mspim () {}
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                // You should call pinMode(4, OUTPUT); by yourself, to maintain compatibility
                void begin ();
                // disables the USART_MSPIM bus
                void end ();
                // This function is used to configure correct settings.
                inline void beginTransaction (USART_MSPIM_Settings settings) {
                    // wait for transmitter ready
                    while (!(UCSR0A & (1 << UDRE0))) {}
                    // wait for data-receiving complete
                    while (!(UCSR0A & (1 << RXC0))) {}
                    UCSR0C = settings.ucsrnc;
                    UBRR0 = settings.ubrrn;
                }
                // releases the USART_MSPIM bus to others
                inline void endTransaction () {
                    //
                }
                // Write to the USART_MSPIM bus (Tx pin) and also receive (Rx pin)
                inline uint8_t transfer (const uint8_t data) {
                    // wait for transmitter ready
                    while (!(UCSR0A & (1 << UDRE0))) {}
                    // send byte
                    UDR0 = data;
                    // wait for data-receiving complete
                    while (!(UCSR0A & (1 << RXC0))) {}
                    // receive byte, return it
                    return UDR0;
                }
                // This function is deprecated.  New applications should use
                // beginTransaction() to configure USART_MSPIM settings.
                inline static void setBitOrder(uint8_t bitOrder) {
                    if (bitOrder == LSBFIRST) UCSR0C |= (1 << UDORD0);
                    else UCSR0C &= ~(1 << UDORD0);
                }
                // This function is deprecated.  New applications should use
                // beginTransaction() to configure USART_MSPIM settings.
                inline static void setDataMode(uint8_t dataMode) {
                    switch (dataMode) {
                        case SPI_MODE0: // CPOL == 0, CPHA == 0
                            UCSR0C &= ~(1 << UCPOL0);
                            UCSR0C &= ~(1 << UCPHA0);
                            break;
                        case SPI_MODE1: // CPOL == 0, CPHA == 1
                            UCSR0C &= ~(1 << UCPOL0);
                            UCSR0C |= (1 << UCPHA0);
                            break;
                        case SPI_MODE2: // CPOL == 1, CPHA == 0
                            UCSR0C |= (1 << UCPOL0);
                            UCSR0C &= ~(1 << UCPHA0);
                            break;
                        case SPI_MODE3: // CPOL == 1, CPHA == 1
                            UCSR0C |= (1 << UCPOL0);
                            UCSR0C |= (1 << UCPHA0);
                            break;
                        default:
                            UCSR0C &= ~(1 << UCPOL0);
                            UCSR0C &= ~(1 << UCPHA0);
                            break;
                    }
                }
                // This function is deprecated.  New applications should use
                // beginTransaction() to configure USART_MSPIM settings.
                inline static void setClockDivider (const uint8_t clockDiv) {
                    uint8_t freq = 0;
                    // UBRR0 = F_CPU / 2 / freq - 1;
                    switch (clockDiv) {
                        case SPI_CLOCK_DIV2:
                            UBRR0 = 2 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV4:
                            UBRR0 = 4 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV8:
                            UBRR0 = 8 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV16:
                            UBRR0 = 16 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV32:
                            UBRR0 = 32 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV64:
                            UBRR0 = 64 / 2 - 1;
                            break;
                        case SPI_CLOCK_DIV128:
                            UBRR0 = 128 / 2 - 1;
                            break;
                        default:
                            UBRR0 = 128 / 2 - 1;
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

extern yh::rec::Usart_mspim USPI;

#endif // #ifndef USART_MSPIM_H
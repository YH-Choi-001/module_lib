// see datasheet of ATmega1280 | ATmega2560
// https://ww1.microchip.com/downloads/en/devicedoc/atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf#G14.1002704

#ifndef USART_MSPIM_H
#define USART_MSPIM_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include <SPI.h>

// UCSRnA:

#if defined(RXC0)
#define RXCn RXC0
#elif defined(RXC1)
#define RXCn RXC1
#else
#define RXCn 7
#endif

#if defined(TXC0)
#define TXCn TXC0
#elif defined(TXC1)
#define TXCn TXC1
#else
#define TXCn 6
#endif

#if defined(UDRE0)
#define UDREn UDRE0
#elif defined(UDRE1)
#define UDREn UDRE1
#else
#define UDREn 5
#endif

// UCSRnB:

#if defined(RXCIE0)
#define RXCIEn RXCIE0
#elif defined(RXCIE1)
#define RXCIEn RXCIE1
#else
#define RXCIEn 7
#endif

#if defined(TXCIE0)
#define TXCIEn TXCIE0
#elif defined(TXCIE1)
#define TXCIEn TXCIE1
#else
#define TXCIEn 6
#endif

#if defined(UDRIE0)
#define UDRIEn UDRIE0
#elif defined(UDRIE1)
#define UDRIEn UDRIE1
#else
#define UDRIEn 5
#endif

#if defined(RXEN0)
#define RXENn RXEN0
#elif defined(RXEN1)
#define RXENn RXEN1
#else
#define RXENn 4
#endif

#if defined(TXEN0)
#define TXENn TXEN0
#elif defined(TXEN1)
#define TXENn TXEN1
#else
#define TXENn 3
#endif

// UCSRnC:

// 0:

#if defined(UMSEL01)
#define UMSELn1 UMSEL01
#elif defined(UMSEL11)
#define UMSELn1 UMSEL11
#else
#define UMSELn1 7
#endif

#if defined(UMSEL00)
#define UMSELn0 UMSEL00
#elif defined(UMSEL10)
#define UMSELn0 UMSEL10
#else
#define UMSELn0 6
#endif

#if defined(UDORD0)
#define UDORDn UDORD0
#elif defined(UDORD1)
#define UDORDn UDORD1
#else
#define UDORDn 2
#endif

#if defined(UCPHA0)
#define UCPHAn UCPHA0
#elif defined(UCPHA1)
#define UCPHAn UCPHA1
#else
#define UCPHAn 1
#endif

#if defined(UCPOL0)
#define UCPOLn UCPOL0
#elif defined(UCPOL1)
#define UCPOLn UCPOL1
#else
#define UCPOLn 0
#endif

namespace yh {
    namespace rec {
        class USART_MSPIM_Class;
        class USART_MSPIM_Settings {
            private:
            public:
                uint16_t ubrrn;
                uint8_t ucsrnc;
                friend class USART_MSPIM_Class;
            public:
                USART_MSPIM_Settings (uint32_t clock, uint8_t bitOrder, uint8_t dataMode) __attribute__((__always_inline__)) {
                    const double ubrrn_double = F_CPU / 2 / static_cast<double>(clock) - 1;
                    ubrrn = (ubrrn_double < 0) ? 0 : ceil(ubrrn_double);
                    ucsrnc = (1 << UMSELn0) | (1 << UMSELn1);
                    switch (dataMode) {
                        case SPI_MODE0: // CPOL == 0, CPHA == 0
                            break;
                        case SPI_MODE1: // CPOL == 0, CPHA == 1
                            ucsrnc |= (1 << UCPHAn);
                            break;
                        case SPI_MODE2: // CPOL == 1, CPHA == 0
                            ucsrnc |= (1 << UCPOLn);
                            break;
                        case SPI_MODE3: // CPOL == 1, CPHA == 1
                            ucsrnc |= (1 << UCPOLn) | (1 << UCPHAn);
                            break;
                        default:
                            break;
                    }
                    if (bitOrder == LSBFIRST)
                        ucsrnc |= (1 << UDORDn);
                }
        };
        class USART_MSPIM_Class {
            private:
                //
            protected:
                // USART MSPIM n baud rate register
                volatile uint16_t *const ubrrn;
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
                // the data direction register of the port of MOSIn pin
                volatile uint8_t *const mosin_port_ddr;
                // the bit mask of MOSIn pin in its port
                const uint8_t mosin_port_bit_mask;
                // the data direction register of the port of MISOn pin
                volatile uint8_t *const mison_port_ddr;
                // the bit mask of MISOn pin in its port
                const uint8_t mison_port_bit_mask;
            public:
                // default constructor
                USART_MSPIM_Class (
                    volatile uint16_t *const init_ubrrn,
                    volatile uint8_t *const init_ucsrna,
                    volatile uint8_t *const init_ucsrnb,
                    volatile uint8_t *const init_ucsrnc,
                    volatile uint8_t *const init_udrn,
                    volatile uint8_t *const init_xckn_port_ddr,
                    const uint8_t init_xckn_port_bit_mask,
                    volatile uint8_t *const init_mosin_port_ddr,
                    const uint8_t init_mosin_port_bit_mask,
                    volatile uint8_t *const init_mison_port_ddr,
                    const uint8_t init_mison_port_bit_mask
                ) __attribute__((__always_inline__))
                :
                    ubrrn(init_ubrrn),
                    ucsrna(init_ucsrna),
                    ucsrnb(init_ucsrnb),
                    ucsrnc(init_ucsrnc),
                    udrn(init_udrn),
                    xckn_port_ddr(init_xckn_port_ddr),
                    xckn_port_bit_mask(init_xckn_port_bit_mask),
                    mosin_port_ddr(init_mosin_port_ddr),
                    mosin_port_bit_mask(init_mosin_port_bit_mask),
                    mison_port_ddr(init_mison_port_ddr),
                    mison_port_bit_mask(init_mison_port_bit_mask)
                {
                    //
                }
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                // You should call pinMode(4, OUTPUT); by yourself, to maintain compatibility
                void begin () {
                    // must be zero before enabling the transmitter
                    (*ubrrn) = 0;
                    (*ucsrna) = (1 << TXCn);  // any old transmit now complete (clear the bit by writing 1 to the bit location)
                    (*xckn_port_ddr) |= xckn_port_bit_mask;  // set XCK pin as output to enable master mode
                    (*ucsrnc) = (1 << UMSELn0) | (1 << UMSELn1);  // Master SPI mode, default SPI mode 0 and MSBFIRST
                    (*ucsrnb) = (1 << TXENn) | (1 << RXENn);  // transmit enable and receive enable, RX and TX and DR buf empty interrupts are disabled
                    // must be done last, see page 206
                    (*ubrrn) = F_CPU / 2 / 4000000 - 1; // clock frequency = default 4MHz
                    (*mosin_port_ddr) |= mosin_port_bit_mask; // set MOSI pin as output
                    (*mison_port_ddr) &= ~mison_port_bit_mask; // set MISO pin as input
                }
                // disables the USART_MSPIM bus
                void end () {
                    (*ucsrna) = 0;
                    (*ucsrnb) = 0;
                    (*ucsrnc) = 0;
                    (*ubrrn) = 0;
                }
                // This function is used to configure correct settings.
                inline void beginTransaction (USART_MSPIM_Settings settings) {
                    // wait for transmitter buffer empty
                    // while (!((*ucsrna) & (1 << UDRE0))) {}
                    // wait for data-transmitting complete
                    // while (!((*ucsrna) & (1 << TXC0))) {}
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
                    while (!((*ucsrna) & (1 << UDREn))) {}
                    // send byte
                    (*udrn) = data;
                    asm volatile("nop");
                    // wait for data-receiving complete
                    while (!((*ucsrna) & (1 << RXCn))) {}
                    const uint8_t received_data = (*udrn);
                    // wait for data-transmitting complete
                    while (!((*ucsrna) & (1 << TXCn))) {}
                    (*ucsrna) |= (1 << TXCn); // clear the bit manually
                    return received_data;
                }
                // transfer 16 bits of data through the USART_MPSIM bus
                inline uint16_t transfer16 (uint16_t data) {
                    union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } in, out;
                    in.val = data;
                    if (!((*ucsrnc) & (1 << UDORDn))) {
                        while (!((*ucsrna) & (1 << UDREn))) {} // wait for transmitter buffer empty
                        (*udrn) = in.msb;                      // write msb data to tx buf
                        asm volatile("nop");                   // nop
                        while (!((*ucsrna) & (1 << RXCn))) {}  // wait for data-receiving complete
                        out.msb = (*udrn);                     // read msb data from rx buf
                        while (!((*ucsrna) & (1 << UDREn))) {} // wait for transmitter buffer empty
                        (*udrn) = in.lsb;                      // write lsb data to tx buf
                        asm volatile("nop");                   // nop
                        while (!((*ucsrna) & (1 << RXCn))) {}  // wait for data-receiving complete
                        out.lsb = (*udrn);                     // read lsb data from rx buf
                    } else {
                        while (!((*ucsrna) & (1 << UDREn))) {} // wait for transmitter buffer empty
                        (*udrn) = in.lsb;                      // write lsb data to tx buf
                        asm volatile("nop");                   // nop
                        while (!((*ucsrna) & (1 << RXCn))) {}  // wait for data-receiving complete
                        out.lsb = (*udrn);                     // read lsb data from rx buf
                        while (!((*ucsrna) & (1 << UDREn))) {} // wait for transmitter buffer empty
                        (*udrn) = in.msb;                      // write msb data to tx buf
                        asm volatile("nop");                   // nop
                        while (!((*ucsrna) & (1 << RXCn))) {}  // wait for data-receiving complete
                        out.msb = (*udrn);                     // read msb data from rx buf
                    }
                    while (!((*ucsrna) & (1 << TXCn))) {}     // wait for data-transmitting complete
                    (*ucsrna) |= (1 << TXCn); // clear the bit manually
                    return out.val;
                }
                // transfer an array of data through the USART_MPSIM bus
                inline void transfer (uint8_t *const buf, const uint8_t len) {
                    for (uint8_t i = 0; i < len; i++) {
                        // wait for transmitter buffer empty
                        while (!((*ucsrna) & (1 << UDREn))) {}
                        // send byte
                        (*udrn) = buf[i];
                        asm volatile("nop");
                        // wait for data-receiving complete
                        while (!((*ucsrna) & (1 << RXCn))) {}
                        buf[i] = (*udrn);
                    }
                    // wait for data-transmitting complete
                    while (!((*ucsrna) & (1 << TXCn))) {}
                    (*ucsrna) |= (1 << TXCn); // clear the bit manually
                }
                // This function is deprecated.  New applications should use
                // beginTransaction() to configure USART_MSPIM settings.
                inline void setBitOrder (uint8_t bitOrder) {
                    if (bitOrder == LSBFIRST) (*ucsrnc) |= (1 << UDORDn);
                    else (*ucsrnc) &= ~(1 << UDORDn);
                }
                // This function is deprecated.  New applications should use
                // beginTransaction() to configure USART_MSPIM settings.
                inline void setDataMode (uint8_t dataMode) {
                    switch (dataMode) {
                        case SPI_MODE0: // CPOL == 0, CPHA == 0
                            (*ucsrnc) &= ~(1 << UCPOLn);
                            (*ucsrnc) &= ~(1 << UCPHAn);
                            break;
                        case SPI_MODE1: // CPOL == 0, CPHA == 1
                            (*ucsrnc) &= ~(1 << UCPOLn);
                            (*ucsrnc) |= (1 << UCPHAn);
                            break;
                        case SPI_MODE2: // CPOL == 1, CPHA == 0
                            (*ucsrnc) |= (1 << UCPOLn);
                            (*ucsrnc) &= ~(1 << UCPHAn);
                            break;
                        case SPI_MODE3: // CPOL == 1, CPHA == 1
                            (*ucsrnc) |= (1 << UCPOLn);
                            (*ucsrnc) |= (1 << UCPHAn);
                            break;
                        default:
                            (*ucsrnc) &= ~(1 << UCPOLn);
                            (*ucsrnc) &= ~(1 << UCPHAn);
                            break;
                    }
                }
                // This function is deprecated.  New applications should use
                // beginTransaction() to configure USART_MSPIM settings.
                inline void setClockDivider (const uint8_t clockDiv) {
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

#if defined(__AVR_ATmega32U4__) // Arduino Micro or Leonardo
#warning The only XCK1 pin on the Arduino Micro (or Leonardo) is broken out as the TX LED pin.
#warning If you insist to use USART_MSPIM, use Arduino Uno instead.
//                                        UBRRn,  UCSRnA,  UCSRnB,  UCSRnC,  UDRn,  DDRxck, MSKxck,  DDRtx, MSKtx,    DDRrx, MSKrx
static yh::rec::USART_MSPIM_Class USPI1 (&UBRR1, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1, &DDRD, (1 << 5), &DDRD, (1 << 3), &DDRD, (1 << 2));
#endif // #if defined(__AVR_ATmega32U4__) // Arduino Micro or Leonardo

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) // Arduino Uno
//                                        UBRRn,  UCSRnA,  UCSRnB,  UCSRnC,  UDRn,  DDRxck, MSKxck,  DDRtx, MSKtx,    DDRrx, MSKrx
static yh::rec::USART_MSPIM_Class USPI  (&UBRR0, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0, &DDRD, (1 << 4), &DDRD, (1 << 1), &DDRD, (1 << 0));
#endif // #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) // Arduino Uno

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega
//                                        UBRRn,  UCSRnA,  UCSRnB,  UCSRnC,  UDRn,  DDRxck, MSKxck,  DDRtx, MSKtx,    DDRrx, MSKrx
static yh::rec::USART_MSPIM_Class USPI  (&UBRR0, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0, &DDRE, (1 << 2), &DDRE, (1 << 1), &DDRE, (1 << 0));
static yh::rec::USART_MSPIM_Class USPI1 (&UBRR1, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1, &DDRD, (1 << 5), &DDRD, (1 << 3), &DDRD, (1 << 2));
static yh::rec::USART_MSPIM_Class USPI2 (&UBRR2, &UCSR2A, &UCSR2B, &UCSR2C, &UDR2, &DDRH, (1 << 2), &DDRH, (1 << 1), &DDRH, (1 << 0));
static yh::rec::USART_MSPIM_Class USPI3 (&UBRR3, &UCSR3A, &UCSR3B, &UCSR3C, &UDR3, &DDRJ, (1 << 2), &DDRJ, (1 << 1), &DDRJ, (1 << 0));
#endif

#endif // #ifndef USART_MSPIM_H
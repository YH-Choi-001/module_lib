#ifndef USART_CUSTOM_H
#define USART_CUSTOM_H __DATE__ ", " __TIME__

#include <Arduino.h>
#include <Stream.h>

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

#if defined(FE0)
#define FEn FE0
#elif defined(FE1)
#define FEn FE1
#else
#define FEn 4
#endif

#if defined(DOR0)
#define DORn DOR0
#elif defined(DOR1)
#define DORn DOR1
#else
#define DORn 3
#endif

#if defined(UPE0)
#define UPEn UPE0
#elif defined(UPE1)
#define UPEn UPE1
#else
#define UPEn 2
#endif

#if defined(U2X0)
#define U2Xn U2X0
#elif defined(U2X1)
#define U2Xn U2X1
#else
#define U2Xn 1
#endif

#if defined(MPCM0)
#define MPCMn MPCM0
#elif defined(MPCM1)
#define MPCMn MPCM1
#else
#define MPCMn 0
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

#if defined(UCSZ02)
#define UCSZn2 UCSZ02
#elif defined(UCSZ12)
#define UCSZn2 UCSZ12
#else
#define UCSZn2 2
#endif

#if defined(RXB80)
#define RXB8n RXB80
#elif defined(RXB81)
#define RXB8n RXB81
#else
#define RXB8n 1
#endif

#if defined(TXB80)
#define TXB8n TXB80
#elif defined(TXB81)
#define TXB8n TXB81
#else
#define TXB8n 0
#endif

// UCSRnC:

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

#if defined(UPM01)
#define UPMn1 UPM01
#elif defined(UPM11)
#define UPMn1 UPM11
#else
#define UPMn1 5
#endif

#if defined(UPM00)
#define UPMn0 UPM00
#elif defined(UPM10)
#define UPMn0 UPM10
#else
#define UPMn0 4
#endif

#if defined(USBS0)
#define USBSn USBS0
#elif defined(USBS1)
#define USBSn USBS1
#else
#define USBSn 3
#endif

#if defined(UCSZ01)
#define UCSZn1 UCSZ01
#elif defined(UCSZ11)
#define UCSZn1 UCSZ11
#else
#define UCSZn1 2
#endif

#if defined(UCSZ00)
#define UCSZn0 UCSZ00
#elif defined(UCSZ10)
#define UCSZn0 UCSZ10
#else
#define UCSZn0 1
#endif

#if defined(UCPOL0)
#define UCPOLn UCPOL0
#elif defined(UCPOL1)
#define UCPOLn UCPOL1
#else
#define UCPOLn 0
#endif

// Check at compiletime that it is really ok to use the bit positions of
// UART0 for the other UARTs as well, in case these values ever get
// changed for future hardware.
#if defined(TXC1) && (TXC1 != TXC0 || RXEN1 != RXEN0 || RXCIE1 != RXCIE0 || \
		      UDRIE1 != UDRIE0 || U2X1 != U2X0 || UPE1 != UPE0 || \
		      UDRE1 != UDRE0)
#error "Not all bit positions for UART1 are the same as for UART0"
#endif
#if defined(TXC2) && (TXC2 != TXC0 || RXEN2 != RXEN0 || RXCIE2 != RXCIE0 || \
		      UDRIE2 != UDRIE0 || U2X2 != U2X0 || UPE2 != UPE0 || \
		      UDRE2 != UDRE0)
#error "Not all bit positions for UART2 are the same as for UART0"
#endif
#if defined(TXC3) && (TXC3 != TXC0 || RXEN3 != RXEN0 || RXCIE3 != RXCIE0 || \
		      UDRIE3 != UDRIE0 || U3X3 != U3X0 || UPE3 != UPE0 || \
		      UDRE3 != UDRE0)
#error "Not all bit positions for UART3 are the same as for UART0"
#endif

// for compatibility with Arduino
// Define config for Serial.begin(baud, config);
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

namespace yh {
    namespace rec {
        // at most 64 * 9-bits, due to max size of integer supported by avr-gcc is uint64_t
        // #ifndef USART_RX_BUFFER_SIZE
        #define USART_RX_BUFFER_SIZE 64
        // #endif
        // #ifndef USART_TX_BUFFER_SIZE
        #define USART_TX_BUFFER_SIZE 64
        // #endif

        class Usart;
        class Usart_settings {
            private:
                uint8_t ucsrna, ucsrnb, ucsrnc, ubrrn, master_mode;
                friend class Usart;
            public:
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                // @param baud max: F_CPU / 8
                // @param data_len: 5 to 9 bits
                // @param stop_bits: 1 to 2 bits
                // @param parity_bits: disable, odd or even (0, 1, or 2)
                // @param is_uart: 0 or 1 (false or true)
                // @param clock_pol: 0 or 1 (0 is data-change on rising XCKn, data-sample on falling XCKn, vice versa)
                Usart_settings (
                    uint32_t baud = 9600,
                    uint8_t data_len = 8,
                    uint8_t stop_bits = 1,
                    uint8_t parity_bits = 0,
                    uint8_t is_uart = 1,
                    uint8_t is_master = 0,
                    uint8_t clock_pol = 0
                ) __attribute__((__always_inline__)) :
                    // pre-assign values to UCSRnX registers
                    ucsrna(
                        (0 << RXCn)  | // rx complete flag no need to be cleared
                        (1 << TXCn)  | // clear tx complete flag
                        (0 << UDREn) | // not to write data register empty flag
                        (0 << FEn)   | // clear frame error flag
                        (0 << DORn)  | // clear data overrun flag
                        (0 << UPEn)  | // clear parity error flag
                        (0 << U2Xn)  | // double speed flag to be set in baud rate config part
                        (0 << MPCMn)   // clear multi-master flag by default (currently not supported)
                    ),
                    ucsrnb(
                        (1 << RXCIEn) | // enable RXCn flag to generate interrupt by default
                        (0 << TXCIEn) | // disable TXCn flag to generate interrupt by default (useless)
                        (1 << UDRIEn) | // enable UDREn flag to generate interrupt by default
                        (1 << RXENn)  | // enable receiver by default
                        (1 << TXENn)  | // enable transmitter by default
                        (0 << UCSZn2) | // 9-bit flag to be set in data frame config part
                        (0 << RXB8n)  | // write a 0 to bits that have no writing permission
                        (0 << TXB8n)    // clear 8th bit of 9-bit mode
                    ),
                    ucsrnc(
                        (0 << UMSELn1) | // clear MSPIM flag as we are using the port as uart / usart
                        (0 << UMSELn0) | // clear sync flag by default to select uart (asynchronous)
                        (0 << UPMn1)   | // clear parity enable flag by default to disable parity bit
                        (0 << UPMn0)   | // clear parity odd flag by default to disable parity bit
                        (0 << USBSn)   | // clear 2-stop-bits flag by default to select 1 stop bit
                        (1 << UCSZn1)  | // to be set in data frame config part
                        (1 << UCSZn0)  | // to be set in data frame config part
                        (0 << UCPOLn)    // to be set in data mode config part
                    )
                {
                    double ubrrn_temp;
                    // set baud rate
                    if (is_uart) { // uart async normal mode
                        // select uart
                        // -----default selected: ucsrnc_temp &= ~(1 << UMSELn0);
                        if (baud > (F_CPU / 16UL)) { // async double speed mode
                            ubrrn_temp = F_CPU / 8 / static_cast<double>(baud) - 1;
                            // enable async double speed mode
                            ucsrna |= (1 << U2Xn);
                        } else { // async normal mode
                            ubrrn_temp = F_CPU / 16 / static_cast<double>(baud) - 1;
                            // disable async double speed mode
                            // -----default selected: ucsrna_temp &= ~(1 << U2Xn);
                        }
                        // XCKn pin is not involved in the communication bus, so we do not touch it
                        // write clock polarity bit to 0
                        ucsrnc &= ~(1 << UCPOLn);
                    } else { // usart sync mode
                        // select usart
                        ucsrnc |= (1 << UMSELn0);
                        // select clock polarity
                        if (clock_pol) {
                            ucsrnc |= (1 << UCPOLn);
                        } else {
                            // -----default selected: ucsrnc_temp &= ~(1 << UCPOLn);
                        }
                        if (is_master) { // sync master mode (or mpsim)
                            master_mode = 1;
                            ubrrn_temp = F_CPU / 2 / static_cast<double>(baud) - 1;
                        } else {
                            master_mode = 0;
                            // sync slave mode
                        }
                    }
                    ubrrn = static_cast<uint16_t>(ceil(ubrrn_temp));
                    // set frame format (data frame and data mode)
                    if (parity_bits) {
                        // enable parity bits and select number of parity bits
                        ucsrnc |= (1 << UPMn1);
                        if (parity_bits & static_cast<uint8_t>(0b1)) {
                            ucsrnc |= (1 << UPMn0);
                        } else {
                            // select even parity bits
                            // -----default selected: ucsrnc &= ~(1 << UPMn0);
                        }
                    } else {
                        // disable parity bits
                        // -----default selected: ucsrnc_temp &= ~((1 << UPMn1) | (1 << UPMn0));
                    }
                    if (stop_bits == static_cast<uint8_t>(2U)) {
                        // enable 2 stop bits
                        ucsrnc |= (1 << USBSn);
                    } else {
                        // disable 2 stop bits to select 1 stop bit
                        // -----default selected: ucsrnc_temp &= ~(1 << USBSn);
                    }
                    if (data_len == static_cast<uint8_t>(9U)) {
                        // 9-bits
                        // enable 9-bit
                        ucsrnb |= (1 << UCSZn2);
                        // select 9-bit
                        ucsrnc |= (1 << UCSZn1) | (1 << UCSZn0);
                    } else {
                        // 5 to 8-bits
                        // disable 9-bit
                        // -----default selected: ucsrnb_temp &= ~(1 << UCSZn2);
                        // select 5 to 8-bits
                        ucsrnc |= (((data_len - static_cast<uint8_t>(5U)) & static_cast<uint8_t>(0b11)) << UCSZn0);
                    }
                }
                // inline void set_syncing (const uint8_t is_sync);
                // inline void set_baud_rate (const uint32_t baud);
                // inline void set_clock_polarity (const uint8_t clock_pol);
                // inline void set_parity_bits (const uint8_t parity_bits);
                // inline void set_stop_bits (const uint8_t stop_bits);
                // inline void set_data_len (const uint8_t data_len);
                // inline void set_master_mode (const uint8_t is_master);
                inline void set_syncing (const uint8_t is_sync) {
                    if (is_sync)
                        // sync mode
                        ucsrnc |= (1 << UMSELn0);
                    else
                        // async mode
                        ucsrnc &= ~(1 << UMSELn0);
                }
                inline void set_baud_rate (const uint32_t baud) {
                    if (ucsrnc & (1 << UMSELn0)) {
                        // sync mode
                        ubrrn = static_cast<uint16_t>(ceil(F_CPU / 2 / static_cast<double>(baud) - 1));
                    } else {
                        // async mode
                        if (baud > (F_CPU / 16UL)) { // async double speed mode
                            ubrrn = static_cast<uint16_t>(ceil(F_CPU / 8 / static_cast<double>(baud) - 1));
                            // enable async double speed mode
                            ucsrna |= (1 << U2Xn);
                        } else { // async normal mode
                            ubrrn = static_cast<uint16_t>(ceil(F_CPU / 16 / static_cast<double>(baud) - 1));
                            // disable async double speed mode
                            ucsrna &= ~(1 << U2Xn);
                        }
                    }
                }
                inline void set_clock_polarity (const uint8_t clock_pol) {
                    if (ucsrnc & (1 << UMSELn0)) {
                        // sync mode
                        // set clock polarity
                        if (clock_pol) {
                            ucsrnc |= (1 << UCPOLn);
                        } else {
                            ucsrnc &= ~(1 << UCPOLn);
                        }
                    }
                }
                inline void set_parity_bits (const uint8_t parity_bits) {
                    if (parity_bits) {
                        // enable parity bits and select number of parity bits
                        ucsrnc |= (1 << UPMn1);
                        if (parity_bits & static_cast<uint8_t>(0b1)) {
                            ucsrnc |= (1 << UPMn0);
                        } else {
                            // select even parity bits
                            ucsrnc &= ~(1 << UPMn0);
                        }
                    } else {
                        // disable parity bits
                        ucsrnc &= ~((1 << UPMn1) | (1 << UPMn0));
                    }
                }
                inline void set_stop_bits (const uint8_t stop_bits) {
                    if (stop_bits == static_cast<uint8_t>(2U)) {
                        // enable 2 stop bits
                        ucsrnc |= (1 << USBSn);
                    } else {
                        // disable 2 stop bits to select 1 stop bit
                        ucsrnc &= ~(1 << USBSn);
                    }
                }
                inline void set_data_len (const uint8_t data_len) {
                    if (data_len == static_cast<uint8_t>(9U)) {
                        // 9-bits
                        // enable 9-bit
                        ucsrnb |= (1 << UCSZn2);
                        // select 9-bit
                        ucsrnc |= (1 << UCSZn1) | (1 << UCSZn0);
                    } else {
                        // 5 to 8-bits
                        // disable 9-bit
                        ucsrnb &= ~(1 << UCSZn2);
                        // select 5 to 8-bits
                        ucsrnc |= (((data_len - static_cast<uint8_t>(5U)) & static_cast<uint8_t>(0b11)) << UCSZn0);
                    }
                }
                inline void set_master_mode (const uint8_t is_master) {
                    master_mode = is_master;
                }
        };
        // data frame:
            // 1 start bit
            // 5, 6, 7, 8 or 9 data bits
            // no, even or odd parity bits
            // 1 or 2 stop bits
        class Usart : public Stream {
            private:
                //
            protected:
                // USART MSPIM n baud rate register
                volatile uint16_t *const ubrrn;
                // USART MSPIM n control and signal register A
                // [RXCn] [TXCn] [UDREn] [FEn] [DORn] [UPEn] [U2Xn] [MPCMn]
                volatile uint8_t *const ucsrna;
                // USART MSPIM n control and signal register B
                // [RXCIEn] [TXCIEn] [UDRIEn] [RXENn] [TXENn] [UCSZn2] [RXB8n] [TXB8n]
                volatile uint8_t *const ucsrnb;
                // USART MSPIM n control and signal register C
                // [UMSELn1] [UMSELn0] [UPMn1] [UPMn0] [USBSn] [UCSZn1] [UCSZn0] [UCPOLn]
                volatile uint8_t *const ucsrnc;
                // USART MSPIM n data register
                volatile uint8_t *const udrn;
                // the data direction register of the port of XCKn pin
                volatile uint8_t *const xckn_port_ddr;
                // the bit mask of XCKn pin in its port
                const uint8_t xckn_port_bit_mask;
                // // the data direction register of the port of MOSIn pin
                // volatile uint8_t *const mosin_port_ddr;
                // // the bit mask of MOSIn pin in its port
                // const uint8_t mosin_port_bit_mask;
                // // the data direction register of the port of MISOn pin
                // volatile uint8_t *const mison_port_ddr;
                // // the bit mask of MISOn pin in its port
                // const uint8_t mison_port_bit_mask;

                #define BIT2BYTE(x) (x>>3)
                #define BIT2BITMASK(x) (1<<(x&0b111))
                // rx buffer
                volatile uint8_t rx_buf [USART_RX_BUFFER_SIZE];
                volatile uint64_t rx_buf_9_bit;
                volatile uint8_t rx_buf_end; // ending index
                volatile uint8_t rx_buf_start; // starting index
                volatile uint8_t rx_buf_full; // rx buffer full flag
                // tx buffer
                volatile uint8_t tx_buf [USART_TX_BUFFER_SIZE];
                volatile uint64_t tx_buf_9_bit;
                volatile uint8_t tx_buf_end; // ending index
                volatile uint8_t tx_buf_start; // starting index
                volatile uint8_t tx_buf_full; // tx buffer full flag

                // a flag to indicate whether the tx has sent data out since begin() is called
                uint8_t tx_used;
            public:
                // default constructor
                Usart (
                    volatile uint16_t *const init_ubrrn,
                    volatile uint8_t *const init_ucsrna,
                    volatile uint8_t *const init_ucsrnb,
                    volatile uint8_t *const init_ucsrnc,
                    volatile uint8_t *const init_udrn,
                    volatile uint8_t *const init_xckn_port_ddr,
                    const uint8_t init_xckn_port_bit_mask//,
                    // volatile uint8_t *const init_mosin_port_ddr,
                    // const uint8_t init_mosin_port_bit_mask,
                    // volatile uint8_t *const init_mison_port_ddr,
                    // const uint8_t init_mison_port_bit_mask
                );

                // this is always true for non-CDC serials
                operator bool () { return true; }
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                // @param baud max: F_CPU / 8
                void begin (const uint32_t baud, const uint8_t config = SERIAL_8N1);
                // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
                // calls pinMode function and config the pin modes
                void begin (Usart_settings settings);
                void end (void);
                int  available (void);
                int  peek (void);
                int  read (void);
                int  availableForWrite (void);
                void flush (void);

                size_t write (uint8_t val);
                size_t write (uint16_t val);
                size_t write (const uint8_t *buffer, size_t size);
                size_t write (const uint16_t *buffer, size_t size);

                using Print::write; // pull in write(str) and write(buf, size) from Print

                // functions to tell the object whether or not to rely on interrupts
                // (interrupts are enabled by default, to maintain compatibility with Arduino)
                void
                    enable_isr (),
                    disable_isr ();
                
                //
                void
                    enable_tx (),
                    disable_tx (),
                    enable_rx (),
                    disable_rx ();

                int (*rx_data_error)(uint16_t received_value, uint8_t ucsrna_err_flags); // pointer to function that is called when error is found
                static int default_rx_data_error (uint16_t received_value, uint8_t ucsrna_err_flags) { return received_value = ucsrna_err_flags = -1; } // drops the error value

                void (*rx_buf_overflow)(uint16_t removed_value); // pointer to function that is called when the rx_buffer overflows
                static void default_rx_buf_overflow (uint16_t removed_value) { removed_value = 0; return; } // do nothing to drop the overflown value

                // these are not intended to be called by users
                void tx_ddr_empty_isr ();
                void rx_isr ();
                void rx_isr_light ();
        };
    }
}

#if defined(UBRRH) || defined(UBRR0H)
extern yh::rec::Usart usart;
#endif

#if defined(UBRR1H)
extern yh::rec::Usart usart1;
#endif

#if defined(UBRR2H)
extern yh::rec::Usart usart2;
#endif

#if defined(UBRR3H)
extern yh::rec::Usart usart3;
#endif

#endif // #ifndef USART_CUSTOM_H
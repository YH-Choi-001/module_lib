#ifndef SPI_SLAVE_H
#define SPI_SLAVE_H _DATE__ ", " __TIME__

#include <Arduino.h>
#include <SPI.h>

namespace yh {
    namespace rec {
        class SPI_slave {
            private:
                static volatile uint8_t *ss_in_reg;
                static uint8_t ss_mask;
            public:
                // enables the usage of hardware SPI
                inline static void begin () {
                    pinMode(SCK, INPUT);   // auto-overrided by hardware SPI
                    pinMode(MOSI, INPUT);  // auto-overrided by hardware SPI
                    pinMode(MISO, OUTPUT); // user-defined
                    pinMode(SS, INPUT);    // auto-overrided by hardware SPI
                    ss_in_reg = portInputRegister(digitalPinToPort(SS));
                    ss_mask = digitalPinToBitMask(SS);
                    uint8_t oldSREG = SREG;
                    noInterrupts();
                    SPCR |= (1 << SPE); // SPI enable
                    switch_to_slave();
                    SREG = oldSREG;
                }
                // enables firing of the interrupt SPI_STC_vect
                inline static void enable_interrupt () { SPI.attachInterrupt(); }
                // disables firing of the interrupt SPI_STC_vect
                inline static void disable_interrupt () { SPI.detachInterrupt(); }
                // @brief sets the bit order to LSB first or MSB first
                inline static void set_bit_order (const uint8_t order) { SPI.setBitOrder(order); }
                // @brief switches the SPI to master mode
                inline static void switch_to_master () { SPCR |= (1 << MSTR); }
                // @brief switches the SPI to slave mode
                inline static void switch_to_slave () { SPCR &= ~(1 << MSTR); }
                // @brief selects the data mode of SPI communication
                inline static void set_data_mode (const uint8_t mode) { SPI.setDataMode(mode); }
                // disables the usage of hardware SPI, and frees the SPI pins
                inline static void end () { SPCR &= ~(1 << SPE); }
                // @return the data received from the previous transaction
                inline static uint8_t read_data () { return SPDR; }
                // @brief writes the data to be sent into the buffer
                inline static void write_data (const uint8_t data) { SPDR = data; }
                // @brief waits until the transaction is completed
                inline static void wait_for_trans_complete () { while (!(SPSR & (1 << SPIF))) {} }
                // @return the status of SS pin
                inline static uint8_t ss_status () { return ( ((*ss_in_reg) & ss_mask) ? 1 : 0 ); }
        };
    }
}

yh::rec::SPI_slave spi_slave;

#endif // #ifndef SPI_SLAVE_H
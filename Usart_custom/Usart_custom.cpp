#ifndef USART_CUSTOM_CPP
#define USART_CUSTOM_CPP __DATE__ ", " __TIME__

#include "Usart_custom.h"

yh::rec::Usart::Usart (
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
) :
    ubrrn(init_ubrrn),
    ucsrna(init_ucsrna),
    ucsrnb(init_ucsrnb),
    ucsrnc(init_ucsrnc),
    udrn(init_udrn),
    xckn_port_ddr(init_xckn_port_ddr),
    xckn_port_bit_mask(init_xckn_port_bit_mask),
    rx_data_error(0),
    rx_buf_overflow(0)
{
    //
}
void yh::rec::Usart::begin (const uint32_t baud, const uint8_t config) {
    tx_used = 0;
    // clear rx buf
    for (uint8_t i = 0; i < USART_RX_BUFFER_SIZE; i++) {
        rx_buf[i] = 0;
    }
    rx_buf_9_bit = 0;
    rx_buf_end = rx_buf_start = 0;
    rx_buf_full = 0;
    // clear tx buf
    for (uint8_t i = 0; i < USART_TX_BUFFER_SIZE; i++) {
        tx_buf[i] = 0;
    }
    tx_buf_9_bit = 0;
    tx_buf_end = tx_buf_start = 0;
    tx_buf_full = 0;
    // pre-assign values to UCSRnX registers
    uint8_t
        ucsrna_temp =
            (0 << RXCn)  | // rx complete flag no need to be cleared
            (1 << TXCn)  | // clear tx complete flag
            (0 << UDREn) | // not to write data register empty flag
            (0 << FEn)   | // clear frame error flag
            (0 << DORn)  | // clear data overrun flag
            (0 << UPEn)  | // clear parity error flag
            (0 << U2Xn)  | // double speed flag to be set in baud rate config part
            (0 << MPCMn),  // clear multi-master flag by default (currently not supported)
        ucsrnb_temp =
            (1 << RXCIEn) | // enable RXCn flag to generate interrupt by default
            (0 << TXCIEn) | // disable TXCn flag to generate interrupt by default (useless)
            (1 << UDRIEn) | // enable UDREn flag to generate interrupt by default
            (1 << RXENn)  | // enable receiver by default
            (1 << TXENn)  | // enable transmitter by default
            (0 << UCSZn2) | // 9-bit flag to be set in data frame config part
            (0 << RXB8n)  | // write a 0 to bits that have no writing permission
            (0 << TXB8n),   // clear 8th bit of 9-bit mode
        ucsrnc_temp = (config & 0x3E);
    double ubrrn_temp;
    // set baud rate
    if (baud > (F_CPU / 16UL)) { // async double speed mode
        ubrrn_temp = F_CPU / 8 / static_cast<double>(baud) - 1;
        // enable async double speed mode
        ucsrna_temp |= (1 << U2Xn);
    } else { // async normal mode
        ubrrn_temp = F_CPU / 16 / static_cast<double>(baud) - 1;
        // disable async double speed mode
        // -----default selected: ucsrna_temp &= ~(1 << U2Xn);
    }
    uint8_t oldSREG = SREG;
    noInterrupts();
    (*ucsrna) = ucsrna_temp;
    (*ucsrnb) = ucsrnb_temp;
    (*ucsrnc) = ucsrnc_temp;
    (*ubrrn) = (ubrrn_temp < 0) ? static_cast<uint16_t>(0) : static_cast<uint16_t>(ceil(ubrrn_temp));
    SREG = oldSREG;
}

void yh::rec::Usart::begin (Usart_settings settings) {
    tx_used = 0;
    // clear rx buf
    for (uint8_t i = 0; i < USART_RX_BUFFER_SIZE; i++) {
        rx_buf[i] = 0;
    }
    rx_buf_9_bit = 0;
    rx_buf_end = rx_buf_start = 0;
    // clear tx buf
    for (uint8_t i = 0; i < USART_TX_BUFFER_SIZE; i++) {
        tx_buf[i] = 0;
    }
    tx_buf_9_bit = 0;
    tx_buf_end = tx_buf_start = 0;
    if (settings.ucsrnc & (1 << UMSELn0)) { // sync mode
        if (settings.master_mode) {
            (*xckn_port_ddr) |= (xckn_port_bit_mask);
        } else {
            (*xckn_port_ddr) &= (~xckn_port_bit_mask);
        }
    }
    uint8_t oldSREG = SREG;
    noInterrupts();
    (*ucsrna) = settings.ucsrna;
    (*ucsrnb) = settings.ucsrnb;
    (*ucsrnc) = settings.ucsrnc;
    (*ubrrn) = settings.ubrrn;
    SREG = oldSREG;
}

void yh::rec::Usart::end () {
    if (tx_used)
        // wait for last transmission to finish
        flush();
    (*ucsrna) = 0;
    (*ucsrnb) = 0;
    (*ucsrnc) = 0;
    (*ubrrn) = 0;
}

#define RX_INT_ABLE ((SREG & (1 << SREG_I)) && ((*ucsrnb) & (1 << RXCIEn)))
#define TX_INT_ABLE (SREG & (1 << SREG_I))

int yh::rec::Usart::available () {
    if (RX_INT_ABLE) {
        // RXC is able to generate interrupts
        if (rx_buf_full) {
            return USART_RX_BUFFER_SIZE;
        } else {
            uint8_t oldSREG = SREG;
            noInterrupts();
            uint8_t unread_len = USART_RX_BUFFER_SIZE + rx_buf_end - rx_buf_start;
            SREG = oldSREG;
            return (unread_len >= USART_RX_BUFFER_SIZE) ? (unread_len - USART_RX_BUFFER_SIZE) : unread_len;
        }
    } else {
        // RXC is unable to generate interrupts
        return ((*ucsrna) & (1 << RXCn)) ? 1 : 0;
    }
}

int yh::rec::Usart::peek () {
    if (!RX_INT_ABLE) return -1; // peek feature unavailable in no-interrupts evnironment
    const uint8_t rx_buf_start_temp = rx_buf_start;
    if ((rx_buf_start_temp == rx_buf_end) && (!rx_buf_full))
        return -1;
    uint16_t temp = rx_buf[rx_buf_start_temp];
    if ((*ucsrnb) & (1 << UCSZn2)) {
        if (rx_buf_9_bit & (static_cast<uint64_t>(1UL) << rx_buf_start_temp)) {
            temp |= 0x0100;
        }
    }
    return temp;
}

int yh::rec::Usart::read () {
    if (RX_INT_ABLE) {
        uint8_t oldSREG = SREG;
        noInterrupts();
        uint8_t rx_buf_start_temp = rx_buf_start;
        // RXC is able to generate interrupts
        if ((rx_buf_start_temp == rx_buf_end) && (!rx_buf_full))
            return -1;
        uint16_t temp = rx_buf[rx_buf_start_temp];
        if ((*ucsrnb) & (1 << UCSZn2)) {
            if (rx_buf_9_bit & (static_cast<uint64_t>(1UL) << rx_buf_start_temp)) {
                temp |= 0x0100;
            }
        }
        rx_buf_start_temp++;
        if (rx_buf_start_temp >= USART_RX_BUFFER_SIZE) {
            rx_buf_start_temp = 0;
        }
        rx_buf_start = rx_buf_start_temp;
        rx_buf_full = 0;
        SREG = oldSREG;
        return static_cast<int>(temp);
    } else {
        // RXC is unable to generate interrupts
        while ((*ucsrna) & (1 << RXCn)) {} // wait for the byte to be received
        const uint8_t ucsrnb_val = (*ucsrnb);
        const uint8_t temp = (*udrn);
        if (ucsrnb_val & (1 << UCSZn2)) { // 9-bit data package
            if (ucsrnb_val & (1 << RXB8n)) {
                return 0x0100 | temp;
            }
        }
        return temp;
    }
}

int yh::rec::Usart::availableForWrite () {
    if (TX_INT_ABLE) {
        // UDRE is able to generate interrupts
        if (tx_buf_full) {
            return 0;
        } else {
            uint8_t oldSREG = SREG;
            noInterrupts();
            uint8_t empty_len = USART_TX_BUFFER_SIZE + tx_buf_start - tx_buf_end;
            SREG = oldSREG;
            return (empty_len > USART_TX_BUFFER_SIZE) ? (empty_len - USART_TX_BUFFER_SIZE) : empty_len;
        }
    } else {
        // UDRE is unable to generate interrupts
        return ((*ucsrna) & (1 << UDREn)) ? 1 : 0;
    }
}

void yh::rec::Usart::flush () {
    if (TX_INT_ABLE) {
        // UDRE is able to generate interrupts
        while ((tx_buf_start != tx_buf_end) || tx_buf_full) {}
    } else {
        // UDRE is unable to generate interrupts
        // wait for transmit complete
        while (!((*ucsrna) & (1 << TXCn))) {}
    }
}

size_t yh::rec::Usart::write (uint8_t val) {
    tx_used = 1;
    if (!TX_INT_ABLE) {
        // UDRE is unable to generate interrupts
        // wait for data register to be empty
        while (!((*ucsrna) & (1 << UDREn))) {}
    }
    const uint8_t ucsrnb_val = (*ucsrnb);
    if ((*ucsrna) & (1 << UDREn)) {
        // data register is empty
        uint8_t oldSREG = SREG;
        noInterrupts();
        // write to UDRn directly
        if (ucsrnb_val & (1 << UCSZn2))
            (*ucsrnb) = ucsrnb_val & ~(1 << TXB8n);
        (*udrn) = val;
        SREG = oldSREG;
    } else {
        // data register is not empty
        // enable data-register-empty interrupt
        (*ucsrnb) = (ucsrnb_val | (1 << UDRIEn));
        // wait for completely-filled tx buf to give a byte of space
        while (tx_buf_full) {}
        uint8_t oldSREG = SREG;
        noInterrupts();
        // load the value to tx buffer
        tx_buf[tx_buf_end] = val;
        if (ucsrnb_val & (1 << UCSZn2)) {
            // 9-bit mode
            tx_buf_9_bit &= ~(static_cast<uint64_t>(1UL) << tx_buf_end);
        }
        tx_buf_end++;
        if (tx_buf_end >= USART_TX_BUFFER_SIZE) {
            tx_buf_end = 0;
        }
        if (tx_buf_end == tx_buf_start) {
            tx_buf_full = 1;
        }
        SREG = oldSREG;
    }
    return 1;
}

size_t yh::rec::Usart::write (uint16_t val) {
    tx_used = 1;
    if (!TX_INT_ABLE) {
        // UDRE is unable to generate interrupts
        // wait for data register to be empty
        while (!((*ucsrna) & (1 << UDREn))) {}
    }
    const uint8_t ucsrnb_val = (*ucsrnb);
    if ((*ucsrna) & (1 << UDREn)) {
        // data register is empty
        uint8_t oldSREG = SREG;
        noInterrupts();
        // write to UDRn directly
        if (ucsrnb_val & (1 << UCSZn2))
            (*ucsrnb) = ( (val & 0x0100) ? (ucsrnb_val | (1 << TXB8n)) : (ucsrnb_val & ~(1 << TXB8n)) );
        (*udrn) = val;
        SREG = oldSREG;
    } else {
        // data register is not empty
        // enable data-register-empty interrupt
        (*ucsrnb) = (ucsrnb_val | (1 << UDRIEn));
        // wait for completely-filled tx buf to give a byte of space
        while (tx_buf_full) {}
        uint8_t oldSREG = SREG;
        noInterrupts();
        // load the value to tx buffer
        tx_buf[tx_buf_end] = val;
        if (ucsrnb_val & (1 << UCSZn2)) {
            // 9-bit mode
            (val & 0x0100) ?
                (tx_buf_9_bit |= (static_cast<uint64_t>(1UL) << tx_buf_end))
            :
                (tx_buf_9_bit &= ~(static_cast<uint64_t>(1UL) << tx_buf_end));
        }
        tx_buf_end++;
        if (tx_buf_end >= USART_TX_BUFFER_SIZE) {
            tx_buf_end = 0;
        }
        if (tx_buf_end == tx_buf_start) {
            tx_buf_full = 1;
        }
        SREG = oldSREG;
    }
    return 1;
}

size_t yh::rec::Usart::write (const uint8_t *buffer, const size_t size) {
    size_t words_sent = 0;
    for (uint8_t i = 0; i < size; i++) {
        words_sent += write(buffer[i]);
    }
    return words_sent;
}

size_t yh::rec::Usart::write (const uint16_t *buffer, const size_t size) {
    size_t words_sent = 0;
    for (uint8_t i = 0; i < size; i++) {
        words_sent += write(buffer[i]);
    }
    return words_sent;
}

void yh::rec::Usart::enable_isr () {
    (*ucsrnb) |= (1 << RXCIEn) | (1 << UDRIEn);
}

void yh::rec::Usart::disable_isr () {
    (*ucsrnb) &= ~(1 << RXCIEn) | (1 << UDRIEn);
}

void yh::rec::Usart::enable_tx () {
    (*ucsrnb) |= (1 << TXENn);
}

void yh::rec::Usart::disable_tx () {
    (*ucsrnb) &= ~(1 << TXENn);
}

void yh::rec::Usart::enable_rx () {
    (*ucsrnb) |= (1 << RXENn);
}

void yh::rec::Usart::disable_rx () {
    (*ucsrnb) &= ~(1 << RXENn);
}

void yh::rec::Usart::tx_ddr_empty_isr () {
    uint8_t tx_buf_start_temp = tx_buf_start;
    if ((tx_buf_end == tx_buf_start_temp) && (!tx_buf_full)) {
        (*ucsrnb) &= (~(1 << UDRIEn)); // tx buf empty, disable the udr interrupt
        return;
    }
    const uint8_t ucsrnb_val = (*ucsrnb);
    if (ucsrnb_val & (1 << UCSZn2)) { // 9-bit data package
        if (tx_buf_9_bit & (static_cast<uint64_t>(1UL) << tx_buf_start_temp)) {
            // set the ninth-bit
            (*ucsrnb) = ucsrnb_val | (1 << TXB8n);
        } else {
            (*ucsrnb) = ucsrnb_val & ~(1 << TXB8n);
        }
    }
    (*udrn) = tx_buf[tx_buf_start_temp];
    // increments the writing index
    tx_buf_start_temp++;
    if (tx_buf_start_temp >= USART_RX_BUFFER_SIZE) {
        tx_buf_start_temp = 0;
    }
    tx_buf_start = tx_buf_start_temp;
    tx_buf_full = 0;
}

void yh::rec::Usart::rx_isr () {
    const uint8_t ucsrna_val = (*ucsrna); // check ucsrna for error flags
    const uint8_t ucsrnb_val = (*ucsrnb);
    uint16_t single_data = (*udrn);
    if (ucsrnb_val & (1 << UCSZn2)) { // 9-bit data package
        if (ucsrnb_val & (1 << RXB8n)) {
            single_data |= 0x0100;
        }
    }
    // check for errors
    const uint8_t err_flags = (ucsrna_val & ((1 << FEn) | (1 << DORn) | (1 << UPEn)));
    int data_to_write = err_flags ? (rx_data_error ? rx_data_error(single_data, err_flags) : -1) : single_data;
    if (data_to_write >= 0) { // save the data
        // storing received data into buffer
        // load volatile mem to register
        uint8_t rx_buf_end_temp = rx_buf_end;
        uint8_t rx_buf_start_temp = rx_buf_start;
        if (rx_buf_full) {
            if (rx_buf_overflow) {
                uint16_t temp = rx_buf[rx_buf_start_temp];
                if ((*ucsrnb) & (1 << UCSZn2)) {
                    if (rx_buf_9_bit & (static_cast<uint64_t>(1UL) << rx_buf_start_temp)) {
                        temp |= 0x0100;
                    }
                }
                rx_buf_overflow(temp);
            }
            rx_buf_start_temp++;
            if (rx_buf_start_temp >= USART_RX_BUFFER_SIZE) {
                rx_buf_start_temp = 0;
            }
        }
        if (ucsrnb_val & (1 << UCSZn2)) { // 9-bit data package
            if (data_to_write & (1 << 8)) {
                // set the ninth-bit
                rx_buf_9_bit |= (static_cast<uint64_t>(1UL) << rx_buf_end_temp);
            } else {
                // clear the ninth-bit
                rx_buf_9_bit &= ~(static_cast<uint64_t>(1UL) << rx_buf_end_temp);
            }
        }
        rx_buf[rx_buf_end_temp] = (data_to_write & 0xff);
        // increments the buffer length
        rx_buf_end_temp++;
        if (rx_buf_end_temp >= USART_RX_BUFFER_SIZE) {
            rx_buf_end_temp = 0;
        }
        if (rx_buf_end_temp == rx_buf_start_temp) {
            rx_buf_full = 1;
        }
        rx_buf_end = rx_buf_end_temp;
        rx_buf_start = rx_buf_start_temp;
    }
    if ((*ucsrna) & (1 << RXCn)) {
        // there is unread data in the UDRn
        rx_isr();
    }
}

void yh::rec::Usart::rx_isr_light () {
    // check for errors
    if ((*ucsrna) & ((1 << FEn) | (1 << DORn) | (1 << UPEn)))
        return; // error detected, quit isr
    const uint8_t ucsrnb_val = (*ucsrnb);
    uint8_t single_data = (*udrn);
    // storing received data into buffer
    // load volatile mem to register
    uint8_t rx_buf_end_temp = rx_buf_end;
    uint8_t rx_buf_start_temp = rx_buf_start;
    if (rx_buf_full) {
        // increment rx_buf_start to give space for new data
        rx_buf_start_temp++;
        if (rx_buf_start_temp >= USART_RX_BUFFER_SIZE) {
            rx_buf_start_temp = 0;
        }
    }
    if (ucsrnb_val & (1 << UCSZn2)) { // 9-bit data package
        if (ucsrnb_val & (1 << RXB8n)) {
            // set the ninth-bit
            rx_buf_9_bit |= (static_cast<uint64_t>(1UL) << rx_buf_end_temp);
        } else {
            // clear the ninth-bit
            rx_buf_9_bit &= ~(static_cast<uint64_t>(1UL) << rx_buf_end_temp);
        }
    }
    rx_buf[rx_buf_end_temp] = single_data;
    // increments the buffer length
    rx_buf_end_temp++;
    if (rx_buf_end_temp >= USART_RX_BUFFER_SIZE) {
        rx_buf_end_temp = 0;
    }
    if (rx_buf_end_temp == rx_buf_start_temp) {
        rx_buf_full = 1;
    }
    rx_buf_end = rx_buf_end_temp;
    rx_buf_start = rx_buf_start_temp;
    if ((*ucsrna) & (1 << RXCn)) {
        // there is unread data in the UDRn
        rx_isr_light();
    }
}

#endif // #ifndef USART_CUSTOM_CPP
#include <Usart_custom.h>

void setup () {
//    usart.begin(9600, SERIAL_8N1);
    usart1.begin(yh::rec::Usart_settings(9600));
    usart3.begin(yh::rec::Usart_settings(9600, 9));
    usart.end();
//    usart1.end();
    usart2.end();
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
}

void loop () {
//    const int x = usart1.available();
//    int x;
//    {
//        if ((*usart1.ucsrnb) & (1 << RXCIEn)) {
            // RXC is able to generate interrupts
//            int unread_len = usart1.rx_buf_end + USART_RX_BUFFER_SIZE - usart1.rx_buf_start;
//            x = (unread_len >= USART_RX_BUFFER_SIZE) ? (unread_len - USART_RX_BUFFER_SIZE) : unread_len;
//        }
//        else
//        {
//            // RXC is unable to generate interrupts
//            x = ((*usart1.ucsrna) & (1 << RXCn)) ? 1 : 0;
//        }
//    }
//    usart.write((uint8_t)'\n');
    {
//        uint8_t val = '\n';
//        usart.tx_used = 1;
//        const uint8_t ucsrnb_val = (*(usart.ucsrnb));
//        if (!(ucsrnb_val & (1 << UDRIEn))) {
//            // UDRE is unable to generate interrupts
//            // wait for data register to be empty
//            while (!((*(usart.ucsrna)) & (1 << UDREn))) {}
//        }
//        if ((*(usart.ucsrna)) & (1 << UDREn)) {
            // data register is empty
            // write to UDRn directly
//            if (ucsrnb_val & (1 << UCSZn2))
//                (*(usart.ucsrnb)) = ucsrnb_val & ~(1 << TXB8n);
            // (*(usart.udrn)) = '\n';
            // UDR0 = 0;
//            if (millis() < 5000) {
//                UDR0 = '\n';
//            }
//        }
//        else
//        {
            // data register is not empty
            // load the value to tx buffer
//            usart.tx_buf[usart.tx_buf_end] = val;
//            if (ucsrnb_val & (1 << UCSZn2)) {
//                // 9-bit mode
//                (usart.tx_buf_9_bit) &= ~(static_cast<uint64_t>(1UL) << (usart.tx_buf_start));
//            }
//            (usart.tx_buf_end)++;
//            if ((usart.tx_buf_end) >= USART_TX_BUFFER_SIZE) {
//                (usart.tx_buf_end) = 0;
//            }
//        }
        // return 1;
    }
//    if (x) {
//        int rxed = usart1.read();
//        //UDR0 = '!';
//        usart.write((uint8_t)'!');
//        usart.write((uint8_t)((rxed % 1000) / 100 + '0'));
//        usart.write((uint8_t)((rxed % 100) / 10 + '0'));
//        usart.write((uint8_t)((rxed % 10) + '0'));
//        usart.write((uint8_t)('\n'));
//    }
}
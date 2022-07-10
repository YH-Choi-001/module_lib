#include <Usart_custom.h>

void setup () {
    usart.begin(9600, SERIAL_8N1);
}

void loop () {
    if (usart.available()) {
        usart.print(usart.read());
    }
}
#include <Usart_custom.h>

void setup () {
    usart.begin(9600, SERIAL_8N1);
}

void loop () {
    usart.println("Hello, world!");
    delay(1500);
}
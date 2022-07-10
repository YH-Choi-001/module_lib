#include <Usart_custom.h>

void setup () {
    usart.begin(9600);
}

void loop () {
    usart.println("Hello, world!");
    delay(1500);
}
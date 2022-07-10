#include <Usart_custom.h>

void setup () {
    usart.begin(yh::rec::Usart_settings(9600, 9, 1, 0, 1, 0, 0));
}

void loop () {
    usart.write("Hello, world!");
    delay(5000);
}
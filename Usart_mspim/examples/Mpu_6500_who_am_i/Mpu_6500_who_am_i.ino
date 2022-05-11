#include <Usart_mspim.h>

// connect:
// MPU-6500 AD0/SDO pin to Arduino Uno pin 0 (RX / MISO)
// MPU-6500 SDA/SDI pin to Arduino Uno pin 1 (TX / MOSI)
// MPU-6500 SCL/SCK pin to Arduino Uno pin 4 (XCK)
// MPU-6500 nCS pin to Arduino Uno pin 7 (SS)

// This example flashes the LED on Arduino Uno pin 13 if the who_am_i value received from the SPI slave (which is MPU-6500) is equal to 0x70.
// The LED on Arduino Uno pin 13 won't flash if the who_am_i value received from the SPI slave (which is MPU-6500) is NOT equal to 0x70.
const int chip_select_pin = 7;

void setup () {
    //
    USPI.begin();
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(0, INPUT); // RX
    pinMode(1, OUTPUT); // TX
    pinMode(4, OUTPUT); // XCK
    pinMode(chip_select_pin, OUTPUT); // SS
}

void loop () {
    USPI.beginTransaction(yh::rec::USART_MSPIM_Settings(1000000UL, MSBFIRST, SPI_MODE3));
    digitalWrite(chip_select_pin, LOW);
    USPI.transfer(0x80 | 0x75);
    if (USPI.transfer(0x00) == 0x70) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
    digitalWrite(chip_select_pin, HIGH);
    USPI.endTransaction();
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}
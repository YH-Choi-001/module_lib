#include <Timer_int.h>

uint8_t mask;
volatile uint8_t *reg;

void setup () {
    pinMode(13, OUTPUT);
    mask = digitalPinToBitMask(13);
    reg = portOutputRegister(digitalPinToPort(13));
    setup_timer_1A_interrupt(500000);
}

ISR (TIMER1_COMPA_vect) {
    (*reg) ^= mask;
}

void loop () {
    //
}
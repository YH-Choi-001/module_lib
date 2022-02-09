#include <Timer_int.h>

void setup () {
    Serial.begin(9600);
    setup_timer_1A_interrupt(500000);
}

ISR (TIMER1_COMPA_vect) {
    Serial.print('\n');
}

void loop () {
    //
}
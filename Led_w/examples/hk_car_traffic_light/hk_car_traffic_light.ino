// Please do the following:
// connect Vcc of led to 5V on arduino
// connect Gnd of led to GND on arduino
// connect on/off signal pin of red led to pin 9 on arduino
// connect on/off signal pin of yellow led to pin 10 on arduino
// connect on/off signal pin of green led to pin 11 on arduino

#include <Led_w.h>

yh::rec::Led_w_fast red (9), yellow (10), green (11);

void setup () {
    Serial.begin(9600);
    red.begin();
    yellow.begin();
    green.begin();
}

void loop () {

    red.led_on();
    yellow.led_off();
    green.led_off();

    delay(8000);

    yellow.led_on();

    delay(2000);

    red.led_off();
    yellow.led_off();
    green.led_on();

    delay(8000);

    green.led_off();
    yellow.led_on();

    delay (2000);
}
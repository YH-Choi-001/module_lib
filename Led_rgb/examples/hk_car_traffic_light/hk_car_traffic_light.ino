// Please do the following:
// connect Vcc of led to 5V on arduino
// connect Gnd of led to GND on arduino
// connect on/off signal pin of red led to pin 9 on arduino
// connect on/off signal pin of yellow led to pin 10 on arduino
// connect on/off signal pin of green led to pin 11 on arduino

#include <Led_rgb.h>

yh::rec::Led_rgb_fast led (9, 10, 11);

void setup () {
    Serial.begin(9600);
    led.begin();
}

void loop () {

    led.r_on();
    led.g_off();
    led.b_off();

    delay(8000);

    led.g_on();

    delay(2000);

    led.r_off();

    delay(8000);

    led.r_on();

    delay (2000);
}
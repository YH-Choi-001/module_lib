// Please do the following:
// connect Vcc of led to 5V on arduino
// connect Gnd of led to GND on arduino
// connect on/off signal pin of led to pin 49 on arduino

#include <Led_w.h>

yh::rec::Led_w_fast led (49);

void setup () {
    Serial.begin(9600);
    led.begin();
}

#define PRINT_LED_STATES() \
    Serial.print("w: "); Serial.println(led.led_state());

void loop () {

    // turn on and off led using led_on() and led_off() methods
    led.led_on();
    PRINT_LED_STATES();
    delay(500);
    led.led_off();
    PRINT_LED_STATES();
    delay(500);

    // turn on and off single color of led using set_led(led_state) method
    led.set_led(1);
    PRINT_LED_STATES();
    delay(500);
    led.set_led(0);
    PRINT_LED_STATES();
    delay(500);

    // turn on and off single color of led using toggle_color() method
    led.toggle_led();
    PRINT_LED_STATES();
    delay(500);
    led.toggle_led();
    PRINT_LED_STATES();
    delay(500);
}
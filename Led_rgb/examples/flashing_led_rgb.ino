// Please do the following:
// connect Vcc of led to 5V on arduino
// connect Gnd of led to GND on arduino
// connect r pin of led to pin 46 on arduino
// connect g pin of led to pin 47 on arduino
// connect b pin of led to pin 48 on arduino

#include <Led_rgb.h>

yh::rec::Led_rgb_fast led (46, 47, 48);

void setup () {
    Serial.begin(9600);
    led.begin();
}

#define PRINT_LED_STATES() \
    Serial.print("r: "); Serial.print(led.r_state()); Serial.print(", g: "); Serial.print(led.g_state()); Serial.print(", b: "); Serial.println(led.b_state());

void loop () {

    // turn on and off single color of led using color_on() and color_off() methods
    led.r_on();
    PRINT_LED_STATES();
    delay(500);
    led.r_off();
    PRINT_LED_STATES();
    delay(500);

    // turn on and off single color of led using set_color(color_led_state) method
    led.set_g(1);
    PRINT_LED_STATES();
    delay(500);
    led.set_g(0);
    PRINT_LED_STATES();
    delay(500);

    // turn on and off single color of led using toggle_color() method
    led.toggle_b();
    PRINT_LED_STATES();
    delay(500);
    led.toggle_b();
    PRINT_LED_STATES();
    delay(500);

    // set the on/off state of all 3 colors of led together using set_rgb(...) method
    led.set_rgb(0b00000111);
    PRINT_LED_STATES();
    delay(500);
    led.set_rgb(0, 0, 0);
    PRINT_LED_STATES();
    delay(500);
}
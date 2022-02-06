// Please do the following:
// connect Vcc of led to 5V on arduino
// connect Gnd of led to GND on arduino
// connect on/off signal pin of led to pin 9 on arduino

#include <Led_w.h>

yh::rec::Led_w_analog led (9);

void setup () {
    Serial.begin(9600);
    led.begin();
}

void loop () {
    static bool increase_brightness_trend = true;
    static int brightness = 0;

    if (increase_brightness_trend) {
        if (brightness < 255) {
            led.set_led(++brightness);
        } else {
            increase_brightness_trend = false;
        }
    } else {
        if (brightness > 0) {
            led.set_led(--brightness);
        } else {
            increase_brightness_trend = true;
        }
    }
    Serial.print("w: "); Serial.println(led.get_led_brightness());
}
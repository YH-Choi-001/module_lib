#include <Software_pwm.h>

void setup () {
    software_pwm.attach_pin(13);
    software_pwm.begin();
}

void loop () {
    static uint8_t val = 0;
    static bool up_trend = true;
    if (up_trend) {
        if (val == 255) {
            up_trend = false;
        } else {
            val += 5;
        }
    } else {
        if (val == 0) {
            up_trend = true;
        } else {
            val -= 5;
        }
    }
    software_pwm.analog_write(13, val);
    delay(10);
}
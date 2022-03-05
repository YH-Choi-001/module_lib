#include <Software_pwm.h>

void setup () {
    software_pwm.attach_pin(13);
    software_pwm.begin();
}

void loop () {
    static uint8_t val = 0;
    software_pwm.analog_write(13, val);
    val += 5;
    delay(10);
}
// Please do the following:
// connect signal pin of button to pin 2 on arduino

#include <Button_inverse.h>

// use Button_inverse_fast for a few hundred nanoseconds faster
yh::rec::Button_inverse_fast button (2);

void setup () {
    Serial.begin(9600);
    button.begin();
}

void loop () {
    Serial.println(button.pressed_down() ? "Button is pressed" : "Button is released");
}
// Please do the following:
// connect Vcc of button to 5V on arduino
// connect Gnd of button to GND on arduino
// connect signal pin of button to pin 2 on arduino

#include <Button_inverse.h>

yh::rec::Button_inverse_fast button (2);

void setup () {
    Serial.begin(9600);
    button.begin();
}

void loop () {
    Serial.println(button.pressed_down() ? "Button is pressed" : "Button is released");
}
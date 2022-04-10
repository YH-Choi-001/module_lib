// Connect pin 12 to signal pin of a button of any type.
// This sketch will toggle the Arduino's built-in LED
// every time the button is pressed.

#include <Attach_pcint.h> // No need to include Pcint.h if Attach_pcint.h is included already

const int button_pin = 12;

void toggle_led () {
    digitalWrite(13, !digitalRead(13));
}

void setup () {
    pinMode(button_pin, INPUT);
    pinMode(13, OUTPUT);
    attach_pcint(pin_to_pcint_no(button_pin), toggle_led, RISING); // the state can be set to LOW, CHANGE, FALLING, RISING
}

void loop () {
    //
}
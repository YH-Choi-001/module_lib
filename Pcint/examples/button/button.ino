// Connect pin 13 to signal pin of a button of any type.
// This sketch will print a '!' sign to the serial monitor
// once the state of the digital pin connecting the button is changed.

#include <Attach_pcint.h> // No need to include Pcint.h if Attach_pcint.h is included already

const int button_pin = 13;

void print_newline () {
    Serial.print('!');
}

void setup () {
    Serial.begin(9600);
    pinMode(button_pin, INPUT);
    attach_pcint(pin_to_pcint_no(button_pin), print_newline, CHANGE); // the state can be set to LOW, CHANGE, FALLING, RISING
}

void loop () {
    //
}
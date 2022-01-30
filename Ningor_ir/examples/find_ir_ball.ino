// Please do the following:
// connect the 2 Vcc of nin gor's ir to 2 5V on arduino
// connect the 2 Gnd of nin gor's ir to 2 GND on arduino
// connect the pins on the same column with Vcc and Gnd accordingly to A0 to A11 on arduino

#include <Ningor_ir.h>

const uint8_t ir_pins [] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11};
yh::rec::Ningor_ir ir (ir_pins);

void setup () {
    Serial.begin(9600);
    ir.begin();
}

void loop () {

    // find the angle of ball using get_ball_direction() method
    Serial.print("angle of ball: ");
    Serial.print(ir.get_ball_direction());

    // find the index of the max eye using idx_of_max_eye() method
    Serial.print(", max eye: ");
    Serial.print(ir.idx_of_max_eye(false));

    // find the reading of the max eye using val_of_max_eye() method
    Serial.print(", reading of max eye: ");
    Serial.print(ir.val_of_max_eye(false));
}
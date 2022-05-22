#include <Joinmax_compoI.h>

yh::rec::CompoI eyes;

void setup () {
    Serial.begin(9600);
    eyes.begin();
}

void loop () {
    eyes.set_filter_off();
    Serial.print("actual: ");
    Serial.print(eyes.get_channel_val(1));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(2));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(3));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(4));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(5));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(6));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(7));
    Serial.print('\n');
    eyes.set_filter_on();
    Serial.print("filter: ");
    Serial.print(eyes.get_channel_val(1));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(2));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(3));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(4));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(5));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(6));
    Serial.print('\t');
    Serial.print(eyes.get_channel_val(7));
    Serial.print('\n');
    Serial.print("\n\n\n");
}
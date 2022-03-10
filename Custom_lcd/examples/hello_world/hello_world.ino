#include <Custom_lcd.h>

Custom_lcd lcd (11, 10, 9, 8, 7, 6, 5, 4, 3, 2);

void setup () {
    lcd.begin(16, 2);
    lcd.setCursor(2, 0);
    lcd.print("Hello, world!");
}

void loop () {
    //
}
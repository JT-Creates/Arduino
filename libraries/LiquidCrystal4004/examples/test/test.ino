#include <LiquidCrystal4004.h>

const int en1 = 12, en2 = 7; LiquidCrystal4004 lcd(13, en1, en2, 11, 10, 9, 8);
void setup() {
    lcd.begin(40,4);
 // lcd.print("hello, world! ");
}

void loop() {
   // lcd1.setCursor(0,0);
    lcd.print("Hello, there!        ");
    delay(1);
    lcd.print("This is a test!      ");
    delay(1);
    lcd.print("Testing 4004.        ");
    delay(1);
    lcd.print("Did it work?         ");
    delay(1000);
}

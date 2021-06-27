#include <LiquidCrystal4004.h>

const int en1 = 12, en2 = 7;
LiquidCrystal4004 lcd(13, en1, en2, 11, 10, 9, 8);
void setup() {
    lcd.begin(40,4);
    lcd.print("hello, world! ABCDEFGHIJKLMNOPQRSTUVWXYZ acbdefghijklmnopqrstuvwxyz 1234567890 `~!@#$%^&*()_+[]|}{:'\"\\/.,?>< ");
}

void loop() {
    // lcd.setCursor(0,2);
   //          012345678901234567890
   //          ---------------------
    delay(100);
    lcd.print("Hello, there!        ");
    delay(1);
    lcd.print("This is a test!      ");
    delay(1);
    lcd.print("Testing 4004.        ");
    delay(1);
    lcd.print("Did it work?         ");
    delay(2000);
}

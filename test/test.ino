#include <LiquidCrystal4004.h>

LiquidCrystal4004 lcd(13, 12, 7, 11, 10, 9, 8);
void setup() {
    lcd.begin(40,4);
    lcd.print("  ");
    lcd.print("hello, world! ABCDEFGHIJKLMNOPQRSTUVWXYZ acbdefghijklmnopqrstuvwxyz 1234567890 `~!@#$%^&*()_+[]|}{:'\"\\/.,?>< ");
}

void loop() {
   // lcd.print("");
   // lcd.setCursor(30,3);
   //          012345678901234567890
   //          ---------------------
    delay(128);
    lcd.print("Hello, there!       ");
   // delay(1);
    lcd.print("This is a test!     ");
   // delay(1);
    lcd.print("Testing 4004.       ");
   // delay(1);
    lcd.print("Did it work?       ");
   // delay(2000);
}

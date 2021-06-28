#include <LiquidCrystal4004.h>

LiquidCrystal4004 lcd(13, 12, 7, 11, 10, 9, 8);
void setup() {
    lcd.begin(40,4);
    lcd.print("Hello, there!       ");
    delay(100);
    lcd.print("                    ");
    delay(100);
    lcd.print("This is a test!     ");
    delay(100);
    lcd.print("                    ");
    delay(100);
   // lcd.print("  ");
   // lcd.print("hello, world! ABCDEFGHIJKLMNOPQRSTUVWXYZ acbdefghijklmnopqrstuvwxyz 1234567890 `~!@#$%^&*()_+[]|}{:'\"\\/.,?>< ");
}

void loop() {
   // lcd.print(" ");
   // lcd.setCursor(30,3);
   //          012345678901234567890
   //          ---------------------
   // delay(2000);
    lcd.print("Testing row 1.      ");
    delay(10);
    lcd.print("Did row 1 work?     ");
    delay(1000);
    lcd.print("Testing row 2.      ");
    delay(10);
    lcd.print("Did row 2 work?     ");
    delay(1000);
    lcd.print("Testing row 3.      ");
    delay(10);
    lcd.print("Did row 3 work?     ");
    delay(1000);
    lcd.print("Testing row 4.      ");
    delay(10);
    lcd.print("Did row 4 work?     ");
   // delay(2000);
}

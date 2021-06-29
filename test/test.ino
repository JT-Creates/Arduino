#include <LiquidCrystal4004.h>

LiquidCrystal4004 lcd(13, 12, 7, 11, 10, 9, 8);
void setup() {
  lcd.begin(40,4);
 //          0123456789012345678901234567890123456789
 //          ----------------------------------------
  lcd.print("Hello, there!                           ");
  lcd.print("This is a test!                         ");
  lcd.print("                                        ");
  lcd.print("                                        ");
  delay(2000);
  lcd.print("Testing row 1.      Did row 1 work?     ");
  lcd.print("Testing row 2.      Did row 2 work?     ");
  lcd.print("Testing row 3.      Did row 3 work?     ");
  lcd.print("Testing row 4.      Did row 4 work?     ");
  delay(2000);
 // lcd.print("  ");
 // lcd.print("hello, world! ABCDEFGHIJKLMNOPQRSTUVWXYZ acbdefghijklmnopqrstuvwxyz 1234567890 `~!@#$%^&*()_+[]|}{:'\"\\/.,?>< ");
}

void loop() {
 //          0123456789012345678901234567890123456789
 //          ----------------------------------------
 // delay(200);
 // lcd.print(" ");
 // lcd.setCursor(30,3);
  lcd.print("-");
  delay(500);
}

#include <LiquidCrystal4004.h>

LiquidCrystal4004 lcd(13, 12, 7, 11, 10, 9, 8);
void setup() {
  lcd.begin(40, 4);
  lcd.cursor();
  //         0123456789012345678901234567890123456789
  //         ----------------------------------------
  lcd.print("Hello, there!                           ");
  lcd.print("This is a test of cursor control!       ");
  lcd.print("                                        ");
  lcd.print("                                        ");
  delay(2000);
}
int z = 0;
void loop() {
  //         0123456789012345678901234567890123456789
  //         ----------------------------------------
  //delay(200);
  //lcd.setCursor(30,3);
  z++;
  for(int i = 0; i < 160; i++) {
    lcd.print(char(z));
    delay(50);
  }
  delay(500); 
}

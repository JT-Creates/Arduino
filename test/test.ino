#include <LiquidCrystal4004.h>

LiquidCrystal4004 lcd(13, 12, 7, 11, 10, 9, 8);
void setup() {
  Serial.begin(9600);
  lcd.begin(40,4);
 // lcd.print("Hello, there!       " + "                    ");
 // lcd.print("This is a test!     " + "                    ");
  delay(100);
 // lcd.print("  ");
 // lcd.print("hello, world! ABCDEFGHIJKLMNOPQRSTUVWXYZ acbdefghijklmnopqrstuvwxyz 1234567890 `~!@#$%^&*()_+[]|}{:'\"\\/.,?>< ");
}

void loop() {
 // lcd.print(" ");
 // lcd.setCursor(30,3);
 //          012345678901234567890
 //          ---------------------
  delay(200);
  lcd.print("Testing row 1.      ");
  lcd.print("Did row 1 work?     ");
  Serial.print("----");
  lcd.print("Testing row 2.      ");
  lcd.print("Did row 2 work?     ");
  Serial.print("----");
  lcd.print("Testing row 3.      ");
  lcd.print("Did row 3 work?     ");
  Serial.print("----");
  lcd.print("Testing row 4.      ");
  lcd.print("Did row 4 work?     ");
  Serial.print("----");
 // delay(200);
  //text();
}

void text(){
  String text = Serial.readString();
  for(int i = 0; i < text.length(); i++) {
    //here you could check the servo number
    String pos = text.substring(i, i + 1);
    Serial.println("Pos: " + i);
    Serial.println(pos);
  }
}

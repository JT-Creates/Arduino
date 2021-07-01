#include <LiquidCrystal4004.h>

LiquidCrystal4004 lcd(13, 12, 7, 11, 10, 9, 8);
int JoyStick_X = 0; //x
int JoyStick_Y = 1; //y
int JoyStick_Z = 3; //key
void setup(){
  pinMode(JoyStick_Z, INPUT); 
  Serial.begin(9600); // 9600 bps
  lcd.begin(40, 4);
  lcd.cursor();
  //         0123456789012345678901234567890123456789
  //         ----------------------------------------
  lcd.print("----------------------------------------");
  lcd.print("|            Hello there!              |");
  lcd.print("|       This is a cursor test!         |");
  lcd.print("----------------------------------------");
  delay(2000);
}
int z = 0, xi = 0, yi = 0, zi = 0, xf, yf, zf, x, y;
void loop() {
  //         0123456789012345678901234567890123456789
  //         ----------------------------------------
  //delay(200);
  //lcd.setCursor(30,3);
  z++;
  for(int i = 0; i < 160; i++) {
   // lcd.print(char(z));
   // delay(50);
  }
  //delay(500); 
  xf = analogRead(JoyStick_X)/10;
  yf = analogRead(JoyStick_Y)/10;
  zf = digitalRead(JoyStick_Z)/10;
  x = xf - xi;
  y = yf - yi;
  z = zf - zi;
  xi = xf;
  yi = yf;
  zi = zf;
  //Serial.print(x ,DEC);
  //Serial.print(",");
  //Serial.print(y ,DEC);
  //Serial.print(",");
  //Serial.println(z ,DEC);
  if (z != 0) {
 //   Serial.print(lcd.cursor_pos());
  }
  delay(100);
  
}

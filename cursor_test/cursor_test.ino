#include <LiquidCrystal4004.h>

LiquidCrystal4004 lcd(13, 12, 7, 11, 10, 9, 8);
int JoyStick_X = 0; //x
int JoyStick_Y = 1; //y
int JoyStick_Z = 3; //key
int z = 0, xi = 0, yi = 0, xf = 0, yf = 0, x, y, cx = 0, cy = 0;
void setup(){
  pinMode(JoyStick_Z, INPUT); 
  cx = analogRead(sqrt(JoyStick_X));
  cy = analogRead(sqrt(JoyStick_Y));
  Serial.begin(9600); // 9600 bps
  lcd.begin(40, 4);
  lcd.cursor();
  //         0123456789012345678901234567890123456789
  //         ----------------------------------------
  lcd.print("----------------------------------------");
  lcd.print("|            Hello there!              |");
  lcd.print("|       This is a cursor test!         |");
  lcd.print("----------------------------------------");
  delay(2);
}
int v1 = 0, v2 = 0, v3 = 0;
void loop() {
  //         0123456789012345678901234567890123456789
  //         ----------------------------------------
  for(int i = 0; i < 160; i++) {
   // lcd.print(char(z));
   // delay(50);
  }
  xf = sqrt(analogRead(JoyStick_X)) - cx;
  yf = sqrt(analogRead(JoyStick_Y)) - cy; 
  x = xf - xi;
  y = yf - yi;
  xi = xf;
  yi = yf;
  if (abs(x) > v2) v2 = x;
  if (abs(y) > v3) v3 = y;
  while (abs(x) > 2 | abs(y) > 2) {
    delay(400);
    //break;
    xf = sqrt(analogRead(JoyStick_X)) - cx;
    yf = sqrt(analogRead(JoyStick_Y)) - cy; 
    x = xf - xi;
    y = yf - yi;
    if (abs(x) > v2) v2 = x;
    if (abs(y) > v3) v3 = y;
    Serial.println(x+y);
    xi = xf;
    yi = yf;
    //Serial.print(x ,DEC);
    //Serial.print(",");
    //Serial.println(y ,DEC);
    //Serial.print(",");
    //Serial.println(z ,DEC);
    if (abs(x) < 2 | abs(y) < 2) break;
  }
  cx = analogRead(sqrt(JoyStick_X));
  cy = analogRead(sqrt(JoyStick_Y));
  z = digitalRead(JoyStick_Z);
  if (z != 0) {
 //   Serial.print(lcd.cursor_pos());
  }
  v1 += v2;
  if ((v1 != lcd.cursorPos()-1) & (v1 != lcd.cursorPos()) & (v1 != lcd.cursorPos()+1)) {
    Serial.println("cell");
    Serial.println(lcd.cursorPos());
    Serial.println("line");
    Serial.println(lcd.cursorPos() % 4);
    Serial.println("columm");
    Serial.println(lcd.cursorPos() - (40 * (lcd.cursorPos() % 4)));
    v1 = lcd.cursorPos();
    if (x != v2 | y != v3){
      lcd.setCursor(v1 + v2-1,v3);
      v2 = 0;
      v3 = 0;
    }
    else {
      if (abs(x) > v2) v2 = x;
      if (abs(y) > v3) v3 = y;
    }
    delay(1000);
  }
}

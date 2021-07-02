#include <LiquidCrystal4004.h>

LiquidCrystal4004 lcd(13, 12, 7, 11, 10, 9, 8);
int JoyStick_X = 0; //x
int JoyStick_Y = 1; //y
int JoyStick_Z = 3; //key
int z = 0, xi = 0, yi = 0, xf = 0, yf = 0, x, y, cx = 0, cy = 0;
void setup(){
  pinMode(JoyStick_Z, INPUT); 
  cx = analogRead((JoyStick_X));
  cy = analogRead((JoyStick_Y));
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
  xf = (analogRead(JoyStick_X)) - cx;
  yf = (analogRead(JoyStick_Y)) - cy; 
  x = xf - xi;
  y = yf - yi;
  xi = xf;
  yi = yf;
  if (x > 10 | y > 10 | x < -10 | y < -10){
    while (abs(x) > v2 | abs(y) > v3) {
      delay(400);
      //break;
      xf = (analogRead(JoyStick_X)) - cx;
      yf = (analogRead(JoyStick_Y)) - cy; 
      x = xf - xi;
      y = yf - yi;
      if (abs(x) > v2) {
        v2 = x;
        if (abs(y) > v3) v3 = y;
        else break;
      }
      else break;
      if (abs(y) > v3) {
        v2 = x;
        if (abs(x) > v2) v2 = 2;
        else break;
      }
      else break;
      Serial.println(x+y);
      xi = xf;
      yi = yf;
      //Serial.print(x ,DEC);
      //Serial.print(",");
      //Serial.println(y ,DEC);
      //Serial.print(",");
      //Serial.println(z ,DEC);
      cx = analogRead((JoyStick_X));
      cy = analogRead((JoyStick_Y));
    }
    z = digitalRead(JoyStick_Z);
    if (abs(x) > abs(v2)) v2 = x;
    if (abs(y) > abs(v3)) v3 = y;
    if (z == 0) {
      int dir = JoyStickDirection();
      Serial.println(dir);
      if (dir == 1) lcd.goto_Cell(lcd.cursorPos() + 1);
      else if (dir == 3) lcd.goto_Cell(lcd.cursorPos() - 1);
      else if (dir == 2) lcd.goto_Cell(lcd.cursorPos() + 40);
      else if (dir == 4) lcd.goto_Cell(lcd.cursorPos() - 40);
      //Serial.print(lcd.cursorPos());
    }
  }
  delay(100);
}

void charSelect(){
  do {
    return 1;
  } while (1);
}

int JoyStickDirection(){
  if (abs(v2) > abs(v3)){
    if (abs(v2) > 0) {
      return 1;
    } else if (abs(v2) < 0) {
      return 3;
    }
  } else if (abs(v3) > abs(v2)) {
    if (abs(v3) > 0) {
      return 2;
    } else if (abs(v3) < 0) {
      return 4;
    }
  }
  return 0;
}

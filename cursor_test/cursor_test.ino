#include <LiquidCrystal4004.h>

LiquidCrystal4004 lcd(13, 12, 7, 11, 10, 9, 8);
int JoyStick_X = 0; //x
int JoyStick_Y = 1; //y
int JoyStick_Z = 3; //key
int z = 0, x, y, cx = 0, cy = 0;
void setup(){
  pinMode(JoyStick_Z, INPUT); 
  Serial.begin(9600); // 9600 bps
  //cx = analogRead(JoyStick_X);
  //cy = analogRead(JoyStick_Y);
  lcd.begin(40, 4);
  lcd.cursor();
  lcd.blink();
  //         0123456789012345678901234567890123456789
  //         ----------------------------------------
  lcd.print("----------------------------------------");
  lcd.print("|            Hello there!              |");
  lcd.print("|       This is a cursor test!         |");
  lcd.print("----------------------------------------");
}
void loop() {
  //         0123456789012345678901234567890123456789
  //         ----------------------------------------
  x = analogRead(JoyStick_X);
  y = analogRead(JoyStick_Y); 
  if (x > 10 | y > 10 | x < -10 | y < -10){
    int dir = JoyStickDirection();
    Serial.println(dir);
    if (dir == 1) lcd.goto_Cell(lcd.cursorPos() + 1);
    else if (dir == 3) lcd.goto_Cell(lcd.cursorPos() - 1);
    else if (dir == 2) lcd.goto_Cell(lcd.cursorPos() + 40);
    else if (dir == 4) lcd.goto_Cell(lcd.cursorPos() - 40);
    delay((abs(x)+1)/(abs(y)+1)*10);
    //Serial.print(lcd.cursorPos());
  }
    cx = analogRead(JoyStick_X);
    cy = analogRead(JoyStick_Y);
}

void charSelect(){
  do {
    for(int i = 0; i < 160; i++) {
     // lcd.print(char(z));
     // delay(50);
    }
  } while (1);
}

int JoyStickDirection(){
  if (x > y){
    if (x > 1) {
      return 1;
    } else if (x < -1) {
      return 3;
    }
  } else if (y > x) {
    if (y > 1) {
      return 2;
    } else if (y < -1) {
      return 4;
    }
  }
  return 0;
}

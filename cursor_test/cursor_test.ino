#include <LiquidCrystal4004.h>

LiquidCrystal4004 lcd(13, 12, 7, 11, 10, 9, 8);
int JoyStick_X = 0; //x
int JoyStick_Y = 1; //y
int JoyStick_Z = 3; //key
int z = 0, dx = 0, dy = 0, x, y, cx = 0, cy = 0;
void setup(){
  pinMode(JoyStick_Z, INPUT); 
  Serial.begin(9600); // 9600 bps
  cx = analogRead(JoyStick_X);
  cy = analogRead(JoyStick_Y);
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
  dx = x - cx;
  dy = y - cy;
  Serial.print("x: ");
  Serial.println(x);
  Serial.print("y: ");
  Serial.println(y);
  if (x > (cx+4) | y > (cy+4) | x < (cx-4) | y < (cy-4)){
  int dir = JoyStickDirection();
  Serial.println(dir);
    if (dir == 1) lcd.goto_Cell(lcd.cursorPos() + 1);
    else if (dir == 3) lcd.goto_Cell(lcd.cursorPos() - 1);
    else if (dir == 2) {
      lcd.goto_Cell(lcd.cursorPos() + 40);
      delay(800);
    }
    else if (dir == 4) {
      lcd.goto_Cell(lcd.cursorPos() - 40);
      delay(800);
    }
    delay(200);
    //delay(100+((abs(dx+1)/abs(dy+1))+1));
  } else {
    cx = analogRead(JoyStick_X);
    cy = analogRead(JoyStick_Y);
  }
  delay(10);
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
  if (x > (y+4) & dx > (dy+4)){
    if (x > (cx+2)) {
      return 1;
    } else if (x < (cx-2)) {
      return 3;
    }
  } else if (y > (x+4) & dy > (dx+4)) {
    if (y > (cy+2)) {
      return 2;
    } else if (y < (cy-2)) {
      return 4;
    }
  }
  return 0;
}

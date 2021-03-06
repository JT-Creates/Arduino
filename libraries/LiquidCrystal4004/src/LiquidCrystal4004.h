#ifndef LiquidCrystal4004_h
#define LiquidCrystal4004_h

#include <inttypes.h>
#include "Print.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

class LiquidCrystal4004 : public Print {
public:
  LiquidCrystal4004(uint8_t rs, uint8_t enable1, uint8_t enable2, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,uint8_t d4 = 0, uint8_t d5 = 0, uint8_t d6 = 0, uint8_t d7 = 0);
  LiquidCrystal4004(uint8_t rs, uint8_t rw, uint8_t enable1, uint8_t enable2, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,uint8_t d4 = 0, uint8_t d5 = 0, uint8_t d6 = 0, uint8_t d7 = 0);
  void init(uint8_t rs, uint8_t rw, uint8_t enable1, uint8_t enable2, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);  
  void begin(uint8_t cols, uint8_t rows, uint8_t _e_pins = 2, uint8_t charsize = LCD_5x8DOTS);
  void clear();
  void home();
  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void state_check();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();
  void setRowOffsets(int row1, int row2, int row3, int row4);
  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t); 
  void cursor_Control(bool, int value = 0); 
  virtual size_t write(uint8_t);
  void command(uint8_t, int mode = 0);
  void goto_Cell(uint8_t);
  void set_Cell(uint8_t);
  uint8_t cursorPos();
  int enable_pin();

  using Print::write;
private:
  void send(uint8_t, uint8_t);
  void writeBits(uint8_t);
  void pulseEnable();
  uint8_t _e_pin;
  uint8_t _rs_pin; // LOW: command.  HIGH: character.
  uint8_t _rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
  uint8_t _data_pins[8];
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _initialized;
  uint8_t _numlines;
  uint8_t _cols;
  uint8_t _lines;
  uint8_t _row_offsets[4];
  uint8_t _enable_pins[]; // activated by a HIGH pulse.
};
#endif
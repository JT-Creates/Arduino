#include "LiquidCrystal4004.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

double cur_col = 0;
int col_override = 0;

int digitalReadOutputPin(uint8_t pin) {
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  if (port == NOT_A_PIN) return LOW;
  return (*portOutputRegister(port) & bit) ? HIGH : LOW;
}

LiquidCrystal4004::LiquidCrystal4004(uint8_t rs, uint8_t rw, uint8_t enable1, uint8_t enable2, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) {
  init(0, rs, rw, enable1, enable2, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal4004::LiquidCrystal4004(uint8_t rs, uint8_t enable1, uint8_t enable2, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) {
  init(0, rs, 255, enable1, enable2, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal4004::LiquidCrystal4004(uint8_t rs, uint8_t rw, uint8_t enable1, uint8_t enable2, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) {
  init(1, rs, rw, enable1, enable2, d0, d1, d2, d3, 0, 0, 0, 0);
}

LiquidCrystal4004::LiquidCrystal4004(uint8_t rs, uint8_t enable1, uint8_t enable2, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) {
  init(1, rs, 255, enable1, enable2, d0, d1, d2, d3, 0, 0, 0, 0);
}

void LiquidCrystal4004::init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable1, uint8_t enable2,  uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) {
  _rs_pin = rs;
  _rw_pin = rw;
  _enable_pin1 = enable1;
  _enable_pin2 = enable2;
  _data_pins[0] = d0;
  _data_pins[1] = d1;
  _data_pins[2] = d2;
  _data_pins[3] = d3; 
  _data_pins[4] = d4;
  _data_pins[5] = d5;
  _data_pins[6] = d6;
  _data_pins[7] = d7; 

  pinMode(_rs_pin, OUTPUT);
  // we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#
  if (_rw_pin != 255) pinMode(_rw_pin, OUTPUT);
  pinMode(_enable_pin1, OUTPUT);
  pinMode(_enable_pin2, OUTPUT);
  
  if (fourbitmode) _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  else _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
  
  begin(16, 1);  
}

void LiquidCrystal4004::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  cur_col = 0;
  if (lines == 2) _displayfunction |= LCD_2LINE;
  _numlines = lines;

  setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);  

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) _displayfunction |= LCD_5x10DOTS;

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  delayMicroseconds(50000); 
  // Now we pull both RS and R/W low to begin commands
  digitalWrite(_rs_pin, LOW);
  digitalWrite(_enable_pin1, LOW);
  digitalWrite(_enable_pin2, LOW);
  if (_rw_pin != 255) digitalWrite(_rw_pin, LOW);
  
  //put the LCD into 4 bit or 8 bit mode
  if (! (_displayfunction & LCD_8BITMODE)) {
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    col_override = 1;
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms
    col_override = 2;
    write4bits(0x03); 
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    col_override = 1;
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms
    col_override = 2;
    write4bits(0x03); 
    delayMicroseconds(4500); // wait min 4.1ms
    
    // third go!
    col_override = 1;
    write4bits(0x03);
    delayMicroseconds(150);
    col_override = 2;
    write4bits(0x03);
    delayMicroseconds(150);

    // finally, set to 4-bit interface
    col_override = 1;
    write4bits(0x02);
    col_override = 2;
    write4bits(0x02);
  } else {
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(4500);  // wait more than 4.1ms

    // second try
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(150);

    // third go
    command(LCD_FUNCTIONSET | _displayfunction);
  }

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);  

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal4004::setRowOffsets(int row0, int row1, int row2, int row3) {
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void LiquidCrystal4004::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
  //cur_col = 0;
}

void LiquidCrystal4004::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
  //cur_col = 0;
}

void LiquidCrystal4004::setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }
  
  //cur_col = col * row;
  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal4004::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal4004::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal4004::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal4004::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal4004::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal4004::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal4004::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal4004::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal4004::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal4004::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal4004::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal4004::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal4004::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystal4004::command(uint8_t value) {
  digitalWrite(_rs_pin, LOW);

  // if there is a RW pin indicated, set it low to Write
  if (_rw_pin != 255) { 
    digitalWrite(_rw_pin, LOW);
  }
  col_override = 1;
  if (_displayfunction & LCD_8BITMODE) {
    write8bits(value); 
  } else {
    write4bits(value>>4);
    write4bits(value);
  }
  col_override = 2;
  if (_displayfunction & LCD_8BITMODE) {
    write8bits(value); 
  } else {
    write4bits(value>>4);
    write4bits(value);
  }
  col_override = 0;
}

inline size_t LiquidCrystal4004::write(uint8_t value) {
  send(value, HIGH);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystal4004::send(uint8_t value, uint8_t mode) {
  digitalWrite(_rs_pin, mode);

  // if there is a RW pin indicated, set it low to Write
  if (_rw_pin != 255) { 
    digitalWrite(_rw_pin, LOW);
  }
  if (_displayfunction & LCD_8BITMODE) {
    write8bits(value); 
  } else {
    write4bits(value>>4);
    write4bits(value);
  }
}

void LiquidCrystal4004::pulseEnable(uint8_t _e_pin) {
  digitalWrite(_e_pin, LOW);
  delayMicroseconds(1);
  digitalWrite(_e_pin, HIGH);
  delayMicroseconds(1);
  digitalWrite(_e_pin, LOW);
  delayMicroseconds(37); // commands need > 37us to settle
  if (cur_col > 160) {
    cur_col = 0;
  } else {
    cur_col++;
  }
}

void LiquidCrystal4004::write4bits(uint8_t value) {
  for (int i = 0; i < 4; i++) {
    pinMode(_data_pins[i], OUTPUT);
    digitalWrite(_data_pins[i], (value >> i) & 0x01);
  }
  if (col_override == 0) {
    if (cur_col < 80) {
      pulseEnable(_enable_pin1);
    } else {
      pulseEnable(_enable_pin2);
    }
  } else if (col_override == 1) {
    pulseEnable(_enable_pin1);
    cur_col--;
  } else if (col_override == 2) {
    pulseEnable(_enable_pin2);
    cur_col--;
  }
}

void LiquidCrystal4004::write8bits(uint8_t value) {
  for (int i = 0; i < 8; i++) {
    pinMode(_data_pins[i], OUTPUT);
    digitalWrite(_data_pins[i], (value >> i) & 0x01);
  }
  if (col_override == 0) {
    if (cur_col < 80) {
      pulseEnable(_enable_pin1);
    } else {
      pulseEnable(_enable_pin2);
    }
  } else if (col_override == 1) {
    pulseEnable(_enable_pin1);
    cur_col--;
  } else if (col_override == 2) {
    pulseEnable(_enable_pin2);
    cur_col--;
  }
}

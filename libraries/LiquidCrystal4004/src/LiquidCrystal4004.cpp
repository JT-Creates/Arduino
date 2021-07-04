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
// Note, however, that resetting the Arduino doesn't reset the LCD, so we can't assume that
// its in that state when a sketch starts (and the LiquidCrystal constructor is called).
// we can save 1 pin by not using RW. Indicated by passing 255 instead of pin#

bool cursor_state = false, blink_state = false;
int cur_col = 0, col_override = 0, control_chips, switch_point, max_cells;

int digitalReadOutputPin(uint8_t pin) {
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  if (port == NOT_A_PIN) return LOW;
  return (*portOutputRegister(port) & bit) ? HIGH : LOW;
}

LiquidCrystal4004::LiquidCrystal4004(uint8_t rs, uint8_t rw, uint8_t enable1, uint8_t enable2, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) {
  init(rs, rw, enable1, enable2, d0, d1, d2, d3, d4, d5, d6, d7);
}
LiquidCrystal4004::LiquidCrystal4004(uint8_t rs, uint8_t enable1, uint8_t enable2, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) {
  init(rs, 255, enable1, enable2, d0, d1, d2, d3, d4, d5, d6, d7);
}

void LiquidCrystal4004::init(uint8_t rs, uint8_t rw, uint8_t enable1, uint8_t enable2,  uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) {
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
  if (_rw_pin != 255) pinMode(_rw_pin, OUTPUT);
  pinMode(_enable_pin1, OUTPUT);
  pinMode(_enable_pin2, OUTPUT);
  _displayfunction = ((d4 == 0 & d5 == 0 & d6 == 0 & d7 == 0) ? LCD_4BITMODE : LCD_8BITMODE) | LCD_1LINE | LCD_5x8DOTS;
}

void LiquidCrystal4004::begin(uint8_t cols, uint8_t lines, uint8_t controlChips, uint8_t dotsize) {
  cur_col = 0;
  _cols = cols;
  _lines = lines;
  control_chips = controlChips;
  max_cells = _cols * _lines;
  switch_point = max_cells / control_chips;
  if (lines > 1) _displayfunction |= LCD_2LINE;
  _numlines = lines;

  setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);  

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) & (lines == 1)) _displayfunction |= LCD_5x10DOTS;

  // I found the Seyko is fine with 1ms.
  delayMicroseconds(100);

  // Now we pull both RS and R/W low to begin commands
  digitalWrite(_rs_pin, LOW);
  if (_rw_pin != 255) digitalWrite(_rw_pin, LOW);

  // This is according to the brute forcing of a Seyko Display.
  // Put the LCD into 4 bit or 8 bit mode.
  if (!(_displayfunction & LCD_8BITMODE)) {

    // we start in 8bit mode, try to set 4 bit mode three times.
    for (int i = 0; i < 3; i++) {
      for (col_override = 2; 0 < col_override; col_override--) writeBits(0x03);
      delayMicroseconds(50); // wait min 4.1ms
    }

    // finally, set to 4-bit interface
    for (col_override = 2; 0 < col_override; col_override--) writeBits(0x02);
  } else {
    // Send function set command sequence three times.
    for (int i = 0; i < 3; i++) {
      command(LCD_FUNCTIONSET | _displayfunction);
      delayMicroseconds(50);  // wait more than 4.1ms
    }
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
void LiquidCrystal4004::clear() {
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(1380);  // this command takes a long time!
  cur_col = 0;
}

void LiquidCrystal4004::home() {
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(1380);  // this command takes a long time!
  cur_col = 0;
}

void LiquidCrystal4004::setCursor(uint8_t col, uint8_t row) {
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if (row >= max_lines) row = max_lines - 1;    // we count rows starting w/0
  if (row >= _numlines) row = _numlines - 1;    // we count rows starting w/0
  cur_col = col + row * _cols;
  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

uint8_t LiquidCrystal4004::cursorPos() {
  return cur_col;
}
void LiquidCrystal4004::goto_Cell(uint8_t cell) {
  cur_col = cell;
  if (cur_col > max_cells) cur_col -= max_cells;
  if (cur_col < 0) cur_col += max_cells;
  command(LCD_SETDDRAMADDR | cur_col);
}
void LiquidCrystal4004::set_Cell(uint8_t value) {
  
}

// Turn the display on/off (quickly)
void LiquidCrystal4004::noDisplay() {
  command(LCD_DISPLAYCONTROL | _displaycontrol & ~LCD_DISPLAYON);
}
void LiquidCrystal4004::display() {
  command(LCD_DISPLAYCONTROL | _displaycontrol | LCD_DISPLAYON);
}

// Turns the underline cursor on/off
void LiquidCrystal4004::noCursor() {
  command(LCD_DISPLAYCONTROL | _displaycontrol & ~LCD_CURSORON);
}
void LiquidCrystal4004::cursor() {
  command(LCD_DISPLAYCONTROL | _displaycontrol | LCD_CURSORON);
}

// Turn on and off the blinking cursor
void LiquidCrystal4004::noBlink() {
  command(LCD_DISPLAYCONTROL | _displaycontrol & ~LCD_BLINKON);
}
void LiquidCrystal4004::blink() {
  command(LCD_DISPLAYCONTROL | _displaycontrol | LCD_BLINKON);
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
  command(LCD_ENTRYMODESET | _displaymode | LCD_ENTRYLEFT);
}

// This is for text that flows Right to Left
void LiquidCrystal4004::rightToLeft(void) {
  command(LCD_ENTRYMODESET | _displaymode & ~LCD_ENTRYLEFT);
}

// This will 'right justify' text from the cursor
void LiquidCrystal4004::autoscroll(void) {
  command(LCD_ENTRYMODESET | _displaymode | LCD_ENTRYSHIFTINCREMENT);
}

// This will 'left justify' text from the cursor
void LiquidCrystal4004::noAutoscroll(void) {
  command(LCD_ENTRYMODESET | _displaymode & ~LCD_ENTRYSHIFTINCREMENT);
}

// Allows us to fill the first 8 CGRAM locations with custom characters
void LiquidCrystal4004::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i = 0; i < 8; i++) write(charmap[i]);
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystal4004::command(uint8_t value) {
  for (col_override = 2; 0 < col_override; col_override--) send(value, LOW);
}
inline size_t LiquidCrystal4004::write(uint8_t value) {
  send(value, HIGH);
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystal4004::send(uint8_t value, uint8_t mode) {
  if (col_override == 0) cur_col++;
  digitalWrite(_rs_pin, mode);
  // if there is a RW pin indicated, set it low to Write
  if (_rw_pin != 255) digitalWrite(_rw_pin, LOW);
  if (!(_displayfunction & LCD_8BITMODE)) writeBits(value>>4);
  writeBits(value);
}

void LiquidCrystal4004::pulseEnable() {
  if (cur_col > max_cells) cur_col = 0;
  if (cur_col < 0) cur_col = max_cells;
  _e_pin = ((cur_col <= switch_point & col_override == 0) | col_override == 1) ? _enable_pin1 : _enable_pin2;
  digitalWrite(_e_pin, LOW);
  digitalWrite(_e_pin, HIGH);
  digitalWrite(_e_pin, LOW);
}

void LiquidCrystal4004::writeBits(uint8_t value) {
  for (int i = 0; i < 8; i++) {
    if (_data_pins[i] == 0) continue;
    pinMode(_data_pins[i], OUTPUT);
    digitalWrite(_data_pins[i], (value >> i) & 0x01);
  }
  pulseEnable();
}

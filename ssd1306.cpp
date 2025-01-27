// Implementation details: SSD1306 implementation: https://www.electronicscomp.com/datasheet/ssd1306-datasheet.pdf

#include "ssd1306.h"

SSD1306::SSD1306(uint sda, uint scl, uint addr): SSD1306(sda, scl, addr, false) {
  
}

SSD1306::SSD1306(uint sda, uint scl, uint addr, bool enable_charge_pump) {
  this->i2c = *(new I2C(sda, scl, addr));
  this->busClaimed = this->i2c.ClaimBus(I2C_CLAIM_MODE_WR);

  if(this->busClaimed) {
    delay(10);
    if(enable_charge_pump) {
        this->i2c.SendByte(CONTROL_BYTE(1, 0));
        this->i2c.SendByte(0x8d);
        this->i2c.SendByte(CONTROL_BYTE(1, 0));
        this->i2c.SendByte(0x14);
        delay(10);
    }

    // Enable display
    this->i2c.SendByte(CONTROL_BYTE(1, 0));
    this->i2c.SendByte(0xaf);
  
    // Set horizontal addressing mode
    this->i2c.SendByte(CONTROL_BYTE(1, 0));
    this->i2c.SendByte(0x20);
    this->i2c.SendByte(CONTROL_BYTE(1, 0));
    this->i2c.SendByte(0x00);
  }
}

void SSD1306::SetContrast(uint contrast) {
  this->i2c.SendByte(CONTROL_BYTE(1, 0));
  this->i2c.SendByte(0x81);
  this->i2c.SendByte(CONTROL_BYTE(1, 0));
  this->i2c.SendByte(contrast);
}

void SSD1306::InvertDisplay() {
  this->i2c.SendByte(CONTROL_BYTE(1, 0));
  this->i2c.SendByte(0xa7);
}

void SSD1306::ClearScreen() {
  this->i2c.SendByte(CONTROL_BYTE(0, 1));
  for(int i = 0; i < 128*8; i++) {
    this->i2c.SendByte(0x00);
  }
  this->i2c.ReleaseClaim();
  this->i2c.ClaimBus(I2C_CLAIM_MODE_WR);
}

void SSD1306::Test() {
  this->i2c.SendByte(CONTROL_BYTE(0, 1));
  for(int j = 0; j < 4; j++) {
    for(int i = 0; i < 128; i++) {
      this->i2c.SendByte(0xaa);
    }
    for(int i = 0; i < 128; i++) {
      this->i2c.SendByte(0x00);
    }
  }
  this->i2c.ReleaseClaim();
  this->i2c.ClaimBus(I2C_CLAIM_MODE_WR);
}

void SSD1306::Line(int x1, int y1, int x2, int y2) {
  int dx = x1 - x2;
  int dy = y1 - y2;

  int steps = max(abs(dx), abs(dy));

  int x_incr = dx / steps, y_incr = dy / steps;

  int x = x1, y = y1;

  for(int i = 0; i < steps; i++) {
    this->updateVram(x, y);

    x += x_incr;
    y += y_incr;
  }
}

void SSD1306::Rect(int x1, int y1, int x2, int y2) {
  this->Line(x1, y1, x2, y1);
  this->Line(x2, y1, x2, y2);
  this->Line(x2, y2, x1, y2);
  this->Line(x1, y2, x1, y1);
}

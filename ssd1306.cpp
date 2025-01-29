#include "c_types.h"
// Implementation details: SSD1306 implementation: https://www.electronicscomp.com/datasheet/ssd1306-datasheet.pdf

#include "ssd1306.h"
#include <stdint.h>

SSD1306::SSD1306(uint sda, uint scl, uint addr): SSD1306(sda, scl, addr, false) {
  randomSeed(ESP.getCycleCount());
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
  memset(this->vram, 0, sizeof(char) * VRAM_SIZE);
  this->Show();  
}

void SSD1306::Test() {
  this->i2c.SendByte(CONTROL_BYTE(0, 1));
  for(int j = 0; j < 2; j++) {
    for(int i = 0; i < 10; i++) {
      this->i2c.SendByte(0x00);
    }
    for(int i = 10; i < 64; i++) {
      this->i2c.SendByte(0x80);
    }
    for(int i = 64; i < 128; i++) {
      this->i2c.SendByte(0x00);
    }
    for(int i = 0; i < 128; i++) {
      this->i2c.SendByte(0x00);
    }
  }
  delay(1000);
  this->i2c.ReleaseClaim();
  this->i2c.ClaimBus(I2C_CLAIM_MODE_WR);
}

void SSD1306::Point(uint x, uint y) {
  this->updateVram(x, y, 1);
}

void SSD1306::Line(uint x1, uint y1, uint x2, uint y2) {
  int dx = abs((int)x2 - (int)x1), dy = abs((int)y2 - (int)y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1; 
    int err = dx - dy;

    while (1) {
        this->updateVram(x1, y1, 1);

        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void SSD1306::Rect(uint x1, uint y1, uint x2, uint y2) {
  this->Line(x1, y1, x2, y1);
  this->Line(x2, y1, x2, y2);
  this->Line(x2, y2, x1, y2);
  this->Line(x1, y2, x1, y1);
}

void SSD1306::WhiteNoise() {
  this->i2c.SendByte(CONTROL_BYTE(0, 1));
  uint32_t r;
  for(int i = 0; i < 128*2; i++) {
    r = ESP.random();
    
    this->i2c.SendByte(r & 0xff);
    r >>= 8;

    this->i2c.SendByte(r & 0xff);
    r >>= 8;

    this->i2c.SendByte(r & 0xff);
    r >>= 8;

    this->i2c.SendByte(r & 0xff);
  }
  this->i2c.ReleaseClaim();
  this->i2c.ClaimBus(I2C_CLAIM_MODE_WR);
}

void SSD1306::Show() {
  this->i2c.SendByte(CONTROL_BYTE(0, 1));
  for(int i = 0; i < VRAM_SIZE; i++) {
    this->i2c.SendByte(this->vram[i]);
  }
  this->i2c.ReleaseClaim();
  this->i2c.ClaimBus(I2C_CLAIM_MODE_WR);
}

void SSD1306::updateVram(uint x, uint y, int value) {
  uint byteIndex = x + (128 * (uint)(y / 8));
  uint bitIndex = y % 8;

  uint8_t deleteMask = ~(1 << bitIndex);
  this->vram[byteIndex] = (this->vram[byteIndex] & deleteMask) | (value << bitIndex);
}

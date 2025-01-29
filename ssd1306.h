#ifndef SSD1306_H
#define SSD1306_H

#include "i2c.h"

#define CONTROL_BYTE(co, dc)  ((co << 1 | dc) << 6)
#define VRAM_SIZE 128*8

class SSD1306 {
  public:
    SSD1306() {}
    SSD1306(uint sda, uint scl, uint addr);
    SSD1306(uint sda, uint scl, uint addr, bool enable_charge_pump);

    bool available() { return busClaimed; }

    void SetContrast(uint contrast);
    void InvertDisplay();
    void ClearScreen();
    
    void Point(uint x, uint y);
    void Line(uint x1, uint y1, uint x2, uint y2);
    void Rect(uint x1, uint y1, uint x2, uint y2);
    void WhiteNoise();

    void Show();

    void Test();
  private:
    I2C i2c;
    bool busClaimed;
    uint8_t vram[VRAM_SIZE];

    void updateVram(uint x, uint y, int value);
};

#endif
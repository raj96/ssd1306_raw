#ifndef SSD1306_H
#define SSD1306_H

#include "i2c.h"

#define CONTROL_BYTE(co, dc)  ((co << 1 | dc) << 6)

class SSD1306 {
  public:
    SSD1306() {}
    SSD1306(uint sda, uint scl, uint addr);
    SSD1306(uint sda, uint scl, uint addr, bool enable_charge_pump);

    bool available() { return busClaimed; }

    void SetContrast(uint contrast);
    void InvertDisplay();
    void ClearScreen();
    void Line(int x1, int y1, int x2, int y2);
    void Rect(int x1, int y1, int x2, int y2);

    void Test();
  private:
    I2C i2c;
    bool busClaimed;
    char gddram[128][8];

    void updateVram(int x, int y);
};

#endif
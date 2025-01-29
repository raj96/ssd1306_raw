/**
  * Sketch implements i2c protocol from scratch
  * and drives a SSD1306 module
  * Cycles through the contrast values for SSD1306
*/
#define OLED_I2C_ADDR 0x78

#define SDA D4
#define SCL D7

#include "ssd1306.h"

SSD1306 screen;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("Initializing screen...");
  SSD1306 s(SDA, SCL, OLED_I2C_ADDR, true);
  screen = s;
  if(!screen.available()) {
    Serial.println("Could not initialize screen");
  }
  // screen.InvertDisplay();
  screen.SetContrast(0xff / 2);

  screen.ClearScreen();
  // screen.WhiteNoise();
  screen.Point(128/2, 64/2);
  screen.Line(10, 10, 64, 15);
  screen.Rect(127, 63, 70, 31);
  screen.Show();
  while(1) { delay(100); };
}

void loop() {

  delay(1500);
}



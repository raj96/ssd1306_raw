/**
  * Sketch implements i2c protocol from scratch
  * and drives a SSD1306 module
  * Cycles through the contrast values for SSD1306
  * SSD1306 implementation: https://www.electronicscomp.com/datasheet/ssd1306-datasheet.pdf
*/
#define OLED_I2C_ADDR 0x78

#define SDA D4
#define SCL D7

#include "i2c.h"

I2C i2c;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("baud rate set to 9600");
  i2c = *(new I2C(SDA, SCL, OLED_I2C_ADDR));
  i2c.ClaimBus();

  delay(500);

  // Enable charge pump
  i2c.SendByte(0b10000000);
  i2c.SendByte(0x8d);
  i2c.SendByte(0b10000000);
  i2c.SendByte(0x14);

  delay(500);

  // Set display on
  i2c.SendByte(0b10000000);
  i2c.SendByte(0xaf);

  // Set inverted mode on for display
  i2c.SendByte(0b10000000);
  i2c.SendByte(0xa7);
}

int contrast = 0x00;
int delta = 1;

void loop() {
  i2c.SendByte(0b10000000);
  i2c.SendByte(0x81);
  i2c.SendByte(0b10000000);
  i2c.SendByte(contrast);
  contrast += delta;
  if(contrast > 0x7e || contrast < 0x01) {
    delta *= -1;
  }
  Serial.printf("contrast:%d\r\n", contrast);
  delay(3);
}



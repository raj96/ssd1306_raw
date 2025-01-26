// Implementation details: https://www.ti.com/lit/an/slva704/slva704.pdf

#include "i2c.h"

I2C::I2C(uint sda, uint scl, uint slave_addr):
  sda(sda),
  scl(scl),
  addr(slave_addr) {
    pinMode(scl, OUTPUT);
    digitalWrite(scl, HIGH);
}

void I2C::sendStart() {
  pinMode(this->scl, OUTPUT);
  pinMode(this->sda, OUTPUT);

  digitalWrite(this->sda, HIGH);
  digitalWrite(this->scl, HIGH);
  digitalWrite(this->sda, LOW);
}

bool I2C::ClaimBus() {
  this->sendStart();
  this->SendByte(addr);
  
  return this->recvBit();
}

bool I2C::SendByte(char data) {
  pinMode(this->scl, OUTPUT);
  pinMode(this->sda, OUTPUT);

  for(int i = 7; i >= 0; i--) {
    this->sendBit((data >> i) & 0x1);
    
  }

  return this->recvBit();
}

bool I2C::recvBit() {
  pinMode(this->scl, INPUT);
  pinMode(this->sda, INPUT);

  while(digitalRead(this->scl) == LOW);
  
  return digitalRead(this->sda);
}

inline void I2C::sendBit(bool bit) {
  digitalWrite(this->scl, LOW);
  digitalWrite(this->sda, bit);
  digitalWrite(this->scl, HIGH);
  digitalWrite(this->scl, LOW);
}

void I2C::ReleaseClaim() {
  pinMode(this->scl, OUTPUT);
  pinMode(this->sda, OUTPUT);

  digitalWrite(sda, LOW);
  digitalWrite(scl, HIGH);
  digitalWrite(sda, HIGH);
}
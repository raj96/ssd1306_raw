#ifndef I2C_H
#define I2C_H

#include <Arduino.h>

class I2C {
  public:
    I2C() {}
    I2C(uint sda, uint scl, uint slave_addr);

    bool ClaimBus();
    bool SendByte(char data);
    char RecvByte();
    void ReleaseClaim();
  private:
    uint sda, scl, addr;
    
    bool recvBit();
    void sendBit(bool bit);
    void sendStart();
};

#endif
#ifndef I2C_H
#define I2C_H

#include <Arduino.h>

#define I2C_ACK   0
#define I2C_NACK  1

typedef enum {
  I2C_CLAIM_MODE_WR,
  I2C_CLAIM_MODE_RD
} I2CClaimMode;

class I2C {
  public:
    I2C() {}
    I2C(uint sda, uint scl, uint slave_addr);

    bool ClaimBus(I2CClaimMode claimMode);
    bool SendByte(uint8_t data);
    char RecvByte();
    void ReleaseClaim();
  private:
    uint sda, scl, addr;
    bool busClaimed;
    
    bool recvBit();
    void sendBit(bool bit);
    void sendStart();
};

#endif
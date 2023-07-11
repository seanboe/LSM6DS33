#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include <Arduino.h>
#include <Wire.h>


class I2C_Device {
  public:
    I2C_Device(uint8_t address, uint8_t whoAmIReg, uint8_t whoAmI);

    bool init();
    uint8_t read(uint8_t reg, unsigned long timeout=50);
    bool readMultiple(uint8_t reg, uint8_t *buffer, uint8_t bytesToRead, unsigned long timeout=50);
    void write(uint8_t reg, uint8_t value);
    void writeMultiple(uint8_t reg, uint8_t *buffer, uint8_t bytesToWrite);
    bool checkID();


  protected:
    uint8_t deviceAddress = 0;
    uint8_t whoAmI = 0;
    uint8_t whoAmIReg = 0;


};





#endif
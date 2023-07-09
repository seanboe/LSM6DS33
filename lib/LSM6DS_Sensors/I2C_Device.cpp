#include "I2C_Device.h"

I2C_Device::I2C_Device(uint8_t address, uint8_t whoAmIReg, uint8_t whoAmI) {
  this->deviceAddress = address;
  this->whoAmIReg = whoAmIReg;
  this->whoAmI = whoAmI;
};

/**
 * Initialize the I2C bus and verify that the device is connected
 * @return Whether initialization ran properly
 */
bool I2C_Device::init() {
  Wire.begin();
  
  if (!checkID())
    return false;
  
  return true;
}

/**
 * Read one byte from a register
 * @param reg register address to read
 * @param timeout optional; the time to wait for a receive until timeout
 * @return the value read
 */
uint8_t I2C_Device::read(uint8_t reg, unsigned long timeout) {
  uint8_t value = 0;
  readMultiple(reg, &value, 1, timeout);
  return value;
}


/**
 * Read multiple bytes from a register
 * @param reg register address to read
 * @param buffer the buffer to read into
 * @param bytesToRead the number of bytes to read
 * @param timeout optional; the time to wait for a receive until timeout
 * @return whether the read timed out
 */
bool I2C_Device::readMultiple(uint8_t reg, uint8_t *buffer, uint8_t bytesToRead, unsigned long timeout) {

  Wire.beginTransmission(this->deviceAddress);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(this->deviceAddress, 1);

  unsigned long timer = millis();

  while(!Wire.available()) {
    if (millis() - timer >= timeout)
      return false;
  }

  for (int x = 0; x < bytesToRead; x++) {
    buffer[x] = Wire.read();
  }

  return true;
}


/**
 * Write one byte to a register
 * @param reg register address to write to
 * @param value the value to write to the register
 */
void I2C_Device::write(uint8_t reg, uint8_t value) {
  writeMultiple(reg, &value, 1);
}

/**
 * Write multiple bytes from a register
 * @param reg register address to write to
 * @param buffer the buffer to write
 * @param bytesToWrite the number of bytes to write
 */
void I2C_Device::writeMultiple(uint8_t reg, uint8_t *buffer, uint8_t bytesToWrite) {
  Wire.beginTransmission(this->deviceAddress);
  Wire.write(reg);
  for (int x = 0; x < bytesToWrite; x++) {
    Wire.write(buffer[x]);
  }
  Wire.endTransmission();
}

/**
 * Checks whether the chip id is what it should be 
 * @returns whether the id check was valid
 */
bool I2C_Device::checkID() {
  return this->whoAmI == read(this->whoAmIReg);
}
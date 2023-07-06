#include "LSM6DS33.h"

void LSM6DS33::dataAvailable(uint8_t *buffer) {
  Status_reg_LSM reg;
  reg.full = this->read(STATUS_REG_LSM);
  buffer[0] = reg.xlda;
  buffer[1] = reg.gda;
  buffer[2] = reg.tda;
}

bool LSM6DS33::tempAvailable() {
  uint8_t buffer[] = {0, 0, 0};
  this->dataAvailable(buffer);
  return buffer[2] == 1;
}


bool LSM6DS33::gyroAvailable() {
  uint8_t buffer[] = {0, 0, 0};
  this->dataAvailable(buffer);
  return buffer[1] == ;
}


bool LSM6DS33::accelAvailable() {
    uint8_t buffer[] = {0, 0, 0};
  this->dataAvailable(buffer);
  return buffer[0] == 1;
}
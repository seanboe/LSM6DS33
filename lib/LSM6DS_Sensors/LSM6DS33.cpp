#include "LSM6DS33.h"

bool LSM6DS33::init(float accelRange, uint8_t accelODR, float gyroDPS, uint8_t gyroODR) {
  if (!I2C_Device::init())
    return false;

  setAccelDataRate(accelODR);
  setAccelSenseRange(accelRange);

  // setGryoDataRate(gyroODR);
  // setGyroSenseRange(gyroDPSRange);

  this->gyroRaw.x = 0;
  this->gyroRaw.y = 0;
  this->gyroRaw.z = 0;
  this->gyroScaled.x = 0.0;
  this->gyroScaled.y = 0.0;
  this->gyroScaled.z = 0.0;

  return true;
}

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
  return buffer[1] == 1;
}


bool LSM6DS33::accelAvailable() {
    uint8_t buffer[] = {0, 0, 0};
  this->dataAvailable(buffer);
  return buffer[0] == 1;
}

void LSM6DS33::reset() {
  Ctrl3_c_reg_LSM reg;
  reg.sw_reset = 1;
  this->write(CTRL3_C_REG_LSM, reg.full);
}


void LSM6DS33::setGryoDataRate(uint8_t gyroODR) {
  Ctrl2_g_reg_LSM reg;
  reg.full = this->read(CTRL2_G_REG_LSM);
  reg.odr_g = gyroODR;
  this->write(CTRL2_G_REG_LSM, reg.full);
}

void LSM6DS33::setGyroSenseRange(float dps) {
  Ctrl2_g_reg_LSM reg;

  reg.full = this->read(CTRL2_G_REG_LSM);

  if (dps == GYRO_DPS_125_LSM) {
    reg.fs_125 = GYRO_DPS_125_BIN_LSM;
  }
  else {
    if (dps == GYRO_DPS_245_LSM)
      reg.fs_g = GYRO_DPS_245_BIN_LSM;
    else if (dps == GYRO_DPS_500_LSM)
      reg.fs_g = GYRO_DPS_500_BIN_LSM;
    else if (dps == GYRO_DPS_1000_LSM)
      reg.fs_g = GYRO_DPS_1000_BIN_LSM;
    else if (dps == GYRO_DPS_2000_LSM)
      reg.fs_g = GYRO_DPS_2000_BIN_LSM;
    }

  this->write(CTRL2_G_REG_LSM, reg.full);
  this->gyroDPSRange = dps;
}



void LSM6DS33::pollGyro(ThreeAxisFloat *data) {
  uint8_t buffer[6];
  buffer[0] = this->read(OUTX_L_G_LSM);
  buffer[1] = this->read(OUTX_H_G_LSM);
  buffer[2] = this->read(OUTY_L_G_LSM);
  buffer[3] = this->read(OUTY_H_G_LSM);
  buffer[4] = this->read(OUTZ_L_G_LSM);
  buffer[5] = this->read(OUTZ_H_G_LSM);

  this->gyroRaw.x = buffer[1] << 8 | buffer[0];
  this->gyroRaw.y = buffer[3] << 8 | buffer[2];
  this->gyroRaw.z = buffer[5] << 8 | buffer[4];

  this->gyroScaled.x = (gyroRaw.x * this->gyroDPSRange * DPS_TO_RDS) / 1000;
  this->gyroScaled.y = (gyroRaw.y * this->gyroDPSRange * DPS_TO_RDS) / 1000;
  this->gyroScaled.z = (gyroRaw.z * this->gyroDPSRange * DPS_TO_RDS) / 1000;

  data->x = this->gyroScaled.x;
  data->y = this->gyroScaled.y;
  data->z = this->gyroScaled.z;
}


void LSM6DS33::setAccelDataRate(uint8_t accelODR) {
  Ctrl1_xl_reg_LSM reg; 
  reg.full = this->read(CTRL1_XL_REG_LSM);
  reg.odr_xl = accelODR;
  this->write(CTRL1_XL_REG_LSM, reg.full);
}

void LSM6DS33::setAccelSenseRange(float range) {
  Ctrl1_xl_reg_LSM reg;

  reg.full = this->read(CTRL1_XL_REG_LSM);

  if (range == ACCEL_STVTY_2_LSM)
    reg.fs_xl = ACCEL_STVTY_2_BIN_LSM;
  else if (range == ACCEL_STVTY_4_LSM)
    reg.fs_xl = ACCEL_STVTY_4_BIN_LSM;
  else if (range == ACCEL_STVTY_8_LSM)
    reg.fs_xl = ACCEL_STVTY_8_BIN_LSM;
  else if (range == ACCEL_STVTY_16_LSM)
    reg.fs_xl = ACCEL_STVTY_16_BIN_LSM;

  this->write(CTRL1_XL_REG_LSM, reg.full);

  this->accelSenseRange = range;
  Serial.println(this->accelSenseRange);
}

void LSM6DS33::pollAccel(ThreeAxisFloat *data) {
  uint8_t buffer[6];
  buffer[0] = this->read(OUTX_L_XL_LSM);
  buffer[1] = this->read(OUTX_H_XL_LSM);
  buffer[2] = this->read(OUTY_L_XL_LSM);
  buffer[3] = this->read(OUTY_H_XL_LSM);
  buffer[4] = this->read(OUTZ_L_XL_LSM);
  buffer[5] = this->read(OUTZ_H_XL_LSM);


  this->accelRaw.x = buffer[1] << 8 | buffer[0];
  // Serial.println(accelRaw.x, BIN);
  this->accelRaw.y = buffer[3] << 8 | buffer[2];
  this->accelRaw.z = buffer[5] << 8 | buffer[4];

  this->accelScaled.x = (this->accelRaw.x * this->accelSenseRange * GRAVITY_ACCEL) / 1000;
  this->accelScaled.y = (this->accelRaw.y * this->accelSenseRange * GRAVITY_ACCEL) / 1000;
  this->accelScaled.z = (this->accelRaw.z * this->accelSenseRange * GRAVITY_ACCEL) / 1000;

  data->x = this->accelScaled.x;
  data->y = this->accelScaled.y;
  data->z = this->accelScaled.z;
}



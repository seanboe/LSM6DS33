#include "LSM6DS33.h"

bool LSM6DS33::init(uint8_t accelODR, double accelRange, uint8_t gyroODR, double gyroRange, bool enableTaps) {
  if (!I2C_Device::init())
    return false;


  setAccelDataRate(accelODR);
  setAccelSenseRange(accelRange);

  setGryoDataRate(gyroODR);
  setGyroSenseRange(gyroRange);

  if (enableTaps) {
    initTaps();
  }

  this->gyroRaw.x = 0;
  this->gyroRaw.y = 0;
  this->gyroRaw.z = 0;
  this->gyroScaled.x = 0.0;
  this->gyroScaled.y = 0.0;
  this->gyroScaled.z = 0.0;

  return true;
}

/**
 * @brief Internal method which reads the 'data available' register
 * @returns Whether new data is available
 */
void LSM6DS33::dataAvailable(uint8_t *buffer) {
  Status_reg_LSM reg;
  reg.full = this->read(STATUS_REG_LSM);
  buffer[0] = reg.xlda;
  buffer[1] = reg.gda;
  buffer[2] = reg.tda;
}

/**
 * @brief Reads whether there is new thermometer data available
 * @returns Whether new data is available
 */
bool LSM6DS33::tempAvailable() {
  uint8_t buffer[] = {0, 0, 0};
  this->dataAvailable(buffer);
  return buffer[2] == 1;
}

/**
 * @brief Reads whether there is new gyroscope data available
 * @returns Whether new data is available
 */
bool LSM6DS33::gyroAvailable() {
  uint8_t buffer[] = {0, 0, 0};
  this->dataAvailable(buffer);
  return buffer[1] == 1;
}


/**
 * @brief Reads whether there is new accelerometer data available
 * @returns Whether new data is available
 */
bool LSM6DS33::accelAvailable() {
    uint8_t buffer[] = {0, 0, 0};
  this->dataAvailable(buffer);
  return buffer[0] == 1;
}


/**
 * @brief Performs a software reset on the imu
 */
void LSM6DS33::reset() {
  Ctrl3_c_reg_LSM reg;
  reg.sw_reset = 1;
  this->write(CTRL3_C_REG_LSM, reg.full);
}


double LSM6DS33::pollTemp() {
  uint8_t buffer[2];
  buffer[0] = this->read(OUT_TEMP_L_LSM);
  buffer[1] = this->read(OUT_TEMP_H_LSM);
  this->thermometerRaw = buffer[1] << 8 | buffer[0];

  this->thermometerScaled = ((double)this->thermometerRaw / TEMP_SENSITIVITY) + 25.0;

  return this->thermometerScaled;
}


/**
 * @brief Sets the gyroscope ODR (output data rate)
 * @param gyroODR The ODR you want
 */
void LSM6DS33::setGryoDataRate(uint8_t gyroODR) {
  Ctrl2_g_reg_LSM reg;
  reg.full = this->read(CTRL2_G_REG_LSM);
  reg.odr_g = gyroODR;
  this->write(CTRL2_G_REG_LSM, reg.full);
}


/**
 * @brief Sets the gyroscope sensitivity range in DPS (degrees per second)
 * @param dps The dps you want
 */
void LSM6DS33::setGyroSenseRange(double dps) {
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
    else {
      Serial.println("here");
    }
  }

  this->write(CTRL2_G_REG_LSM, reg.full);

  this->gyroRange = dps;
}


/**
 * @brief Reads the gyroscope register in rad/s
 * @param data The angular velocity in each direction
 */
void LSM6DS33::pollGyro(ThreeAxisDouble *data) {
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

  this->gyroScaled.x = (gyroRaw.x * this->gyroRange * DPS_TO_RDS) / 1000;
  this->gyroScaled.y = (gyroRaw.y * this->gyroRange * DPS_TO_RDS) / 1000;
  this->gyroScaled.z = (gyroRaw.z * this->gyroRange * DPS_TO_RDS) / 1000;

  data->x = this->gyroScaled.x;
  data->y = this->gyroScaled.y;
  data->z = this->gyroScaled.z;
}


/**
 * @brief Sets the accerometer ODR (output data rate)
 * @param accelODR The ODR you want
 */
void LSM6DS33::setAccelDataRate(uint8_t accelODR) {
  Ctrl1_xl_reg_LSM reg; 
  reg.full = this->read(CTRL1_XL_REG_LSM);
  reg.odr_xl = accelODR;
  this->write(CTRL1_XL_REG_LSM, reg.full);
}


/**
 * @brief Sets the accerometer sensitivity in +-x Gs. 
 * @param range The sensitivity / range
 */
void LSM6DS33::setAccelSenseRange(double range) {
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
}


/**
 * @brief Reads the accerometer register in m/s^2
 * @param data Acceration in each direction
 */
void LSM6DS33::pollAccel(ThreeAxisDouble *data) {
  uint8_t buffer[6];
  buffer[0] = this->read(OUTX_L_XL_LSM);
  buffer[1] = this->read(OUTX_H_XL_LSM);
  buffer[2] = this->read(OUTY_L_XL_LSM);
  buffer[3] = this->read(OUTY_H_XL_LSM);
  buffer[4] = this->read(OUTZ_L_XL_LSM);
  buffer[5] = this->read(OUTZ_H_XL_LSM);


  this->accelRaw.x = buffer[1] << 8 | buffer[0];
  this->accelRaw.y = buffer[3] << 8 | buffer[2];
  this->accelRaw.z = buffer[5] << 8 | buffer[4];

  this->accelScaled.x = (this->accelRaw.x * this->accelSenseRange * GRAVITY_ACCEL) / 1000;
  this->accelScaled.y = (this->accelRaw.y * this->accelSenseRange * GRAVITY_ACCEL) / 1000;
  this->accelScaled.z = (this->accelRaw.z * this->accelSenseRange * GRAVITY_ACCEL) / 1000;

  data->x = this->accelScaled.x;
  data->y = this->accelScaled.y;
  data->z = this->accelScaled.z;
}


/**
 * @brief Reads the tap event register
 * @param data Whether each direction has a tap event
 */
void LSM6DS33::pollTaps(ThreeAxisInt *data) {

  Tap_src_reg_LSM reg;
  reg.full = this->read(TAP_SRC_REG_LSM);

  data->x = (reg.xyz_tap & 0b100 ? 1 : 0);
  data->y = (reg.xyz_tap & 0b010 ? 1 : 0);
  data->z = (reg.xyz_tap & 0b001 ? 1 : 0);
}


/**
 * @brief Initializes tap functionality; requires ACCEL_ODR to be at least 416Hz
 * @param reg register address to write to
 * @param buffer the buffer to write
 * @param threshold the number of bytes to write
 */
void LSM6DS33::initTaps(uint8_t threshold) {

  // Enable the tap registers
  Tap_cfg_reg_LSM reg;
  reg.full = this->read(TAP_CFG_REG_LSM);
  reg.tap_xyz = 0b111;
  this->write(TAP_CFG_REG_LSM, reg.full);

  // Set a really low threshold (no threshold = disabled)
  Tap_ths_6d_reg_LSM reg2;
  reg2.full = this->read(TAP_THS_6D_REG_LSM);
  reg2.tap_ths = threshold;
  this->write(TAP_THS_6D_REG_LSM, reg2.full);
}


/**
 * @brief Initializes accelerometer filters in high-pass mode; CURRENTLY NOT WORKING
 * @param cutoff The cutoff frequency you want, dependent on ODR
 */
void LSM6DS33::enableAccelFilters(uint8_t cutoff) {

  Ctrl8_xl_reg_LSM reg;
  reg.full = this->read(CTRL8_XL_REG_LSM);

  // reg.lpf2_xl_en = 0b1;
  reg.hp_slope_xl_en = 0b1;
  reg.hpcf_xl = cutoff;


  this->write(CTRL8_XL_REG_LSM, reg.full);
}
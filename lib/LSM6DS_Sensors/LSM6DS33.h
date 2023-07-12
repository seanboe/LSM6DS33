#ifndef LSM6DS33_H
#define LSM6DS33_H

#include <Arduino.h>
#include <Wire.h>

#include "I2C_Device.h"
#include "LSM6DS33_Registers.h"

#define LSM_ADDRESS       0x6A

#define LSM_WHO_AM_I  0x69
#define LSM_WHO_AM_I_REG  0x0F


typedef struct {
  int16_t x;        // For Gyro, pitch
  int16_t y;        // For Gyro, roll
  int16_t z;        // For Gyro, yaw
} ThreeAxisInt;

typedef struct {
  double x;        // For Gyro, pitch
  double y;        // For Gyro, roll
  double z;        // For Gyro, yaw
} ThreeAxisDouble;


class LSM6DS33 : public I2C_Device {

  public:
    LSM6DS33() : I2C_Device(LSM_ADDRESS, whoAmIReg=LSM_WHO_AM_I_REG, whoAmI=LSM_WHO_AM_I) {};
 
    bool init(uint8_t accelODR=ACCEL_ODR_832_BIN_LSM, double accelRange=ACCEL_STVTY_16_LSM, uint8_t gyroODR=GYRO_ODR_104_BIN_LSM, double gyroRange=GYRO_DPS_2000_LSM, bool enableTaps=true);

    void setAccelDataRate(uint8_t accelODR);
    void setAccelSenseRange(double range);

    void setGryoDataRate(uint8_t gyroODR);
    void setGyroSenseRange(double dps);

    double pollTemp();
    void pollGyro(ThreeAxisDouble *data);
    void pollAccel(ThreeAxisDouble *data);
    void pollTaps(ThreeAxisInt *data);

    void enableAccelFilters(uint8_t filter=ACCEL_FILTER_HP_100);

    bool tempAvailable();
    bool gyroAvailable();
    bool accelAvailable();

    void reset();

  private:

    void initTaps(uint8_t threshold=0b0001);

    void dataAvailable(uint8_t *buffer);

    double accelSenseRange;
    double gyroRange;

    double thermometerScaled;
    int16_t thermometerRaw;
    ThreeAxisInt gyroRaw;
    ThreeAxisDouble gyroScaled;
    ThreeAxisInt accelRaw;
    ThreeAxisDouble accelScaled;

};






#endif
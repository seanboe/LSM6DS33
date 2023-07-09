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
  uint16_t x;        // For Gyro, pitch
  uint16_t y;        // For Gyro, roll
  uint16_t z;        // For Gyro, yaw
} ThreeAxisInt;

typedef struct {
  float x;        // For Gyro, pitch
  float y;        // For Gyro, roll
  float z;        // For Gyro, yaw
} ThreeAxisFloat;


class LSM6DS33 : public I2C_Device {

  public:
    LSM6DS33() : I2C_Device(LSM_ADDRESS, whoAmIReg=LSM_WHO_AM_I_REG, whoAmI=LSM_WHO_AM_I) {};
 
    bool init(float accelRange=ACCEL_STVTY_4_LSM, uint8_t accelODR=ACCEL_ODR_104_BIN_LSM, float gyroDPS=GYRO_DPS_2000_LSM, uint8_t gyroODR=GYRO_ODR_104_BIN_LSM);

    void setAccelDataRate(uint8_t accelODR);
    void setAccelSenseRange(float range);

    void setGryoDataRate(uint8_t gyroODR);
    void setGyroSenseRange(float dps);

    void pollGyro(ThreeAxisFloat *data);
    void pollAccel(ThreeAxisFloat *data);

    bool tempAvailable();
    bool gyroAvailable();
    bool accelAvailable();

    void reset();
    

    float gyroDPSRange;
    float accelSenseRange;

  private:
    void dataAvailable(uint8_t *buffer);

    // float gyroDPSRange;
    // float accelSenseRange;

    ThreeAxisInt gyroRaw;
    ThreeAxisFloat gyroScaled;
    ThreeAxisInt accelRaw;
    ThreeAxisFloat accelScaled;

};






#endif
#ifndef LSM6DS33_H
#define LSM6DS33_H

#include <Arduino.h>
#include <Wire.h>

#include "I2C_Device.h"
#include "LSM6DS33_Registers.h"

#define ADDRESS       0x6A
#define LSM_WHO_AM_I  0x69




class LSM6DS33 : public I2C_Device {

  public:
    LSM6DS33() : I2C_Device(ADDRESS, LSM_WHO_AM_I) {};

    bool tempAvailable();
    bool gyroAvailable();
    bool accelAvailable();


  private:
    void dataAvailable(uint8_t *buffer);





};






#endif
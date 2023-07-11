#include <Arduino.h>
#include <Wire.h>

#include <LSM6DS33.h>

#define ADDRESS 0x6A


LSM6DS33 lsmDevice;


uint8_t read(uint8_t reg);
void readAccelX();
void readGyroX();

void setup() {

  Serial.begin(112500);

  while (!Serial) {
    ;
  }


  if (!lsmDevice.init()) {
  Serial.println("LSM Device failed init");
    while(1) 
      ;
  }

  Serial.println(lsmDevice.read(LSM_WHO_AM_I_REG));
  Serial.println(lsmDevice.read(CTRL2_G_REG_LSM), BIN);


  delay(1000);



}

void loop() {

  ThreeAxisDouble accelerometer;
  lsmDevice.pollAccel(&accelerometer);

  Serial.println("X: " + String(accelerometer.x, 3) + ", Y: " + String(accelerometer.y, 3) + ", Z: " + String(accelerometer.z, 3));

  ThreeAxisDouble gyroscope;
  lsmDevice.pollGyro(&gyroscope);

  Serial.println("Pitch: " + String(gyroscope.x, 3) + ", Roll: " + String(gyroscope.y, 3) + ", Yaw: " + String(gyroscope.z, 3));


}


void readAccelX() {
  uint8_t high = lsmDevice.read(OUTX_H_XL_LSM);
  uint8_t low = lsmDevice.read(OUTX_L_XL_LSM);


  int16_t tot = high << 8 | low;

  // Serial.println(tot);


  // bool neg = high >> 7 & 0b1;
  // Serial.println(neg);
  // tot = ~tot;
  // tot = tot + 1;
  // tot = tot * (neg ? -1 : 1);
  double scaled = (lsmDevice.accelSenseRange * tot * GRAVITY_ACCEL) / 1000;
  Serial.println(scaled);
}

void readGyroX() {
  uint8_t high = lsmDevice.read(OUTX_H_G_LSM);
  uint8_t low = lsmDevice.read(OUTX_L_G_LSM);

  int16_t tot = high << 8 | low;
  // Serial.print(tot, BIN);
  // Serial.print("      ");

  double scaled = (lsmDevice.gyroRange * DPS_TO_RDS * tot) / 1000;
  Serial.println(scaled);

}




  // Reg reg;
  // reg.full = read(0x19);
  // Serial.println(reg.full, BIN);
  // reg.sign_motion_en |= 0b1;
  // Serial.println(reg.full, BIN);


  // Wire.beginTransmission(ADDRESS);
  // Wire.write(0x19);
  // Wire.write(reg.full);
  // Wire.endTransmission();

  // delay(50);

  // Serial.println(read(0x19), BIN);


// uint8_t read(uint8_t reg) {
//   Wire.beginTransmission(ADDRESS);
//   Wire.write(reg);
//   Wire.endTransmission();

//   Wire.requestFrom((uint8_t)ADDRESS, (uint8_t)1);

//   return Wire.read();
// }
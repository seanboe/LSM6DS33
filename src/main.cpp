#include <Arduino.h>
#include <Wire.h>

#include <LSM6DS33.h>

#define ADDRESS 0x6A


LSM6DS33 lsmDevice;


uint8_t read(uint8_t reg);


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



  // uint8_t value;
  // value = lsmDevice.read(0x0B);
  // Serial.println(value, BIN);

  // lsmDevice.write(0x0B, 0b00111111);

  // value = lsmDevice.read(0x0B);
  // Serial.println(value, BIN);



}

void loop() {

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
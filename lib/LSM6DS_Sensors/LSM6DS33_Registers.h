#ifndef REGISTERS_H
#define REGISTERS_H

#include <Arduino.h>


// LOOK AT PEDOMETER LATER



// Degrees to radians scale factor
#define DPS_TO_RDS        0.017453
#define GRAVITY_ACCEL     9.80665

// THERMOMETER
#define OUT_TEMP_L_LSM    0x20
#define OUT_TEMP_H_LSM    0x20

// GYROSCOPE

// Pitch
#define OUTX_L_G_LSM      0x22
#define OUTX_H_G_LSM      0x23

// Roll
#define OUTY_L_G_LSM      0x24
#define OUTY_H_G_LSM      0x25

// Yaw
#define OUTZ_L_G_LSM      0x26
#define OUTZ_H_G_LSM      0x27

// ACCELEROMETER

// X axis
#define OUTX_L_XL_LSM     0x28
#define OUTX_H_XL_LSM     0x29

// Y axis
#define OUTY_L_XL_LSM     0x2A
#define OUTY_H_XL_LSM     0x2B

// Z axis
#define OUTZ_L_XL_LSM     0x2C
#define OUTZ_H_XL_LSM     0x2D


// GYROSCOPE SENSITIVITY 
// Used in Ctrl2_G (0x11)

// Sensitivity in mdps / LSB
#define GYRO_DPS_125_LSM  4.375
#define GYRO_DPS_245_LSM  8.75
#define GYRO_DPS_500_LSM  17.50
#define GYRO_DPS_1000_LSM 35
#define GYRO_DPS_2000_LSM 70

// Binary represention of each setting in Ctrl2_g (0x11)
#define GYRO_DPS_125_BIN_LSM    0b0
#define GYRO_DPS_245_BIN_LSM    0b00
#define GYRO_DPS_500_BIN_LSM    0b01
#define GYRO_DPS_1000_BIN_LSM   0b10
#define GYRO_DPS_2000_BIN_LSM   0b11

// GYROSCOPE ODR
// Used in Ctrl2_G (0x11); assumes that gyro is in high power mode 
// (G_HM_MODE = 0 ~ CTRL7_G)
#define GYRO_ODR_13_BIN_LSM     0b0001
#define GYRO_ODR_26_BIN_LSM     0b0010
#define GYRO_ODR_52_BIN_LSM     0b0011
#define GYRO_ODR_104_BIN_LSM    0b0100
#define GYRO_ODR_208_BIN_LSM    0b0101
#define GYRO_ODR_416_BIN_LSM    0b0110
#define GYRO_ODR_832_BIN_LSM    0b0111
#define GYRO_ODR_1664_BIN_LSM   0b1000

// ACCELEROMETER FULL-SCALE
// Used in Ctrl1_xl (0x10)

// Accelerometer sensitity in mg/LSB
#define ACCEL_STVTY_2_LSM   0.061
#define ACCEL_STVTY_4_LSM   0.122
#define ACCEL_STVTY_8_LSM   0.244
#define ACCEL_STVTY_16_LSM  0.488

// Binary representation of each setting in Ctrl1_xl (0x10)
#define ACCEL_STVTY_2_BIN_LSM   0b00
#define ACCEL_STVTY_4_BIN_LSM   0b10
#define ACCEL_STVTY_8_BIN_LSM   0b11
#define ACCEL_STVTY_16_BIN_LSM  0b01

// ACCELEROMETER ODR
// Used in Ctrl1_xl (0x10); assumes that accel is in high power mode
// (XL_HM_MODE = 0 ~ CTRL6_XL)
#define ACCEL_ODR_13_BIN_LSM    0b0001
#define ACCEL_ODR_26_BIN_LSM    0b0010
#define ACCEL_ODR_52_BIN_LSM    0b0011
#define ACCEL_ODR_104_BIN_LSM   0b0100
#define ACCEL_ODR_208_BIN_LSM   0b0101
#define ACCEL_ODR_416_BIN_LSM   0b0110
#define ACCEL_ODR_832_BIN_LSM   0b0111
#define ACCEL_ODR_1664_BIN_LSM  0b1000
#define ACCEL_ODR_3328_BIN_LSM  0b1001
#define ACCEL_ODR_6644_BIN_LSM  0b1010


/**
 * @brief Register used for setting Accelerometer ODR
 */
#define CTRL1_XL_REG_LSM 0x10
typedef union {
  struct {
    uint8_t bw_xl : 2;
    uint8_t fs_xl : 2;
    uint8_t odr_xl : 4;
  };
  uint8_t full = 0;
} Ctrl1_xl_reg_LSM;

/**
 * @brief Register used for setting Gyroscope ODR
 */
#define CTRL2_G_REG_LSM 0x11
typedef union {
  struct {
    uint8_t unused : 1;
    uint8_t fs_125 : 1;
    uint8_t fs_g : 2;
    uint8_t odr_g : 4;
  };
  uint8_t full = 0;
} Ctrl2_g_reg_LSM;


/**
 * @brief Register used for software reset
 */
#define CTRL3_C_REG_LSM 0x12
typedef union {
  struct {
    uint8_t sw_reset : 1;
    uint8_t ble : 1;
    uint8_t if_inc : 1;
    uint8_t si : 1;
    uint8_t pp_od : 1;
    uint8_t h_lactive : 1;
    uint8_t bdu : 1;
    uint8_t boot : 1;
  };
  uint8_t full = 0;
} Ctrl3_c_reg_LSM;


/**
 * @brief Register used for enabling / disabling high-performance 
 * operating mode for accelerometer
 */
#define CTRL6_C_REG_LSM 0x15
typedef union {
  struct {
    uint8_t unused : 4;
    uint8_t xl_hm_mode : 1;
    uint8_t lvl2_en : 1;
    uint8_t lvlen : 1;
    uint8_t trig_en : 1;
  };
  uint8_t full = 0;
} Ctrl6_c_reg_LSM;


/**
 * @brief Register used for enabling / disabling high-performance 
 * operating mode for gyroscope + enable and reset gyro high pass filter
 */
#define CTRL7_G_REG_LSM 0x16
typedef union {
  struct {
    uint8_t unused : 2;
    uint8_t rounding_status : 1;
    uint8_t hp_g_r_st : 1;
    uint8_t hpcf_g : 2;
    uint8_t hpcf_g_en : 1;
    uint8_t g_hm_mode : 1;
  };
  uint8_t full = 0;
} Ctrl7_g_reg_LSM;

/**
 * @brief Register used for enabling accelerometer low pass filter
 */
#define CTRL8_XL_REG_LSM 0x17
typedef union {
  struct {
    uint8_t low_pass_on_6d : 1;
    uint8_t unused_1 : 1;
    uint8_t hp_slope_xl_en : 1;
    uint8_t unused_2 : 2;
    uint8_t hpcf_xl : 2;
    uint8_t lpf2_xl_en : 1;
  };
  uint8_t full = 0;
} Ctrl8_xl_reg_LSM;


/**
 * @brief Register used for tap events
 */
#define TAP_SRC_REG_LSM 0x1C
typedef union {
  struct {
    uint8_t zyz_tap : 3;
    uint8_t tap_sign : 1;
    uint8_t double_tap : 1;
    uint8_t single_tap : 1;
    uint8_t tap_ia : 1;
    uint8_t unused : 1;
  };
  uint8_t full = 0;
} Tap_src_reg_LSM;


/**
 * @brief Register used for new data available 
 */
#define STATUS_REG_LSM  0x1E
typedef union {
  struct {
    uint8_t xlda : 1;
    uint8_t gda : 1;
    uint8_t tda : 1;
    uint8_t ev_boot : 1;
    uint8_t unused : 4;
  };
  uint8_t full = 0;
} Status_reg_LSM;




#endif
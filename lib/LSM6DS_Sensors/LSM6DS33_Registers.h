#ifndef REGISTERS_H
#define REGISTERS_H

#include <Arduino.h>


#define LSM_WHO_AM_I_REG  0x0F

// THERMOMETER
#define OUT_TEMP_L_LSM    0x20
#define OUT_TEMP_H_LSM    0x20

// GYROSCOPE

// Pitch
#define OUTX_L_G          0x22
#define OUTX_H_G          0x23




// REDO
#define CTRL1_XL_REG_LSM 0x10

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
} Ctrl6_c_LSM;



#define CTRL7_G_REG_LSM 0x16

typedef union {
  struct {
    uint8_t unused : 2;
    uint8_t rounding_status : 1;
    uint8_t hp_g_r_st : 1;
    uint8_t hpcf_g0 : 1;
    uint8_t hpcf_g1 : 1;
    uint8_t hpcf_g_en : 1;
    uint8_t g_hm_mode : 1;
  };
  uint8_t full = 0;
} Ctrl7_g_LSM;


#define CTRL8_XL_REG_LSM 0x17

typedef union {
  struct {
    uint8_t low_pass_on_6d : 1;
    uint8_t unused_1 : 1;
    uint8_t hp_slope_xl_en : 1;
    uint8_t unused_2 : 2;
    uint8_t hpcf_xl0 : 1;
    uint8_t hpcf_xl1 : 1;
    uint8_t lpf2_xl_en : 1;
  };
  uint8_t full = 0;
} Ctrl8_xl_LSM;



#define TAP_SRC_REG_LSM 0x1C

typedef union {
  struct {
    uint8_t z_tap : 1;
    uint8_t y_tap : 1;
    uint8_t x_tap : 1;
    uint8_t tap_sign : 1;
    uint8_t double_tap : 1;
    uint8_t single_tap : 1;
    uint8_t tap_ia : 1;
    uint8_t unused : 1;
  };
  uint8_t full = 0;
} Tap_src_reg_LSM;



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
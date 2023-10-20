#include <lsm303_accel.hpp>

LSM303_Accel::LSM303_Accel(I2C_HandleTypeDef *_pntr_hi2c) {
  pntr_hi2c = _pntr_hi2c;

  // Initialize the raw accel data
  data.x = 0;
  data.y = 0;
  data.z = 0;
}

bool LSM303_Accel::write(const uint8_t reg_location, const uint8_t *data, const uint8_t len){
  if (HAL_I2C_Mem_Write((I2C_HandleTypeDef *) pntr_hi2c, LSM303_ADDRESS_ACCEL, reg_location, I2C_MEMADD_SIZE_8BIT, (uint8_t *)data, len, HAL_MAX_DELAY) != HAL_OK) {
    // Error handling: You can handle write errors here
  	APP_LOG(TS_ON, VLEVEL_M,"Error writing \r\n");
    return false;
  }
  return true;
}

bool LSM303_Accel::read(const uint8_t reg_location, uint8_t *data, const uint8_t len){
  if (HAL_I2C_Mem_Read((I2C_HandleTypeDef *) pntr_hi2c, LSM303_ADDRESS_ACCEL, reg_location, I2C_MEMADD_SIZE_8BIT, (uint8_t *)data, len, HAL_MAX_DELAY) != HAL_OK) {
    // Error handling: You can handle write errors here
    return false;
  }
  return true;
}

bool LSM303_Accel::init() {
  uint8_t len = 1;

  uint8_t reg_data = 0x57; // Enable the accelerometer (100Hz)
  if ( write(LSM303_REGISTER_ACCEL_CTRL_REG1_A, &reg_data, len) ){
    if ( read(LSM303_REGISTER_ACCEL_WHO_AM_I, &reg_data, len) ) {
      if (reg_data == 0x33) return true; //confirm that we are talking
    }
  }
  return false; //something went wrong
}

void LSM303_Accel::setRange(lsm303_accel_range_t new_range) {
  //get the current register values
  uint8_t reg_data;
  uint8_t len = 1;
  read(LSM303_REGISTER_ACCEL_CTRL_REG4_A, &reg_data, len);

  //clear the current range bits
  reg_data &= ~(3 << 4);

  //update with the new desired range
  reg_data |= (new_range << 4);

  //send it to the device
  write(LSM303_REGISTER_ACCEL_CTRL_REG4_A, &reg_data, len);
}

lsm303_accel_range_t LSM303_Accel::getRange(void) {
  //get the current register values
  uint8_t reg_data;
  uint8_t len = 1;
  read(LSM303_REGISTER_ACCEL_CTRL_REG4_A, &reg_data, len);

  //isolate the current range bits
  reg_data &= (3 << 4);

  //shift the bits
  reg_data = (reg_data >> 4);

  return (lsm303_accel_range_t)reg_data;
}

void LSM303_Accel::setMode(lsm303_accel_mode_t new_mode) {
  ///////////////////////////Edit power//////////////////////
  //get the current register value for the power mode
  uint8_t power_reg = 0;
  uint8_t len = 1;
  read(LSM303_REGISTER_ACCEL_CTRL_REG1_A, &power_reg, len);

  //clear the current power bit
  power_reg &= ~(1 << 3);

  //update with the new desired power mode
  power_reg |= ((new_mode & 0b01) << 3);

  //send it to the device
  write(LSM303_REGISTER_ACCEL_CTRL_REG1_A, &power_reg, len);
  HAL_Delay(20);

  ///////////////////////////Edit resolution//////////////////
  uint8_t res_reg = 0;
  read(LSM303_REGISTER_ACCEL_CTRL_REG4_A, &res_reg, len);

  //clear the current resolution bit
  res_reg &= ~(1 << 3);

  //update with the new desired resolution mode
  res_reg |= ((new_mode & 0b10) << 2);

  //send it to the device
  write(LSM303_REGISTER_ACCEL_CTRL_REG4_A, &res_reg, len);
  HAL_Delay(20);
}

lsm303_accel_mode_t LSM303_Accel::getMode(void) {
  ///////////////////////////power//////////////////////
  //get the current register value for the power mode
  uint8_t power_reg = 0;
  uint8_t len = 1;
  read(LSM303_REGISTER_ACCEL_CTRL_REG1_A, &power_reg, len);

  //isolate the current power bit
  power_reg &= (1 << 3);

  //shift it to the LSB
  power_reg = power_reg >> 3;

  ///////////////////////////resolution//////////////////////
  //get the current register value for the power mode
  uint8_t res_reg = 0;
  read(LSM303_REGISTER_ACCEL_CTRL_REG4_A, &res_reg, len);

  //isolate the current resolution bit
  res_reg &= (1 << 3);

  //shift it to the 2nd LSB
  res_reg = res_reg >> 2;

  return (lsm303_accel_mode_t)(res_reg | power_reg);
}
// /***************************************************************************
//  PRIVATE FUNCTIONS
//  ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Reads the raw data from the sensor
*/
/**************************************************************************/
lsm303AccelData LSM303_Accel::getAccelData(void) {
  // this sucks but using one register with a 6 byte read to buffer doesn't
  // work.
  uint8_t xlo;
  uint8_t xhi;
  uint8_t ylo;
  uint8_t yhi;
  uint8_t zlo;
  uint8_t zhi;

  uint8_t len = 1;

  read(LSM303_REGISTER_ACCEL_OUT_X_L_A, &xlo, len);
  read(LSM303_REGISTER_ACCEL_OUT_X_H_A, &xhi, len);
  read(LSM303_REGISTER_ACCEL_OUT_Y_L_A, &ylo, len);
  read(LSM303_REGISTER_ACCEL_OUT_Y_H_A, &yhi, len);
  read(LSM303_REGISTER_ACCEL_OUT_Z_L_A, &zlo, len);
  read(LSM303_REGISTER_ACCEL_OUT_Z_H_A, &zhi, len);

  uint16_t raw_x = (int16_t)(xlo | (xhi << 8));
  uint16_t raw_y = (int16_t)(ylo | (yhi << 8));
  uint16_t raw_z = (int16_t)(zlo | (zhi << 8));

  lsm303_accel_mode_t mode = getMode();

  float lsb = getLSB(mode);
  uint8_t shift = getShift(mode);

  data.x = (float)(raw_x >> shift) * lsb * GRAVITY_EARTH;
  data.y = (float)(raw_y >> shift) * lsb * GRAVITY_EARTH;
  data.z = (float)(raw_z >> shift) * lsb * GRAVITY_EARTH;

  return data;
}
/**************************************************************************/
/*!
    @brief  Gets the Least Significant Bit value for the current mode
    @param mode the current mode, used to determind the appropriate lsb value
    in concert with the current range setting.
*/
/**************************************************************************/
float LSM303_Accel::getLSB(lsm303_accel_mode_t mode) {
  float lsb = 0;
  lsm303_accel_range_t range = getRange();
  if (mode == LSM303_MODE_NORMAL) {
    switch (range) {
    case LSM303_RANGE_2G:
      lsb = 0.0039;
      break;
    case LSM303_RANGE_4G:
      lsb = 0.00782;
      break;
    case LSM303_RANGE_8G:
      lsb = 0.01563;
      break;
    case LSM303_RANGE_16G:
      lsb = 0.0469;
      break;
    }
  }

  else if (mode == LSM303_MODE_HIGH_RESOLUTION) {
    switch (range) {
    case LSM303_RANGE_2G:
      lsb = 0.00098;
      break;
    case LSM303_RANGE_4G:
      lsb = 0.00195;
      break;
    case LSM303_RANGE_8G:
      lsb = 0.0039;
      break;
    case LSM303_RANGE_16G:
      lsb = 0.01172;
      break;
    }
  } else if (mode == LSM303_MODE_LOW_POWER) {
    switch (range) {
    case LSM303_RANGE_2G:
      lsb = 0.01563;
      break;
    case LSM303_RANGE_4G:
      lsb = 0.03126;
      break;
    case LSM303_RANGE_8G:
      lsb = 0.06252;
      break;
    case LSM303_RANGE_16G:
      lsb = 0.18758;
      break;
    }
  }

  return lsb;
}
/**************************************************************************/
/*!
    @brief  Gets the bit shift amount for the current mode
    @param mode the current mode, used to determind the appropriate shift
    amount based on the bitdepth of the mode
*/
/**************************************************************************/
uint8_t LSM303_Accel::getShift(lsm303_accel_mode_t mode) {
  uint8_t shift = 0;
  switch (mode) {
  case LSM303_MODE_HIGH_RESOLUTION:
    shift = 4;
    break;
  case LSM303_MODE_NORMAL:
    shift = 6;
    break;
  case LSM303_MODE_LOW_POWER:
    shift = 8;
    break;
  }

  return shift;
}

/*************************************************************************/
/*!
    @brief Sets the polarity of the interrupt pins
    @param active_high Set to true for the INT pints to be active high,
        false for active low
*/
void LSM303_Accel::interruptsActiveHigh(bool active_high) {
  //get the current register value for the interrupt mode
  uint8_t int_reg = 0;
  uint8_t len = 1;
  read(LSM303_REGISTER_ACCEL_CTRL_REG6_A, &int_reg, len);

  //clear the current interrupt bit
  int_reg &= ~(1 << 1);

  //update with the new desired interrupt mode
  int_reg |= (active_high << 1);

  //send it to the device
  write(LSM303_REGISTER_ACCEL_CTRL_REG1_A, &int_reg, len);
}

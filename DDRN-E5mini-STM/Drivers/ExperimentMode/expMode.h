/*
 * expMode.h
 *
 *  Created on: Nov 1, 2023
 *      Author: dave
 */

#ifndef EXPERIMENTMODE_EXPMODE_H_
#define EXPERIMENTMODE_EXPMODE_H_

#include "env_sensors.hpp"
#include "detector.h"
#include "sys_app.h"
#include "main.h"
#include "usart.h"
#include "stm32_seq.h"
#include "utilities_def.h"
#include "lora_app.h"

extern Detector detector;
extern EnvionmentSensors bsp_env_sensors;
extern UART_HandleTypeDef huart1;

void enterExperimentMode(void);

#endif /* EXPERIMENTMODE_EXPMODE_H_ */

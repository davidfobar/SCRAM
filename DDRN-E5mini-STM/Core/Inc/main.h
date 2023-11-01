/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RTC_N_PREDIV_S 10
#define RTC_PREDIV_S ((1<<RTC_N_PREDIV_S)-1)
#define RTC_PREDIV_A ((1<<(15-RTC_N_PREDIV_S))-1)
#define SiPM_Temp_Pin GPIO_PIN_3
#define SiPM_Temp_GPIO_Port GPIOB
#define SiPM_Signal_Pin GPIO_PIN_4
#define SiPM_Signal_GPIO_Port GPIOB
#define USARTx_RX_Pin GPIO_PIN_7
#define USARTx_RX_GPIO_Port GPIOB
#define Stim_Enable_Pin GPIO_PIN_9
#define Stim_Enable_GPIO_Port GPIOB
#define Anneal_Enable_Pin GPIO_PIN_10
#define Anneal_Enable_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_5
#define LED2_GPIO_Port GPIOB
#define Env_Sens_Enable_Pin GPIO_PIN_0
#define Env_Sens_Enable_GPIO_Port GPIOA
#define Battery_Monitor_Pin GPIO_PIN_13
#define Battery_Monitor_GPIO_Port GPIOB
#define USARTx_TX_Pin GPIO_PIN_6
#define USARTx_TX_GPIO_Port GPIOB
#define Accel_Int_Pin GPIO_PIN_0
#define Accel_Int_GPIO_Port GPIOC
#define Boot_Mode_Pin GPIO_PIN_3
#define Boot_Mode_GPIO_Port GPIOA
#define Memory_CS_Pin GPIO_PIN_2
#define Memory_CS_GPIO_Port GPIOA
#define Bias_CS_Pin GPIO_PIN_10
#define Bias_CS_GPIO_Port GPIOB
#define RF_CTRL1_Pin GPIO_PIN_4
#define RF_CTRL1_GPIO_Port GPIOA
#define RF_CTRL2_Pin GPIO_PIN_5
#define RF_CTRL2_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

#define SiPM_TEMP_ADC_CHANNEL ADC_CHANNEL_2
#define SiPM_SIGNAL_ADC_CHANNEL ADC_CHANNEL_3

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

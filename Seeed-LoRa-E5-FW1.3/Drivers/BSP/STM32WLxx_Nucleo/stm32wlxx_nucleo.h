/*
 * stm32wlxx_nucleo.h
 *
 *  Created on: Mar 18, 2022
 *      Author: Dana H. Myers
 */

#ifndef BSP_STM32WLXX_NUCLEO_STM32WLXX_NUCLEO_H_
#define BSP_STM32WLXX_NUCLEO_STM32WLXX_NUCLEO_H_

#include "main.h"

// Memory_CS
void Enable_Memory_CS(void);
void Disable_Memory_CS(void);

// Bias_CS
void Enable_Bias_CS(void);
void Disable_Bias_CS(void);

// Status LED
void Status_LED_Off(void);
void Status_LED_On(void);

// Anneal_Enable
void Enable_Anneal_LED(void);
void Disable_Anneal_LED(void);

    // Bias_Enable
void Enable_Bias(void);
void Disable_Bias(void);

// Stim_Enable
void Enable_Stim_LED(void);
void Disable_Stim_LED(void);

void testGPIO(void);

#endif /* BSP_STM32WLXX_NUCLEO_STM32WLXX_NUCLEO_H_ */

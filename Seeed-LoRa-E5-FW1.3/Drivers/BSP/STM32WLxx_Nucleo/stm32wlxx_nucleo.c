#include "stm32wlxx_nucleo.h"

// Memory_CS
void Enable_Memory_CS(void) {
	HAL_GPIO_WritePin(Memory_CS_GPIO_Port, Memory_CS_Pin, GPIO_PIN_SET);
}

void Disable_Memory_CS(void) {
	HAL_GPIO_WritePin(Memory_CS_GPIO_Port, Memory_CS_Pin, GPIO_PIN_RESET);
}

// Bias_CS
void Enable_Bias_CS(void) {
	HAL_GPIO_WritePin(Bias_CS_GPIO_Port, Bias_CS_Pin, GPIO_PIN_SET);
}

void Disable_Bias_CS(void) {
	HAL_GPIO_WritePin(Bias_CS_GPIO_Port, Bias_CS_Pin, GPIO_PIN_RESET);
}

// Staus LED
void Status_LED_Off(void) {
	HAL_GPIO_WritePin(Status_LED_GPIO_Port, Status_LED_Pin, GPIO_PIN_SET);
}

void Status_LED_On(void) {
	HAL_GPIO_WritePin(Status_LED_GPIO_Port, Status_LED_Pin, GPIO_PIN_RESET);
}

// Anneal_Enable
void Enable_Anneal_LED(void) {
	HAL_GPIO_WritePin(Anneal_Enable_GPIO_Port, Anneal_Enable_Pin, GPIO_PIN_SET);
}

void Disable_Anneal_LED(void) {
	HAL_GPIO_WritePin(Anneal_Enable_GPIO_Port, Anneal_Enable_Pin, GPIO_PIN_RESET);
}

    // Bias_Enable
void Enable_Bias(void) {
	HAL_GPIO_WritePin(Bias_Enable_GPIO_Port, Bias_Enable_Pin, GPIO_PIN_SET);
}

void Disable_Bias(void) {
	HAL_GPIO_WritePin(Bias_Enable_GPIO_Port, Bias_Enable_Pin, GPIO_PIN_RESET);
}

// Stim_Enable
void Enable_Stim_LED(void) {
	HAL_GPIO_WritePin(Stim_Enable_GPIO_Port, Stim_Enable_Pin, GPIO_PIN_SET);
}

void Disable_Stim_LED(void) {
	HAL_GPIO_WritePin(Stim_Enable_GPIO_Port, Stim_Enable_Pin, GPIO_PIN_RESET);
}

void testGPIO(void) {
	// Memory_CS
	Enable_Memory_CS();
	HAL_Delay(20);
	Disable_Memory_CS();
	HAL_Delay(20);

	// Bias_CS
	Enable_Bias_CS();
	HAL_Delay(20);
	Disable_Bias_CS();
	HAL_Delay(20);

	// Status LED
	Status_LED_Off();
	HAL_Delay(20);
	Status_LED_On();
	HAL_Delay(20);

	// Anneal_Enable
	Enable_Anneal_LED();
	HAL_Delay(20);
	Disable_Anneal_LED();
	HAL_Delay(20);

	// Bias_Enable
	Enable_Bias();
	HAL_Delay(20);
	Disable_Bias();
	HAL_Delay(20);

	// Stim_Enable
	Enable_Stim_LED();
	HAL_Delay(20);
	Disable_Stim_LED();
	HAL_Delay(20);

}


#include "digitalIO.hpp"

// Path: Core/Src/digitalIO.c

namespace dio{
    // Memory_CS
    void Memory_CS_Set(void) {
        HAL_GPIO_WritePin(Memory_CS_GPIO_Port, Memory_CS_Pin, GPIO_PIN_SET);
    }

    void Memory_CS_Reset(void) {
        HAL_GPIO_WritePin(Memory_CS_GPIO_Port, Memory_CS_Pin, GPIO_PIN_RESET);
    }

    // Bias_CS
    void Bias_CS_Set(void) {
        HAL_GPIO_WritePin(Bias_CS_GPIO_Port, Bias_CS_Pin, GPIO_PIN_SET);
    }

    void Bias_CS_Reset(void) {
        HAL_GPIO_WritePin(Bias_CS_GPIO_Port, Bias_CS_Pin, GPIO_PIN_RESET);
    }

    // LED
    void LED_Set(void) {
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    }

    void LED_Reset(void) {
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    }

    // Anneal_Enable
    void Anneal_Enable_Set(void) {
        HAL_GPIO_WritePin(Anneal_Enable_GPIO_Port, Anneal_Enable_Pin, GPIO_PIN_SET);
    }

    void Anneal_Enable_Reset(void) {
        HAL_GPIO_WritePin(Anneal_Enable_GPIO_Port, Anneal_Enable_Pin, GPIO_PIN_RESET);
    }

/*    // Bias_Enable
    void Bias_Enable_Set(void) {
        HAL_GPIO_WritePin(Bias_Enable_GPIO_Port, Bias_Enable_Pin, GPIO_PIN_SET);
    }

    void Bias_Enable_Reset(void) {
        HAL_GPIO_WritePin(Bias_Enable_GPIO_Port, Bias_Enable_Pin, GPIO_PIN_RESET);
    }

    // Stim_Enable
    void Stim_Enable_Set(void) {
        HAL_GPIO_WritePin(Stim_Enable_GPIO_Port, Stim_Enable_Pin, GPIO_PIN_SET);
    }

    void Stim_Enable_Reset(void) {
        HAL_GPIO_WritePin(Stim_Enable_GPIO_Port, Stim_Enable_Pin, GPIO_PIN_RESET);
    }*/
}
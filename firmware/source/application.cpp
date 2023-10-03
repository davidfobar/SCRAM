#include "application.hpp"

void testDigitalIO(){
    // set all digital outputs to high
    dio::LED_Set();
    HAL_Delay(10U);
    dio::Bias_CS_Set();
    HAL_Delay(10U);
    //dio::Bias_Enable_Set();
    HAL_Delay(10U);
    dio::Memory_CS_Set();
    HAL_Delay(10U);
    dio::Anneal_Enable_Set();
    HAL_Delay(10U);
    //dio::Stim_Enable_Set();
    HAL_Delay(200U);

    // set all digital outputs to low
    dio::LED_Reset();
    HAL_Delay(10U);
    dio::Bias_CS_Reset();
    HAL_Delay(10U);
    //dio::Bias_Enable_Reset();
    HAL_Delay(10U);
    dio::Memory_CS_Reset();
    HAL_Delay(10U);
    dio::Anneal_Enable_Reset();
    HAL_Delay(10U);
    //dio::Stim_Enable_Reset();
    HAL_Delay(200U);
}
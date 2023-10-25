#include "detector.h"

static float avgSipmSignalToDose(float signal) {

  //TODO: calculate the dose from the signal

  return signal;
}

bool Detector::setSiPM_Bias(float temp) {
  uint8_t dacValue = 0;

  //TODO: calculate the dacValue from the temperature

  return setSiPM_Bias(dacValue);
}

static uint32_t now() {
  //use the HAL to get the current time in milliseconds
  return HAL_GetTick();
}

static float avg(uint16_t *arr, uint16_t len) {
    float result = 0;
    for (uint16_t i = 0; i < len; i++) {
        result += arr[i];
    }
    return result / len;
}

Detector::Detector() {
  lastClearingTime      = 0;
  integratedDoseTime_ms = 0;
  measuredDose          = 0;
}

void Detector::init() {
  clearPhosphor();
}

void Detector::clearPhosphor() {
  clearLED_On();
  HAL_Delay(CLEARING_TIME_ms);
  clearLED_Off();

  lastClearingTime = now();
}

float Detector::readAccumulatedDose() {
  sampleSiPM_temp();
  float temperature = avg(SiPM_temp_measurements, NUM_TEMP_SAMPLES);
  setSiPM_Bias(temperature);
  stimLED_On();
  HAL_Delay(STABLE_MEASUREMENT_WINDOW_ms);
  sampleSiPM_signal();
  stimLED_Off();
  sipmBias_Off();
  float avgSipmSignal = avg(SiPM_signal_measurements, NUM_SiPM_SAMPLES);
  measuredDose = avgSipmSignalToDose(avgSipmSignal);
  integratedDoseTime_ms = now() - lastClearingTime;
  clearPhosphor();

  return measuredDose;
}

float Detector::calcDoseRate() {
  return measuredDose / (integratedDoseTime_ms / 1000.0);
}

void Detector::stimLED_On() {
  //use the HAL to turn on the LED
  HAL_GPIO_WritePin(Stim_Enable_GPIO_Port, Stim_Enable_Pin, GPIO_PIN_SET);
}

void Detector::stimLED_Off() {
  //use the HAL to turn off the LED
  HAL_GPIO_WritePin(Stim_Enable_GPIO_Port, Stim_Enable_Pin, GPIO_PIN_RESET);
}

void Detector::clearLED_On() {
  //use the HAL to turn on the LED
  HAL_GPIO_WritePin(Anneal_Enable_GPIO_Port, Anneal_Enable_Pin, GPIO_PIN_SET);
}

void Detector::clearLED_Off() {
  //use the HAL to turn off the LED
  HAL_GPIO_WritePin(Anneal_Enable_GPIO_Port, Anneal_Enable_Pin, GPIO_PIN_RESET);
}

void Detector::sampleSiPM_temp() {
  //reset the temperature array to 0
  memset(SiPM_temp_measurements, 0, NUM_TEMP_SAMPLES * sizeof(uint16_t));

  //set the adc to read from the SiPM temperature pin
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = SiPM_TEMP_ADC_CHANNEL;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
  
  //take NUM_TEMP_SAMPLES measurements
  for (uint16_t i = 0; i < NUM_TEMP_SAMPLES; i++) {
    //start the ADC conversion
    HAL_ADC_Start(&hadc);
    //wait for the conversion to complete
    HAL_ADC_PollForConversion(&hadc, 100);
    //read the ADC value
    SiPM_temp_measurements[i] = HAL_ADC_GetValue(&hadc);
  }
}

void Detector::sampleSiPM_signal() {
  //reset the signal array to 0
  memset(SiPM_signal_measurements, 0, NUM_SiPM_SAMPLES * sizeof(uint16_t));

  //set the adc to read from the SiPM signal pin
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = SiPM_SIGNAL_ADC_CHANNEL;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
  
  //take NUM_SiPM_SAMPLES measurements
  for (uint16_t i = 0; i < NUM_SiPM_SAMPLES; i++) {
    //start the ADC conversion
    HAL_ADC_Start(&hadc);
    //wait for the conversion to complete
    HAL_ADC_PollForConversion(&hadc, 100);
    //read the ADC value
    SiPM_signal_measurements[i] = HAL_ADC_GetValue(&hadc);
  }
}

bool Detector::sipmBias_Off() {
  uint8_t dacValue = 0;
  return setSiPM_Bias(dacValue);
}

bool Detector::setSiPM_Bias(uint8_t dacValue) {
  //use the HAL SPI to send the dacValue to the SiPM bias DAC
  
  //set the CS pin low
  HAL_GPIO_WritePin(Bias_CS_GPIO_Port, Bias_CS_Pin, GPIO_PIN_RESET);
  //send the dacValue
  HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi2, &dacValue, 1, 100);
  //set the CS pin high
  HAL_GPIO_WritePin(Bias_CS_GPIO_Port, Bias_CS_Pin, GPIO_PIN_SET);
  //check the status
  if (status != HAL_OK) {
    return false;
  }
  return true;
}
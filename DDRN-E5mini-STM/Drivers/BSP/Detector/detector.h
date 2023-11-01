#ifndef DETECTOR_H
#define DETECTOR_H

#include "main.h"
#include "adc.h"

#define NUM_TEMP_SAMPLES 16
#define NUM_SiPM_SAMPLES 16
#define STIM_STABLIZATION_ms 25
#define STABLE_MEASUREMENT_WINDOW_ms 100
#define CLEARING_TIME_ms 1000

extern ADC_HandleTypeDef hadc;
extern SPI_HandleTypeDef hspi2;

class Detector {
  public:
    Detector();

    void init();
    void clearPhosphor();
    float readAccumulatedDose();
    float calcDoseRate();
    float getSiPMtemp(bool newSample);

    bool setSiPM_Bias(uint8_t dacValue);
    bool sipmBias_Off();
  private:
    void stimLED_On();
    void stimLED_Off();

    void clearLED_On();
    void clearLED_Off();

    void sampleSiPM_temp();
    void sampleSiPM_signal();

    //bool setSiPM_Bias(float temp);

    uint32_t lastClearingTime;
    uint32_t integratedDoseTime_ms;
    float measuredDose;

    uint16_t SiPM_temp_measurements[NUM_SiPM_SAMPLES];
    uint16_t SiPM_signal_measurements[NUM_SiPM_SAMPLES];
};

#endif // DETECTOR_H 

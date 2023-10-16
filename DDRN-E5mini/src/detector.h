#ifndef DECTECTOR_H
#define DECTECTOR_H

#include "Arduino.h"

#define TEMP_ADC_PIN PB3
#define SIPM_ADC_PIN PB3
#define ANNEAL_ENABLE_PIN PA0
#define BIAS_ENABLE_PIN PA3
#define STIM_ENABLE_PIN PA2
#define BIAS_CS_PIN PA10


class Detector {
  public:
    Detector();
    void setup();
    float getDose();
    float getDoseRate();
    void clearDose();
    
  private:
    uint16_t readTemp_ADC();
    uint32_t readSiPM_ADC(int n);

    //save the time of the last dose meassurement
    unsigned long lastDoseTime;
};


#endif


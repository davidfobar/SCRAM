#ifndef DECTECTOR_H
#define DECTECTOR_H

#include "Arduino.h"

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


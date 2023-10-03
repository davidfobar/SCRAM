#ifndef __DIGITALIO_H
#define __DIGITALIO_H

#include "stm32wlxx_hal.h"
#include "main.h"

//this project has 6 digital outputs
//  Memory_CS
//  Bias_CS
//  LED ***not connected to usable pin on dev board***
//  Anneal_Enable
//  Bias_Enable
//  Stim_Enable

//provide wrapper functions for each digital output that can be called from main.c
//  and abstract away the hardware details

//wrap them in a namespace so they don't conflict with other functions

namespace dio {
    void Memory_CS_Set(void);
    void Memory_CS_Reset(void);
    void Bias_CS_Set(void);
    void Bias_CS_Reset(void);
    void LED_Set(void);
    void LED_Reset(void);
    void Anneal_Enable_Set(void);
    void Anneal_Enable_Reset(void);
    //void Bias_Enable_Set(void);
    //void Bias_Enable_Reset(void);
    //void Stim_Enable_Set(void);
    //void Stim_Enable_Reset(void);
}

#endif /* __DIGITALIO_H */
#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h> //temp and pressure
#include <Adafruit_ADXL343.h> //accelerometer

extern Adafruit_BMP3XX bmp390;
extern Adafruit_ADXL343 adxl343;

#define ACCEL_INT_PIN PC0
#define ACCEL_SENSOR_ID 1

#define DEFAULT_BMP390_ADDRESS 0x77

#define BATTERY_MONITOR_ADC_PIN PB13
#define BATTERY_MONITOR_ENABLE_PIN PB10

void setupSensors();
void adxl343Interrupt();

float getTemperature();
float getPressure();
float getAltitute();

#endif

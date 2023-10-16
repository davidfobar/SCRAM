#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h> //temp and pressure
#include <Adafruit_ADXL343.h> //accelerometer

#define ACCEL_INT_PIN PC0
#define ACCEL_SENSOR_ID 1

void setupSensors();
void adxl343Interrupt();

float getTemperature();
float getPressure();
float getAltitute();

#endif

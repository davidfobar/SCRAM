#ifndef ENV_SENSORS_H
#define ENV_SENSORS_H

#include "sys_app.h"
#include "bmp390.hpp"
#include "lsm303_accel.hpp"
#include "adc.h"
#include "main.h"

#define NUM_BATT_SAMPLES 16

extern ADC_HandleTypeDef hadc;

class EnvionmentSensors {
	public:
		EnvionmentSensors();

		bool init(I2C_HandleTypeDef *pntr_hi2c);

		float getPressure(); //returns pressure in [Pa]
		float getTemperature(); //returns temperature in units of [C]
		float getAltitude(float seaLevel); //returns the estimated altitude provided the SSL air pressure in units of hPA
		float getBattVoltage();

		lsm303AccelData getAccelData();
	private:
		LSM303_Accel lsm_accel;
		BMP390 bmp;
		uint16_t batt_adc_measurements[NUM_BATT_SAMPLES];

		void enableBattMonitor();
		void disableBattMonitor();
};

#endif //ENV_SENSORS_H




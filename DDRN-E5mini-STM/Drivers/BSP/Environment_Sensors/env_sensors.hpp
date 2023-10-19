#ifndef ENV_SENSORS_H
#define ENV_SENSORS_H

#include "sys_app.h"
#include "bmp390.hpp"
#include "lsm303_accel.hpp"
#include "main.h"

class EnvionmentSensors {
	public:
		EnvionmentSensors(I2C_HandleTypeDef *pntr_hi2c);

		float getPressure(); //returns pressure in [Pa]
		float getTemperature(); //returns temperature in units of [C]
		float getAltitude(float seaLevel); //returns the estimated altitude provided the SSL air pressure in units of hPA

		lsm303AccelData getAccelData();
	private:
		BMP390 bmp;
		LSM303_Accel lsm_accel;
};

#endif //ENV_SENSORS_H




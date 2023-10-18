#ifndef ENV_SENSORS_H
#define ENV_SENSORS_H

#include "sys_app.h"
#include "bmp390.hpp"
#include "main.h"

class EnvionmentSensors {
	public:
		EnvionmentSensors(I2C_HandleTypeDef *pntr_hi2c);

		float getPressure();
		float getTemperature();

	private:
		BMP390 bmp;
};

#endif //ENV_SENSORS_H




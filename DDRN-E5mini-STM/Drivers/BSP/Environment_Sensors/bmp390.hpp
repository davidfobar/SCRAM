/*
 * bmp390.h
 *
 *  Created on: Oct 18, 2023
 *      Author: dave
 */

#include "bmp3.h"
#include "main.h"

#ifndef BSP_ENVIRONMENT_SENSORS_BMP390_H_
#define BSP_ENVIRONMENT_SENSORS_BMP390_H_

#define BMP390_I2C_ADDRESS 0x77<<1

class BMP390{
	public:
		BMP390(I2C_HandleTypeDef *pntr_hi2c);
		float readTemperature(); //celsius
		float readPressure(); //pascals
		float getAltitude(float seaLevel);
		bool performReading();
		bool init();

	  bool setTemperatureOversampling(uint8_t os);
	  bool setPressureOversampling(uint8_t os);
	  bool setIIRFilterCoeff(uint8_t fs);
	  bool setOutputDataRate(uint8_t odr);

	  double temperature;
	  double pressure;

	private:
		struct bmp3_dev the_sensor;

		bool _filterEnabled, _tempOSEnabled, _presOSEnabled, _ODREnabled;
		uint8_t _i2caddr;
		int32_t _sensorID;
		int8_t _cs;
		unsigned long _meas_end;
};

#endif /* BSP_ENVIRONMENT_SENSORS_BMP390_H_ */

#include <env_sensors.hpp>


//#define IGNORE_ENV_SENSORS

EnvionmentSensors::EnvionmentSensors(){ }

bool EnvionmentSensors::init(I2C_HandleTypeDef *pntr_hi2c){
	APP_LOG(TS_ON, VLEVEL_M, "env sensors i2c interface: %d \r\n", pntr_hi2c);
	if( lsm_accel.init(pntr_hi2c) ){
		APP_LOG(TS_ON, VLEVEL_M, "lsm303 setup valid \r\n");
	} else {
		APP_LOG(TS_ON, VLEVEL_M, "lsm303 setup failed \r\n");
		return false;
	}

	if( bmp.init(pntr_hi2c) ){
		APP_LOG(TS_ON, VLEVEL_M, "bmp390 setup valid \r\n");
	} else {
		APP_LOG(TS_ON, VLEVEL_M, "bmp390 setup failed \r\n");
		return false;
	}
	bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
	bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
	bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
	bmp.setOutputDataRate(BMP3_ODR_50_HZ);

	return true;
}

float EnvionmentSensors::getPressure(){
	return bmp.readPressure();
}

float EnvionmentSensors::getTemperature(){
	return bmp.readTemperature();
}

float EnvionmentSensors::getAltitude(float seaLevel){
  return bmp.getAltitude(seaLevel);
}

lsm303AccelData EnvionmentSensors::getAccelData(){
  return lsm_accel.getAccelData();
}


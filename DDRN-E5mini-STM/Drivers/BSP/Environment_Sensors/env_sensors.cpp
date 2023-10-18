#include <env_sensors.hpp>

EnvionmentSensors::EnvionmentSensors(I2C_HandleTypeDef *pntr_hi2c) : bmp(pntr_hi2c) {
  if(bmp.init()){
  	APP_LOG(TS_ON, VLEVEL_M, "bmp390 setup valid \r\n");
  } else {
  	APP_LOG(TS_ON, VLEVEL_M, "bmp390 setup failed \r\n");
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
	bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
	bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
	bmp.setOutputDataRate(BMP3_ODR_50_HZ);

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

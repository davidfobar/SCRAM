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

	disableBattMonitor();

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

void EnvionmentSensors::enableBattMonitor() {
  //use the HAL to turn on the LED
  HAL_GPIO_WritePin(Env_Sens_Enable_GPIO_Port, Env_Sens_Enable_Pin, GPIO_PIN_SET);
}

void EnvionmentSensors::disableBattMonitor() {
  //use the HAL to turn off the LED
  HAL_GPIO_WritePin(Env_Sens_Enable_GPIO_Port, Env_Sens_Enable_Pin, GPIO_PIN_RESET);
}

static float avg(uint16_t *arr, uint16_t len) {
    float result = 0;
    for (uint16_t i = 0; i < len; i++) {
        result += arr[i];
    }
    return result / len;
}

float EnvionmentSensors::getBattVoltage() {

	enableBattMonitor();

  //reset the temperature array to 0
  for (uint16_t i = 0; i < NUM_BATT_SAMPLES; i++) {
  	batt_adc_measurements[i] = 0;
  }

  //set the adc to read from the SiPM temperature pin
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = BATT_VOLTAGE_ADC_CHANNEL;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);

  HAL_Delay(100);

  //toss the first measurement
  //start the ADC conversion
	HAL_ADC_Start(&hadc);
	//wait for the conversion to complete
	HAL_ADC_PollForConversion(&hadc, 100);

  //take NUM_BATT_SAMPLES measurements
  for (uint16_t i = 0; i < NUM_BATT_SAMPLES; i++) {
    //start the ADC conversion
    HAL_ADC_Start(&hadc);
    //wait for the conversion to complete
    HAL_ADC_PollForConversion(&hadc, 100);
    //read the ADC value
    batt_adc_measurements[i] = HAL_ADC_GetValue(&hadc);
    HAL_Delay(100);
  }

  disableBattMonitor();

  float battAvgADC = avg(batt_adc_measurements, NUM_BATT_SAMPLES);
  return battAvgADC * 2 * 3.3 / 4096; //the pcb has a 2x voltage divider, the analog ref voltage is 3.3, the adc has 12 bit resolution
}


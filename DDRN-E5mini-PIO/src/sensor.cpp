#include "sensor.h"

/** Global variable to determine which interrupt(s) are enabled on the ADXL343. */
int_config g_int_config_enabled = { 0 };

/** Global variables to determine which INT pin interrupt(s) are mapped to on the ADXL343. */
int_config g_int_config_map = { 0 };

// sea level pressure in hPa that can be changed externally by a LoRaWAN message
float seaLevelPressure = 1013.25;

void setupSensors() {

  //battery monitor
  pinMode(BATTERY_MONITOR_ADC_PIN, INPUT);

  if (!bmp390.begin_I2C(DEFAULT_BMP390_ADDRESS, &Wire)) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  //if (!adxl343.begin()){
  //  Serial.println("Could not find a valid ADXL343 sensor, check wiring!");
  //  while (1);
  //}

  adxl343.setRange(ADXL343_RANGE_16_G);
  pinMode(ACCEL_INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ACCEL_INT_PIN), adxl343Interrupt, RISING);

  /* Enable interrupts on the accelerometer. */
  g_int_config_enabled.bits.overrun    = false;    /* Set the INT1 */
  g_int_config_enabled.bits.watermark  = false;
  g_int_config_enabled.bits.freefall   = false;
  g_int_config_enabled.bits.inactivity = false;
  g_int_config_enabled.bits.activity   = true;
  g_int_config_enabled.bits.double_tap = false;
  g_int_config_enabled.bits.single_tap = false;
  g_int_config_enabled.bits.data_ready = false;    /* Set to INT2 */
  adxl343.enableInterrupts(g_int_config_enabled);

  /* Map specific interrupts to one of the two INT pins. */
  g_int_config_map.bits.overrun    = ADXL343_INT1;
  g_int_config_map.bits.watermark  = ADXL343_INT1;
  g_int_config_map.bits.freefall   = ADXL343_INT1;
  g_int_config_map.bits.inactivity = ADXL343_INT1;
  g_int_config_map.bits.activity   = ADXL343_INT1;
  g_int_config_map.bits.double_tap = ADXL343_INT1;
  g_int_config_map.bits.single_tap = ADXL343_INT1;
  g_int_config_map.bits.data_ready = ADXL343_INT2;
  adxl343.mapInterrupts(g_int_config_map); // Map the interrupts to the INT1 and INT2 pins.
}

void adxl343Interrupt() {
  sensors_event_t event;
  adxl343.getEvent(&event);
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
}

float getTemperature() {
  return bmp390.readTemperature();
}

float getPressure() {
  return bmp390.readPressure();
}

float getAltitute() {
  return bmp390.readAltitude(seaLevelPressure);
}
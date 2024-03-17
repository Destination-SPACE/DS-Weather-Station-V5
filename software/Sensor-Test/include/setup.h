//System libraries
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

//Sensor libraries
#include <Adafruit_LPS2X.h>
#include <Adafruit_SHT4x.h>
#include <ScioSense_ENS160.h>
#include <Adafruit_LTR390.h>
#include <SensirionI2CScd4x.h>
#include <Adafruit_VEML7700.h>

//Set variable types for functions
void INITIALIZE_SENSORS(bool, bool, bool, bool, bool, bool, bool, int);
float GET_SENSOR_DATA(bool, bool, bool, bool, bool, bool, bool);

//Units
extern bool UNITS_FEET, UNITS_METERS, UNITS_PASCAL, UNITS_MBAR, UNITS_K_PASCAL, UNITS_IN_HG, UNITS_MM_HG, UNITS_PSI, UNITS_CELCIUS, UNITS_FAHRENHEIT;

//Sensors
extern bool SENSOR_ENS160, SENSOR_GUVA_B, SENSOR_LPS22, SENSOR_LTR390, SENSOR_SCD40, SENSOR_SHT41, SENSOR_VEML7700;

//Set data types
extern float ENS160_AQI, ENS160_eCO2, ENS160_TVOC, LPS22_ALTITUDE, LPS22_PRESSURE, LPS22_TEMPERATURE, LTR390_RAW_UV, LTR390_UVI, SCD40_CO2, SCD40_HUMIDITY, SCD40_TEMPERATURE, SHT41_ABSOLUTE_HUMIDITY, SHT41_HEAT_INDEX,SHT41_HUMIDITY, SHT41_TEMPERATURE, VEML7700_LUX;

extern float ENS160_AQI_PREV;
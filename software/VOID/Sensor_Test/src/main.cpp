/*******************************************************************************
DESTINATION WEATHER FEATHER WING EXAMPLE - SENSOR TEST

This is an example sketch for the Destination Weather FeatherWing remote sensing
platform to test the SHT41 humidity & temperature sensor and the LPS22 pressure
sensor.

modified 2023-12-17
by Madison Gleydura

MIT LICENSE AGREEMENT

Copyright © 2023 Destination SPACE Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/

#include <Arduino.h>
#include "setup.h"
#include <Wire.h>

bool INITIALIZE_SENSORS(bool, bool, bool, bool, bool, bool, bool, bool, int);
void INITIALIZE_TFT();
void loop0(void * parameter);
void loop1(void * parameter);

float GET_SENSOR_DATA(bool, bool, bool, bool, bool, bool, bool, bool);



bool SENSOR_ENS160_val;
bool SENSOR_GUVA_B_val;
bool SENSOR_LPS22_val;
bool SENSOR_LTR390_val;
bool SENSOR_MAX17048_val;
bool SENSOR_SCD40_val;
bool SENSOR_SHT41_val;
bool SENSOR_VEML7700_val;

//Set data types
//float ENS160_AQI, ENS160_eCO2, ENS160_TVOC, LPS22_ALTITUDE, LPS22_PRESSURE, LPS22_TEMPERATURE, LTR390_RAW_UV, LTR390_UVI, MAX17048_CHARGE_RATE, MAX17048_PERCENTAGE, MAX17048_VOLTAGE, SCD40_CO2, SCD40_HUMIDITY, SCD40_TEMPERATURE, SHT41_ABSOLUTE_HUMIDITY, SHT41_HEAT_INDEX,SHT41_HUMIDITY, SHT41_TEMPERATURE, VEML7700_LUX;

float ENS160_AQI_PREV = 0.0f;

float REFRESH_RATE = 1000;

void setup() {
  Serial.print(115200);
  
  while(!Serial) {
    delay(10);
  }
  Wire.begin();

  Serial.print("\n\nInitializing Sensors...");
  SENSOR_ENS160_val, SENSOR_GUVA_B_val, SENSOR_LPS22_val, SENSOR_LTR390_val, SENSOR_MAX17048_val, SENSOR_SCD40_val, SENSOR_SHT41_val, SENSOR_VEML7700_val = INITIALIZE_SENSORS(SENSOR_ENS160, SENSOR_GUVA_B, SENSOR_LPS22, SENSOR_LTR390, SENSOR_MAX17048, SENSOR_SCD40, SENSOR_SHT41, SENSOR_VEML7700, REFRESH_RATE);
  Serial.print("\n\nSensor Initialization Complete!");
  Serial.print("\n\nTFT Initialization Complete!");
  Serial.print("\n\n+==============================================================================+\n|  TIME  | TEMP | HUM |  HI  | PRES | ALT | CO2 | TVOC |  AQI  |  UVI  |  LUX  |\n|hh:mm:ss| (°C) | (%) | (°C) | hPa. | (m) |(ppm)|(ppb.)|(0-300)|(0-+11)|(k-lux)|\n+==============================================================================+");
}

void loop() {
  int time_prev = millis();

  ENS160_AQI, ENS160_eCO2, ENS160_TVOC, LPS22_ALTITUDE, LPS22_PRESSURE, LPS22_TEMPERATURE, LTR390_RAW_UV, LTR390_UVI, MAX17048_CHARGE_RATE, MAX17048_PERCENTAGE, MAX17048_VOLTAGE, SCD40_CO2, SCD40_HUMIDITY, SCD40_TEMPERATURE, SHT41_ABSOLUTE_HUMIDITY, SHT41_HEAT_INDEX, SHT41_HUMIDITY, SHT41_TEMPERATURE, VEML7700_LUX = GET_SENSOR_DATA(SENSOR_ENS160_val, SENSOR_GUVA_B_val, SENSOR_LPS22_val, SENSOR_LTR390_val, SENSOR_MAX17048_val, SENSOR_SCD40_val, SENSOR_SHT41_val, SENSOR_VEML7700_val);
      
  char buffer[1024];

  sprintf(buffer,"\n|20:12:12| %4.1f |%5.2f| %4.1f |%5.1f|%5.1f| %3.0f | %4.0f | %5.1f | %5.2f |%7.3f|", SHT41_TEMPERATURE, SHT41_HUMIDITY, SHT41_HEAT_INDEX, LPS22_PRESSURE, LPS22_ALTITUDE, SCD40_CO2, ENS160_TVOC, ENS160_AQI, LTR390_UVI, VEML7700_LUX);

  Serial.print(buffer);

  while(true){
    if(millis() - time_prev >= 1000) break;
    delay(5);
  }
}
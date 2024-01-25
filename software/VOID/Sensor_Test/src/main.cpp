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
#include <Adafruit_LPS2X.h>
#include <Adafruit_SHT4x.h>
#include <ScioSense_ENS160.h>
#include <Adafruit_LTR390.h>
#include <Adafruit_MAX1704X.h>
#include <Adafruit_Sensor.h>
#include <SensirionI2CScd4x.h>
#include <Adafruit_VEML7700.h>
#include <Wire.h>

ScioSense_ENS160 ENS160(ENS160_I2CADDR_1);
Adafruit_LPS22 LPS22;
Adafruit_LTR390 LTR390 = Adafruit_LTR390();
Adafruit_MAX17048 MAX17048;
SensirionI2CScd4x SCD40;
Adafruit_SHT4x SHT41 = Adafruit_SHT4x();
Adafruit_VEML7700 VEML7700 = Adafruit_VEML7700();

bool INITIALIZE_SENSORS(bool, bool, bool, bool, bool, bool, bool, bool, int);
void INITIALIZE_TFT();
void loop0(void * parameter);
void loop1(void * parameter);

float GET_SENSOR_DATA(bool, bool, bool, bool, bool, bool, bool, bool);

bool UNITS_FEET = false;
bool UNITS_METERS = true;   //true
bool UNITS_PASCAL = false; 
bool UNITS_MBAR = true;     //true
bool UNITS_K_PASCAL = false; 
bool UNITS_IN_HG = false;
bool UNITS_MM_HG = false;
bool UNITS_PSI = false;
bool UNITS_CELCIUS = true;  //true
bool UNITS_FAHRENHEIT = false;

//Sensors
bool SENSOR_ENS160 = true;
bool SENSOR_GUVA_B = false; //false
bool SENSOR_LPS22 = true;
bool SENSOR_LTR390 = true;
bool SENSOR_MAX17048 = true;
bool SENSOR_SCD40 = true;
bool SENSOR_SHT41 = true;
bool SENSOR_VEML7700 = true;

bool SENSOR_ENS160_val;
bool SENSOR_GUVA_B_val;
bool SENSOR_LPS22_val;
bool SENSOR_LTR390_val;
bool SENSOR_MAX17048_val;
bool SENSOR_SCD40_val;
bool SENSOR_SHT41_val;
bool SENSOR_VEML7700_val;

//Set data types
float ENS160_AQI, ENS160_eCO2, ENS160_TVOC, LPS22_ALTITUDE, LPS22_PRESSURE, LPS22_TEMPERATURE, LTR390_RAW_UV, LTR390_UVI, MAX17048_CHARGE_RATE, MAX17048_PERCENTAGE, MAX17048_VOLTAGE, SCD40_CO2, SCD40_HUMIDITY, SCD40_TEMPERATURE, SHT41_ABSOLUTE_HUMIDITY, SHT41_HEAT_INDEX,SHT41_HUMIDITY, SHT41_TEMPERATURE, VEML7700_LUX;

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

bool INITIALIZE_SENSORS(bool SENSOR_ENS160, bool SENSOR_GUVA_B, bool SENSOR_LPS22, bool SENSOR_LTR390, bool SENSOR_MAX17048, bool SENSOR_SCD40, bool SENSOR_SHT41, bool SENSOR_VEML7700, int DATA_RATE){
  if(SENSOR_ENS160){
    if(!ENS160.begin()){
      Serial.print("\n\nENS160 Not Found");
      SENSOR_ENS160_val = false;
    }
    else{
      SENSOR_ENS160_val = true;
      if(!ENS160.setMode(ENS160_OPMODE_STD)){
        Serial.print("\n\nENS160 Mode-Set Failure - Restart Device");
        while(1) delay(10);
      }

      if(1/DATA_RATE > 1){
        Serial.print("\n\nRefresh Rate Too Fast! Decrease Speed");
        while(1) delay(10);
      }
      
      //Get device validity flag
      Wire.beginTransmission(ENS160_I2CADDR_1);
      Wire.requestFrom(ENS160_REG_DATA_STATUS,1);
      byte STATUS = Wire.read();
      Wire.endTransmission();
      bool VALIDITY_BIT1 = (STATUS & (1 << 1)) != 0;
      bool VALIDITY_BIT2 = (STATUS & (1 << 2)) != 0;
      int VALIDITY = (VALIDITY_BIT1 << 1) | VALIDITY_BIT2;

      //Check the status of the device
      switch(VALIDITY){
          case 0:     // Normal operation 
              break;
          case 1:     // Warm-up phase
              Serial.print("\n\nENS160 Warming Up...");
              for(int i = 180; i > 0; i--){
                  Serial.printf("\nT- %d s", i);
                  delay(1000);
              }
              break;
          case 2:     // Initial start-up phase
              Serial.print("\n\nENS160 Initial Start-up...");
              for(int i = 3600; i > 0; i--){
                  Serial.printf("\nT- %d s", i);
              }
              break;
          default:
              Serial.print("\n\nENS160 Validity Flag Retrieval  Failure - Restart Device"); while(1) delay(10);
      }
    }
  }
  if(SENSOR_GUVA_B){
      //tbd
  }
  if(SENSOR_LPS22){
    if(!LPS22.begin_I2C()){
      Serial.print("\n\nLSP22 Not Found");
      SENSOR_LPS22_val = false;
    }
    else{
      SENSOR_LPS22_val = true;
      if(1/(DATA_RATE/1000) < 10){
        LPS22.setDataRate(LPS22_RATE_10_HZ);
      }
      else if(1/(DATA_RATE/1000) < 50){
        LPS22.setDataRate(LPS22_RATE_50_HZ);
      } 
      else{
        Serial.print("\n\nRefresh Rate Too Fast! Decrease Speed");
        while(1) delay(10);
      }
    }
  }
  if(SENSOR_LTR390){
    if(!LTR390.begin()){
      Serial.print("\n\nLTR390 Not Found");
      SENSOR_LTR390_val = false;
    }
    else{
      SENSOR_LTR390_val = true;
      if(1/(DATA_RATE/1000) > 2){
        Serial.print("\n\nRefresh Rate Too Fast! Decrease Speed");
        while(1) delay(10);
      }
      LTR390.setMode(LTR390_MODE_UVS);
      LTR390.setGain(LTR390_GAIN_18);
      LTR390.setResolution(LTR390_RESOLUTION_20BIT);
      LTR390.setThresholds(100, 1000);
    }
  }
  if(SENSOR_MAX17048){
    if(!MAX17048.begin()){
      Serial.print("\n\nMAX17048 Not Found");
      SENSOR_MAX17048_val = false;
    }
    else{
      SENSOR_MAX17048_val = true;
      MAX17048.setAlertVoltages(2.0, 4.2);
    }
  }
  if(SENSOR_SCD40){
    SCD40.begin(Wire);
    delay(10);
    uint16_t error = SCD40.stopPeriodicMeasurement();
    if(error){
      Serial.print("\n\nSCD40 Not Found");
      SENSOR_SCD40_val = false;
    }
    else{
      SENSOR_SCD40_val = true;
      SCD40.startPeriodicMeasurement();
    }
  }
  if(SENSOR_SHT41){
    if(!SHT41.begin()){
      Serial.print("\n\nSHT41 Not Found");
      SENSOR_SHT41_val = false;
    }
    else{
      SENSOR_SHT41_val = true;
      SHT41.setPrecision(SHT4X_HIGH_PRECISION);
      SHT41.setHeater(SHT4X_NO_HEATER);
    }
  }
  if(SENSOR_VEML7700){
    if(!VEML7700.begin()){
      Serial.print("\n\nVEML7700 Not Found");
      SENSOR_VEML7700_val = false;
    }
    else {
      SENSOR_VEML7700_val = true;
    }
  }

  return SENSOR_ENS160_val, SENSOR_GUVA_B_val, SENSOR_LPS22_val, SENSOR_LTR390_val, SENSOR_MAX17048_val, SENSOR_SCD40_val, SENSOR_SHT41_val, SENSOR_VEML7700_val;
}

float GET_SENSOR_DATA(bool SENSOR_ENS160, bool SENSOR_GUVA_B, bool SENSOR_LPS22, bool SENSOR_LTR390, bool SENSOR_MAX17048, bool SENSOR_SCD40, bool SENSOR_SHT41, bool SENSOR_VEML7700){
  if(SENSOR_ENS160){
    ENS160_AQI = 0.0f;
    ENS160_eCO2 = 0.0f;
    ENS160_TVOC = 0.0f;
    
    if(ENS160.available()){
      ENS160.measure(true);
      ENS160.measureRaw(true);

      ENS160_AQI = ENS160.getAQI(); // algorythm from Umweltbundesamt – German Federal Environmental Agency
      ENS160_eCO2 = ENS160.geteCO2(); //ppm
      ENS160_TVOC = ENS160.getTVOC(); //ppb

      float TVOC_CONSENTRATION = ENS160_TVOC*(29/24.45); // ug/m^3

      float alpha = 0.5; //Weight factor
      ENS160_AQI = alpha*TVOC_CONSENTRATION + (1-alpha)*ENS160_AQI_PREV; //Calculated from NowCast algorithm

      ENS160_AQI_PREV = ENS160_AQI;
    }
  }
  else{
      ENS160_AQI = 0.0f;
      ENS160_eCO2 = 0.0f;
      ENS160_TVOC = 0.0f;
  }

  if(SENSOR_GUVA_B){
      //Figure out how to read from ADC
  }
  else{
      //Figure out how to read from ADC
  }

  if(SENSOR_LPS22){
    sensors_event_t LPS22_TEMPERATURE_SEN, LPS22_PRESSURE_SEN;
    LPS22.getEvent(&LPS22_PRESSURE_SEN, &LPS22_TEMPERATURE_SEN);

    LPS22_TEMPERATURE = LPS22_TEMPERATURE_SEN.temperature;

    LPS22_PRESSURE = LPS22_PRESSURE_SEN.pressure;

    LPS22_ALTITUDE = -log10(LPS22_PRESSURE/1013.25)*(8.31432*(272.15+LPS22_TEMPERATURE))/(9.80665*0.0289644);
  }
  else{
    LPS22_PRESSURE = 0.0f;
    LPS22_TEMPERATURE = 0.0f;
    LPS22_ALTITUDE = 0.0f;
  }

  if(SENSOR_LTR390){
    LTR390_RAW_UV = 0.0f;
    LTR390_UVI = 0.0f;

    if(LTR390.newDataAvailable()){
      LTR390_RAW_UV = LTR390.readUVS();
      LTR390_UVI = LTR390_RAW_UV / 2300.00;
    }
  }
  else{
    LTR390_RAW_UV = 0.0f;
    LTR390_UVI = 0.0f;
  }

  if(SENSOR_MAX17048){
    MAX17048_VOLTAGE = MAX17048.cellVoltage();
    MAX17048_PERCENTAGE = MAX17048.cellPercent();
    MAX17048_CHARGE_RATE = MAX17048.chargeRate();
  }
  else{
    MAX17048_CHARGE_RATE = 0.0f;
    MAX17048_PERCENTAGE = 0.0f;
    MAX17048_VOLTAGE = 0.0f;
  }

  if(SENSOR_SCD40){
    bool isDataReady = false;

    SCD40_CO2 = 0.0f;
    SCD40_HUMIDITY = 0.0f;
    SCD40_TEMPERATURE = 0.0f;

    uint16_t error = SCD40.getDataReadyFlag(isDataReady);
    if(!error && isDataReady){
      uint16_t SCD40_CO2_uint;
      error = SCD40.readMeasurement(SCD40_CO2_uint, SCD40_TEMPERATURE, SCD40_HUMIDITY);
      if (!error && SCD40_CO2 != 0){
        SCD40_CO2 = SCD40_CO2_uint;
        SCD40_HUMIDITY;
        SCD40_TEMPERATURE;
      }
    }
    Serial.print(SCD40_CO2);
  }
  else{
    SCD40_CO2 = 0.0f;
    SCD40_HUMIDITY = 0.0f;
    SCD40_TEMPERATURE = 0.0f;
  }

  if(SENSOR_SHT41){
    sensors_event_t SHT41_TEMPERATURE_SEN, SHT41_HUMIDITY_SEN;
    SHT41.getEvent(&SHT41_HUMIDITY_SEN, &SHT41_TEMPERATURE_SEN);

    SHT41_HUMIDITY = SHT41_HUMIDITY_SEN.relative_humidity;
    SHT41_TEMPERATURE = SHT41_TEMPERATURE_SEN.temperature;

    float h = (log10(SHT41_HUMIDITY)-2.0)/0.4343+(17.62*SHT41_TEMPERATURE)/(243.12+SHT41_TEMPERATURE);
    float SHT41_DEW_POINT = 243.12*h/(17.62-h);

    SHT41_ABSOLUTE_HUMIDITY = 216.7*(SHT41_HUMIDITY/100.0*6.112*exp(17.62*SHT41_TEMPERATURE/(243.12+SHT41_TEMPERATURE))/(275.15+SHT41_TEMPERATURE));

    SHT41_HEAT_INDEX = 1.1*SHT41_TEMPERATURE + 5*(0.047*SHT41_HUMIDITY - 7.1)/9;

    if((SHT41_HEAT_INDEX+SHT41_TEMPERATURE)/2 >= 26.7){
      SHT41_HEAT_INDEX = -8.78469475556 + 1.61139411*SHT41_TEMPERATURE + 2.33854883889*SHT41_HUMIDITY - 0.14611605*SHT41_TEMPERATURE*SHT41_HUMIDITY + 0.012308094*SHT41_TEMPERATURE*SHT41_TEMPERATURE - 0.0164248277778*SHT41_HUMIDITY*SHT41_HUMIDITY + 0.002211732*SHT41_TEMPERATURE*SHT41_TEMPERATURE*SHT41_HUMIDITY + 0.00072546*SHT41_TEMPERATURE*SHT41_HUMIDITY*SHT41_HUMIDITY - 0.000003582*SHT41_TEMPERATURE*SHT41_TEMPERATURE*SHT41_HUMIDITY*SHT41_HUMIDITY;
      
      if(SHT41_HUMIDITY < 13 && SHT41_TEMPERATURE > 26.7 && SHT41_TEMPERATURE < 44.4){
        SHT41_HEAT_INDEX = SHT41_HEAT_INDEX + (5/36)*(SHT41_HUMIDITY-13)*sqrt((17-abs(1.8*SHT41_TEMPERATURE-63))/17)-160/9;
      }
      else if(SHT41_HUMIDITY > 85 && SHT41_TEMPERATURE > 26.7 && SHT41_TEMPERATURE < 30.6){
        SHT41_HEAT_INDEX = SHT41_HEAT_INDEX + 5*(SHT41_HUMIDITY-85)*(55-1.8*SHT41_TEMPERATURE)/450-160/9;
      }
    }
  }
  else{
    SHT41_HUMIDITY = 0.0f;
    SHT41_TEMPERATURE = 0.0f;
    SHT41_ABSOLUTE_HUMIDITY = 0.0f;
    SHT41_HEAT_INDEX = 0.0f;
  }

  if(SENSOR_VEML7700){
    VEML7700_LUX = VEML7700.readLux(VEML_LUX_AUTO);
  }
  else{
    VEML7700_LUX = 0.0f;
  }

  //Unit conversions
  if(UNITS_FEET){
    LPS22_ALTITUDE = LPS22_ALTITUDE * 0.0254/12;
  }
  if(UNITS_PASCAL){
    LPS22_PRESSURE = LPS22_PRESSURE * 100;
  }
  else if(UNITS_K_PASCAL){
    LPS22_PRESSURE = LPS22_PRESSURE * 0.1;
  }
  else if(UNITS_IN_HG){
    LPS22_PRESSURE = LPS22_PRESSURE * 0.029529983071445;
  }
  else if(UNITS_MM_HG){
    LPS22_PRESSURE = LPS22_PRESSURE * 0.75006157584566;
  }
  else if(UNITS_PSI){
    LPS22_PRESSURE = LPS22_PRESSURE * 0.01450377;
  }
  else{}

  if(UNITS_FAHRENHEIT){
    LPS22_TEMPERATURE*(9/5)+32;
    SCD40_TEMPERATURE*(9/5)+32;
    SHT41_TEMPERATURE*(9/5)+32;
  }

  return ENS160_AQI, ENS160_eCO2, ENS160_TVOC, LPS22_ALTITUDE, LPS22_PRESSURE, LPS22_TEMPERATURE, LTR390_RAW_UV, LTR390_UVI, MAX17048_CHARGE_RATE, MAX17048_PERCENTAGE, MAX17048_VOLTAGE, SCD40_CO2, SCD40_HUMIDITY, SCD40_TEMPERATURE, SHT41_ABSOLUTE_HUMIDITY, SHT41_HEAT_INDEX, SHT41_HUMIDITY, SHT41_TEMPERATURE, VEML7700_LUX;
}
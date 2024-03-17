#include "setup.h"

float ENS160_AQI_PREV = 0.0f;

bool SENSOR_ENS160 = true;
bool SENSOR_GUVA_B = false; //false
bool SENSOR_LPS22 = true;
bool SENSOR_LTR390 = true;
bool SENSOR_SCD40 = true;
bool SENSOR_SHT41 = true;
bool SENSOR_VEML7700 = true;

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

ScioSense_ENS160 ENS160(0x52);
Adafruit_LPS22 LPS22;
Adafruit_LTR390 LTR390 = Adafruit_LTR390();
SensirionI2CScd4x SCD40;
Adafruit_SHT4x SHT41 = Adafruit_SHT4x();
Adafruit_VEML7700 VEML7700 = Adafruit_VEML7700();

void INITIALIZE_SENSORS(bool SENSOR_ENS160, bool SENSOR_GUVA_B, bool SENSOR_LPS22, bool SENSOR_LTR390, bool SENSOR_SCD40, bool SENSOR_SHT41, bool SENSOR_VEML7700, int DATA_RATE){
  if(SENSOR_ENS160){
    if(!ENS160.begin()){
      Serial.print("\n\nENS160 Not Found");
      SENSOR_ENS160 = false;
    }
    else{
      if(1/DATA_RATE > 1){
        Serial.print("\n\nRefresh Rate Too Fast! Decrease Speed");
        while(1){
          delay(10);
        }
      }
    }
  }

  if(SENSOR_GUVA_B){
      //tbd
  }

  if(SENSOR_LPS22){
    if(!LPS22.begin_I2C()){
      Serial.print("\n\nLSP22 Not Found");
      SENSOR_LPS22 = false;
    }
    else{
      if(1/DATA_RATE < 10){
        LPS22.setDataRate(LPS22_RATE_10_HZ);
        Serial.print("\n\nLPS22 Set to 10Hz");
        SENSOR_LPS22 = true;
      }
      else if(1/DATA_RATE < 50){
        LPS22.setDataRate(LPS22_RATE_50_HZ);
        Serial.print("\n\nLPS22 Set to 50Hz");
        SENSOR_LPS22 = true;
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
        SENSOR_LTR390 = false;
    }
    else{
      if(1/DATA_RATE > 2){
        Serial.print("\n\nRefresh Rate Too Fast! Decrease Speed");
        while(1) delay(10);
      }
      else{
        LTR390.setMode(LTR390_MODE_UVS);
        LTR390.setGain(LTR390_GAIN_18);
        LTR390.setResolution(LTR390_RESOLUTION_20BIT);
        LTR390.setThresholds(100, 1000);
        Serial.print("\n\nLTR390 Initialized!");
        SENSOR_LTR390 = true;
      }
    }
  }

  if(SENSOR_SCD40){
    SCD40.begin(Wire);
    delay(10);
    if(!SCD40.startPeriodicMeasurement()){
      Serial.print("\n\nSCD40 Not Found");
      SENSOR_SCD40 = false;
    } 
    else{
      SCD40.startPeriodicMeasurement();
      Serial.print("\n\nSCD40 Initialized!");
      SENSOR_SCD40 = true;
    }
  }

  if(SENSOR_SHT41){
    if(!SHT41.begin()){
      Serial.print("\n\nSHT41 Not Found");
      SENSOR_SHT41 = false;
    }
    else{
      SHT41.setPrecision(SHT4X_HIGH_PRECISION);
      SHT41.setHeater(SHT4X_NO_HEATER);
      Serial.print("\n\nSHT41 Initialized!");
      SENSOR_SHT41 = true;
    }
  }

  if(SENSOR_VEML7700){
    if(!VEML7700.begin()){
      Serial.print("\n\nVEML7700 Not Found");
      SENSOR_VEML7700 = false;
    }
    else{
      Serial.print("\n\nVEML7700 Initialized!");
      SENSOR_VEML7700 = true;
    }
  }
}

float GET_SENSOR_DATA(bool SENSOR_ENS160, bool SENSOR_GUVA_B, bool SENSOR_LPS22, bool SENSOR_LTR390, bool SENSOR_SCD40, bool SENSOR_SHT41, bool SENSOR_VEML7700){
  Serial.println("ENS");
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
  Serial.println("LPS");
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
  Serial.println("LTR");
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
  Serial.println("before");
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
  }
  else{
    SCD40_CO2 = 0.0f;
    SCD40_HUMIDITY = 0.0f;
    SCD40_TEMPERATURE = 0.0f;
  }
  Serial.println("after");
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

  return ENS160_AQI, ENS160_eCO2, ENS160_TVOC, LPS22_ALTITUDE, LPS22_PRESSURE, LPS22_TEMPERATURE, LTR390_RAW_UV, LTR390_UVI, SCD40_CO2, SCD40_HUMIDITY, SCD40_TEMPERATURE, SHT41_ABSOLUTE_HUMIDITY, SHT41_HEAT_INDEX, SHT41_HUMIDITY, SHT41_TEMPERATURE, VEML7700_LUX;
}
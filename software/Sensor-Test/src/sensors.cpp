#include "setup.h"

float aqiPrev = 0.0f;

units unit;
sensors sen;
parameters param;

ScioSense_ENS160 ENS160(0x52);
Adafruit_LPS22 LPS22;
Adafruit_LTR390 LTR390 = Adafruit_LTR390();
SensirionI2CScd4x SCD40;
Adafruit_SHT4x SHT41 = Adafruit_SHT4x();
Adafruit_VEML7700 VEML7700 = Adafruit_VEML7700();

units getUnits(void){
  unit.feet = false;
  unit.meters = true;

  unit.pascal = false;
  unit.mbar = true;
  unit.kpa = false;
  unit.inhg = false;
  unit.mmhg = false;
  unit.psi = false;

  unit.celcius = true;
  unit.fahrenheit = false;

  return unit;
}

sensors getSensors(void){
  sen.ens160 = true;
  sen.lps22 = true;
  sen.ltr390 = true;
  sen.scd40 = true;
  sen.sht41 = true;
  sen.veml7700 = true;

  return sen;
}

void INITIALIZE_SENSORS(sensors sen, int DATA_RATE){
  if(sen.ens160){
    if(!ENS160.begin()){
      Serial.print("\n\nENS160 Not Found");
      sen.ens160 = false;
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

  if(sen.lps22){
    if(!LPS22.begin_I2C()){
      Serial.print("\n\nLSP22 Not Found");
      sen.lps22 = false;
    }
    else{
      if(1/DATA_RATE < 10){
        LPS22.setDataRate(LPS22_RATE_10_HZ);
        Serial.print("\n\nLPS22 Set to 10Hz");
        sen.lps22 = true;
      }
      else if(1/DATA_RATE < 50){
        LPS22.setDataRate(LPS22_RATE_50_HZ);
        Serial.print("\n\nLPS22 Set to 50Hz");
        sen.lps22 = true;
      }
      else{
        Serial.print("\n\nRefresh Rate Too Fast! Decrease Speed");
        while(1) delay(10);
      }
    }
  }

  if(sen.ltr390){
    if(!LTR390.begin()){
        Serial.print("\n\nLTR390 Not Found");
        sen.ltr390 = false;
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
        sen.ltr390 = true;
      }
    }
  }

  if(sen.scd40){
    SCD40.begin(Wire);
    delay(10);
    if(!SCD40.startPeriodicMeasurement()){
      Serial.print("\n\nSCD40 Not Found");
      sen.scd40 = false;
    } 
    else{
      SCD40.startPeriodicMeasurement();
      Serial.print("\n\nSCD40 Initialized!");
      sen.scd40 = true;
    }
  }

  if(sen.sht41){
    if(!SHT41.begin()){
      Serial.print("\n\nSHT41 Not Found");
      sen.sht41 = false;
    }
    else{
      SHT41.setPrecision(SHT4X_HIGH_PRECISION);
      SHT41.setHeater(SHT4X_NO_HEATER);
      Serial.print("\n\nSHT41 Initialized!");
      sen.sht41 = true;
    }
  }

  if(sen.veml7700){
    if(!VEML7700.begin()){
      Serial.print("\n\nVEML7700 Not Found");
      sen.veml7700 = false;
    }
    else{
      Serial.print("\n\nVEML7700 Initialized!");
      sen.veml7700 = true;
    }
  }
}

parameters GET_SENSOR_DATA(sensors sen){
  Serial.println("ENS");
  if(sen.ens160){
    param.aqi = 0.0f;
    param.eCO2 = 0.0f;
    param.tvoc = 0.0f;
    
    if(ENS160.available()){
      ENS160.measure(true);
      ENS160.measureRaw(true);

      param.aqi = ENS160.getAQI(); // algorythm from Umweltbundesamt – German Federal Environmental Agency
      param.eCO2 = ENS160.geteCO2(); //ppm
      param.tvoc = ENS160.getTVOC(); //ppb

      float tvocCon = param.tvoc*(29/24.45); // ug/m^3

      float alpha = 0.5; //Weight factor
      param.aqi = alpha*tvocCon + (1-alpha)*aqiPrev; //Calculated from NowCast algorithm

      aqiPrev = param.aqi;
    }
  }
  else{
    param.aqi = 0.0f;
    param.aqi = 0.0f;
    param.aqi = 0.0f;
  }
  Serial.println("LPS");
  if(sen.lps22){
    sensors_event_t LPS22_TEMPERATURE_SEN, LPS22_PRESSURE_SEN;
    LPS22.getEvent(&LPS22_PRESSURE_SEN, &LPS22_TEMPERATURE_SEN);

    param.tempLPS = LPS22_TEMPERATURE_SEN.temperature;

    param.pres = LPS22_PRESSURE_SEN.pressure;

    param.alt = -log10(param.pres/1013.25)*(8.31432*(272.15+param.tempLPS))/(9.80665*0.0289644);
  }
  else{
    param.pres = 0.0f;
    param.tempLPS = 0.0f;
    param.alt = 0.0f;
  }
  Serial.println("LTR");
  if(sen.ltr390){
    param.uvRaw = 0.0f;
    param.uviLTR = 0.0f;
    param.alsLTR = 0.0f;

    if(LTR390.newDataAvailable()){
      param.uvRaw = LTR390.readUVS();
      param.uviLTR = param.uvRaw / 2300.00;
      param.alsLTR = LTR390.readALS();
    }
  }
  else{
    param.uvRaw = 0.0f;
    param.uviLTR = 0.0f;
    param.alsLTR = 0.0f;
  }
  Serial.println("SCD40");
  if(sen.scd40){
    bool isDataReady = false;

    param.CO2 = 0.0f;
    param.humdSCD = 0.0f;
    param.tempSCD = 0.0f;

    uint16_t error = SCD40.getDataReadyFlag(isDataReady);
    if(!error && isDataReady){
      uint16_t SCD40_CO2_uint;
      error = SCD40.readMeasurement(SCD40_CO2_uint, param.tempSCD, param.humdSCD);
      if (!error && param.CO2 != 0){
        param.CO2 = SCD40_CO2_uint;
        param.humdSCD;
        param.tempSCD;
      }
    }
  }
  else{
    param.CO2 = 0.0f;
    param.humdSCD = 0.0f;
    param.tempSCD = 0.0f;
  }
  Serial.println("after");
  if(sen.sht41){
    sensors_event_t SHT41_TEMPERATURE_SEN, SHT41_HUMIDITY_SEN;
    SHT41.getEvent(&SHT41_HUMIDITY_SEN, &SHT41_TEMPERATURE_SEN);

    param.humdSHT = SHT41_HUMIDITY_SEN.relative_humidity;
    param.tempSHT = SHT41_TEMPERATURE_SEN.temperature;

    float h = (log10(param.humdSHT)-2.0)/0.4343+(17.62*param.tempSHT)/(243.12+param.tempSHT);
    param.dewPoint = 243.12*h/(17.62-h);

    param.humdABS = 216.7*(param.humdSHT/100.0*6.112*exp(17.62*param.tempSHT/(243.12+param.tempSHT))/(275.15+param.tempSHT));

    param.heatIndex = 1.1*param.tempSHT + 5*(0.047*param.humdSHT - 7.1)/9;

    if((param.heatIndex+param.tempSHT)/2 >= 26.7){
      param.heatIndex = -8.78469475556 + 1.61139411*param.tempSHT + 2.33854883889*param.humdSHT - 0.14611605*param.tempSHT*param.humdSHT + 0.012308094*param.tempSHT*param.tempSHT - 0.0164248277778*param.humdSHT*param.humdSHT + 0.002211732*param.tempSHT*param.tempSHT*param.humdSHT + 0.00072546*param.tempSHT*param.humdSHT*param.humdSHT - 0.000003582*param.tempSHT*param.tempSHT*param.humdSHT*param.humdSHT;
      
      if(param.humdSHT < 13 && param.tempSHT > 26.7 && param.tempSHT < 44.4){
        param.heatIndex = param.heatIndex + (5/36)*(param.heatIndex-13)*sqrt((17-abs(1.8*param.tempSHT-63))/17)-160/9;
      }
      else if(param.humdSHT > 85 && param.tempSHT > 26.7 && param.tempSHT < 30.6){
        param.heatIndex = param.heatIndex + 5*(param.humdSHT-85)*(55-1.8*param.tempSHT)/450-160/9;
      }
    }
  }
  else{
    param.humdSHT = 0.0f;
    param.tempSHT = 0.0f;
    param.humdABS = 0.0f;
    param.heatIndex = 0.0f;
  }

  if(sen.veml7700){
    param.alsVEML = VEML7700.readLux(VEML_LUX_AUTO);
  }
  else{
    param.alsVEML = 0.0f;
  }

  //Unit conversions
  if(unit.feet){
    param.alt = param.alt * 0.0254/12;
  }
  if(unit.pascal){
    param.pres = param.pres * 100;
  }
  else if(unit.kpa){
    param.pres = param.pres * 0.1;
  }
  else if(unit.inhg){
    param.pres = param.pres * 0.029529983071445;
  }
  else if(unit.mmhg){
    param.pres = param.pres * 0.75006157584566;
  }
  else if(unit.psi){
    param.pres = param.pres * 0.01450377;
  }
  else{}

  if(unit.fahrenheit){
    param.tempLPS*(9/5)+32;
    param.tempSCD*(9/5)+32;
    param.tempSHT*(9/5)+32;
  }

  return param;
}
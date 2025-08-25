#include "setup.h"

sensors sen;
parameters param;
configuration config;

SdFat SD;
SdFile file;

String alt, pres, temp, alt_file, pres_file, temp_file, presDisp, tempDisp;

ScioSense_ENS160 ENS160(ENS160_I2CADDR_0);
Adafruit_LPS22 LPS22;
Adafruit_LTR390 LTR390 = Adafruit_LTR390();
Adafruit_MAX17048 MAX17048;
SensirionI2CScd4x SCD40;
Adafruit_SHT4x SHT41 = Adafruit_SHT4x();
Adafruit_VEML7700 VEML7700 = Adafruit_VEML7700();
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
//Adafruit_USBD_MSC USB_MSC;

configuration getConfig(void){
  if(!file.open("config.json", FILE_READ)){
    Serial.print("\nFailed to open config.json or file does not exist");
  }
  if(file){
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    if(error){
      Serial.print("\nDeserializeJson() failed");
      Serial.print(error.f_str());
    }

    config.studentName = doc["STUDENT_NAME"] | "NAME";
    config.refreshRate = doc["REFRESH_RATE"] | 1000;
    config.wifiSSID = doc["NETWORK"]["SSID"] | "wifi";
    config.wifiPASS = doc["NETWORK"]["PASSWORD"] | "password";

    config.neoPixelRGB = doc["NEO_PIXEL"]["RGB"]["STS"] | true;
    config.neoPixelRed = doc["NEO_PIXEL"]["RGB"]["RED"] | 0;
    config.neoPixelGrn = doc["NEO_PIXEL"]["RGB"]["GREEN"] | 0;
    config.neoPixelBlu = doc["NEO_PIXEL"]["RGB"]["BLUE"] | 30;
    config.neoPixelBri = doc["NEO_PIXEL"]["RGB"]["BRIGHTNESS"] | 10;

    config.neoPixelHSV = doc["NEO_PIXEL"]["HSV"]["STS"] | false;
    config.neoPixelHue = doc["NEO_PIXEL"]["HSV"]["HUE"] | 0;
    config.neoPixelSat = doc["NEO_PIXEL"]["HSV"]["SATURATION"] | 0;
    config.neoPixelVal = doc["NEO_PIXEL"]["HSV"]["VALUE"] | 0;

    file.close();
  }
  else{
    Serial.print("\nCould not open config.json");
  }
  return config;
}

sensors getSensors(void){
  if(!file.open("config.json", O_RDONLY)){
    Serial.print("\nFailed to open config.json or file does not exist");
  }
  if(file){
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    if(error){
      Serial.print("\nDeserializeJson() failed");
      Serial.print(error.f_str());
    }

    sen.ens160 = doc["SENSORS"]["ENS160"] | false;
    sen.lps22 = doc["SENSORS"]["LPS22"] | false;
    sen.ltr390 = doc["SENSORS"]["LTR390"] | false;
    sen.max1704 = doc["SENSORS"]["MAX17048"] | false;
    sen.scd40 = doc["SENSORS"]["SCD40"] | false;
    sen.sht41 = doc["SENSORS"]["SHT41"] | false;
    sen.veml7700 = doc["SENSORS"]["VEML7700"] | false;

    file.close();
  }
  else{
    Serial.print("\nCould not open config.json");
  }
  return sen;
}

sensors initializeSensors(sensors sen){
  if(sen.ens160){
    if(!ENS160.begin()){
      Serial.print("\n\nENS160 not found");
      sen.ens160 = false;
    }
    else{
      if(!ENS160.setMode(ENS160_OPMODE_STD)){
        Serial.print("\n\nENS160 failed to init");
        sen.ens160 = false;
      }
    }
  }

  if(sen.lps22){
    if(!LPS22.begin_I2C()){
      Serial.print("\n\nLSP22 not found");
      sen.lps22 = false;
    }
    else{
      LPS22.setDataRate(LPS22_RATE_10_HZ);
    }
  }

  if(sen.ltr390){
    if(!LTR390.begin()){
        Serial.print("\n\nLTR390 not found");
        sen.ltr390 = false;
    }
    else{
      LTR390.setMode(LTR390_MODE_UVS);
      LTR390.setGain(LTR390_GAIN_18);
      LTR390.setResolution(LTR390_RESOLUTION_20BIT);
      LTR390.setThresholds(100, 1000);
      LTR390.configInterrupt(true, LTR390_MODE_UVS);
    }
  }

  if(sen.max1704){
    if(!MAX17048.begin()){
      Serial.print("\n\nMAX17048 not found.");
      sen.max1704 = false;
    }
    else{
      MAX17048.setAlertVoltages(2.0, 4.2);
      sen.max1704 = true;
    }
  }

  if(sen.scd40){
    SCD40.begin(Wire);
    if(SCD40.stopPeriodicMeasurement() || SCD40.startPeriodicMeasurement()){
      Serial.print("\n\nSCD40 failed to respond");
      sen.scd40 = false;
    }
  }

  if(sen.sht41){
    if(!SHT41.begin()){
      Serial.print("\n\nSHT41 not found");
      sen.sht41 = false;
    }
    else{
      SHT41.setPrecision(SHT4X_HIGH_PRECISION);
      SHT41.setHeater(SHT4X_NO_HEATER);
    }
  }

  if(sen.veml7700){
    if(!VEML7700.begin()){
      Serial.print("\n\nVEML7700 not found");
      sen.veml7700 = false;
    }
    else{
      VEML7700.setLowThreshold(10000);
      VEML7700.setHighThreshold(20000);
      VEML7700.interruptEnable(true);
    }
  }

  return sen;
}

parameters getSensorData(sensors sen){
  if(sen.ens160 && ENS160.available()){
    ENS160.measure(true);
    ENS160.measureRaw(true);

    param.aqi = ENS160.getAQI(); // algorithm from Umweltbundesamt – German Federal Environmental Agency
    param.eCO2 = ENS160.geteCO2(); //ppm
    param.tvoc = ENS160.getTVOC(); //ppb
  }
  else{
    param.aqi = 0.0f;
    param.aqi = 0.0f;
    param.aqi = 0.0f;
  }

  if(sen.lps22){
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

  if(sen.ltr390 && LTR390.newDataAvailable()){
    param.uvRaw = LTR390.readUVS();
    param.uviLTR = param.uvRaw / 2300.00;
    param.alsLTR = LTR390.readALS();
  }
  else{
    param.uvRaw = 0.0f;
    param.uviLTR = 0.0f;
    param.alsLTR = 0.0f;
  }

  if(sen.max1704){
    param.voltage = MAX17048.cellVoltage();
    param.battPercent = MAX17048.cellPercent();
    param.chrgRate = MAX17048.chargeRate();
  }
  else{
    param.voltage = 0.0f;
    param.battPercent = 0.0f;
    param.chrgRate = 0.0f;
  }

  if(sen.scd40){
    bool isDataReady = false;

    uint16_t error = SCD40.getDataReadyFlag(isDataReady);
    if(SCD40.getDataReadyFlag(isDataReady)){
    }

    if(isDataReady){
      uint16_t SCD40_CO2_uint;
      if(SCD40.readMeasurement(SCD40_CO2_uint, param.tempSCD, param.humdSCD)){
      }
      else if(SCD40_CO2_uint == 0){
      }
      else{
        param.CO2 = static_cast<float>(SCD40_CO2_uint);
        param.humdSCD;
        param.tempSCD;
      }
    }
    else{
    }
  }
  else{
    param.CO2 = 0.0f;
    param.humdSCD = 0.0f;
    param.tempSCD = 0.0f;
  }

  if(sen.sht41){
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

  return param;
}
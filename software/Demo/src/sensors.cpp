#include "setup.h"

units unit;
sensors sen;
parameters param;
configuration config;

const char* yamlString = "";

ScioSense_ENS160 ENS160(ENS160_I2CADDR_0);
Adafruit_LPS22 LPS22;
Adafruit_LTR390 LTR390 = Adafruit_LTR390();
Adafruit_MAX17048 MAX17048;
SensirionI2CScd4x SCD40;
Adafruit_SHT4x SHT41 = Adafruit_SHT4x();
Adafruit_VEML7700 VEML7700 = Adafruit_VEML7700();
Adafruit_ST7789 TFT = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

//Adafruit_USBD_MSC USB_MSC;
DynamicJsonDocument doc(128);
JsonObject json_obj = doc.as<JsonObject>();


void deserializeYAML(void){
  if(file.open("config.yaml", O_READ)){
    while(file.available()){
      yamlString += (char)file.read();
    }
    file.close();
    
    deserializeYml(json_obj, yamlString); // deserialize yaml string to JsonObject
  }
  else{
    Serial.print("\n\nCant read config file");
  }
}

configuration getConfig(void){
  config.studentName = json_obj["STUDENT_NAME"];
  config.refreshRate = json_obj["REFRESH_RATE"];

  JsonObject NETWORK_obj = json_obj["NETWORK"].as<JsonObject>();

  config.wifiSSID = NETWORK_obj["SSID"];
  config.wifiPASS = NETWORK_obj["PASSWORD"];

  JsonObject NEOPIXEL_obj = json_obj["NEO_PIXEL"].as<JsonObject>();
  JsonObject NEOPIXEL_RGB_obj = NEOPIXEL_obj["RGB"].as<JsonObject>();
  JsonObject NEOPIXEL_HSV_obj = NEOPIXEL_obj["HSV"].as<JsonObject>();
  
  config.neoPixelRGB = NEOPIXEL_RGB_obj["STS"];
  config.neoPixelRed = NEOPIXEL_RGB_obj["RED"];
  config.neoPixelGrn = NEOPIXEL_RGB_obj["GREEN"];
  config.neoPixelBlu = NEOPIXEL_RGB_obj["BLUE"];
  config.neoPixelBri = NEOPIXEL_RGB_obj["BRIGHTNESS"];

  config.neoPixelHSV = NEOPIXEL_HSV_obj["STS"];
  config.neoPixelHue = NEOPIXEL_HSV_obj["HUE"];
  config.neoPixelSat = NEOPIXEL_HSV_obj["SATURATION"];
  config.neoPixelVal = NEOPIXEL_HSV_obj["VALUE"];

  return config;
}

units getUnits(void){
  JsonObject UNITS_obj = json_obj["UNITS"].as<JsonObject>();

  JsonObject ALTITUDE_obj = UNITS_obj["ALTITUDE"].as<JsonObject>();

  unit.feet = ALTITUDE_obj["FEET"];
  unit.meters = ALTITUDE_obj["METERS"];

  JsonObject PRESSURE_obj = UNITS_obj["PRESSURE"].as<JsonObject>();

  unit.pascal = PRESSURE_obj["PASCAL"];
  unit.mbar = PRESSURE_obj["MBAR"];
  unit.kpa = PRESSURE_obj["K_PASCAL"];
  unit.inhg = PRESSURE_obj["IN_HG"];
  unit.mmhg = PRESSURE_obj["MM_HG"];
  unit.psi = PRESSURE_obj["PSI"]; 

  JsonObject TEMPERATURE_obj = UNITS_obj["TEMPERATURE"].as<JsonObject>();

  unit.celsius = TEMPERATURE_obj["CELSIUS"];
  unit.fahrenheit = TEMPERATURE_obj["FAHRENHEIT"];

  return unit;
}

sensors getSensors(void){
  JsonObject SENSORS_obj = json_obj["SENSORS"].as<JsonObject>();

  sen.ens160 = SENSORS_obj["ENS160"];
  sen.lps22 = SENSORS_obj["LPS22"];
  sen.ltr390 = SENSORS_obj["LTR390"];
  sen.max1704 = SENSORS_obj["MAX17048"];
  sen.scd40 = SENSORS_obj["SCD40"];
  sen.sht41 = SENSORS_obj["SHT41"];
  sen.veml7700 = SENSORS_obj["VEML7700"];

  return sen;
}

sensors initializeSensors(sensors sen){
  if(sen.ens160){
    if(!ENS160.begin()){
      Serial.print("\n\nENS160 not found.");
      sen.ens160 = false;
    }
    else{
      if(!ENS160.setMode(ENS160_OPMODE_STD)){
        Serial.print("\n\nENS160 failed to init.");
        sen.ens160 = false;
      }
    }
  }

  if(sen.lps22){
    if(!LPS22.begin_I2C()){
      Serial.print("\n\nLSP22 not found.");
      sen.lps22 = false;
    }
    else{
      LPS22.setDataRate(LPS22_RATE_10_HZ);
    }
  }

  if(sen.ltr390){
    if(!LTR390.begin()){
        Serial.print("\n\nLTR390 not found.");
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
      Serial.print("\n\nSCD40 failed to respond.");
      sen.scd40 = false;
    }
  }

  if(sen.sht41){
    if(!SHT41.begin()){
      Serial.print("\n\nSHT41 not found.");
      sen.sht41 = false;
    }
    else{
      SHT41.setPrecision(SHT4X_HIGH_PRECISION);
      SHT41.setHeater(SHT4X_NO_HEATER);
    }
  }

  if(sen.veml7700){
    if(!VEML7700.begin()){
      Serial.print("\n\nVEML7700 not found.");
      sen.veml7700 = false;
    }
  }

  return sen;
}

void initializeTFT(){
  pinMode(TFT_BACKLIGHT, OUTPUT);
  pinMode(TFT_I2C_POWER, OUTPUT);

  digitalWrite(TFT_BACKLIGHT, HIGH);
  digitalWrite(TFT_I2C_POWER, HIGH);

  TFT.init(135, 240); // Define TFT resolution
  TFT.setRotation(3); // CCW orientation
  TFT.fillScreen(ST77XX_BLACK); // Clear screen
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
    param.eCO2 = 0.0f;
    param.tvoc = 0.0f;
  }

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

  if(sen.ltr390 && LTR390.newDataAvailable()){
    param.uvRaw = LTR390.readUVS();
    param.uvi = param.uvRaw / 2300.00;
    param.alsLTR = LTR390.readALS();
  }
  else{
    param.uvRaw = 0.0f;
    param.uvi = 0.0f;
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
    sensors_event_t SHT41_TEMPERATURE_SEN, SHT41_HUMIDITY_SEN;
    SHT41.getEvent(&SHT41_HUMIDITY_SEN, &SHT41_TEMPERATURE_SEN);

    param.humdSHT = SHT41_HUMIDITY_SEN.relative_humidity;
    param.tempSHT = SHT41_TEMPERATURE_SEN.temperature;

    float h = (log10(param.humdSHT)-2.0)/0.4343+(17.62*param.tempSHT)/(243.12+param.tempSHT);
    param.dewPoint = 243.12*h/(17.62-h);

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
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SdFat.h>

//Configuration
typedef struct{
    const char* studentName;
    int refreshRate;
    const char* wifiSSID;
    const char* wifiPASS;
    bool neoPixelRGB;
    bool neoPixelHSV;
    int neoPixelRed;
    int neoPixelGrn;
    int neoPixelBlu;
    int neoPixelBri;
    int neoPixelHue;
    int neoPixelSat;
    int neoPixelVal;
} configuration;

//Units
typedef struct{
    bool feet;
    bool meters;
    bool pascal;
    bool mbar;
    bool kpa;
    bool inhg;
    bool mmhg;
    bool psi;
    bool celsius;
    bool fahrenheit;
} units;

//Senors
typedef struct{
    bool ens160;
    bool lps22;
    bool ltr390;
    bool max1704;
    bool scd40;
    bool sht41;
    bool veml7700;
} sensors;

SdFat SD;
SdFile file;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  if(!SD.begin(5)){
    Serial.print("\nInitialization failed!");
  }
  else{
    Serial.print("\nInitialization complete!");
  }

  if(!file.open("config.json", FILE_READ)){
    Serial.print("\nFailed to open config.json");
  }

  if(file){
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    if(error){
      Serial.print("\nDeserializeJson() failed");
      Serial.print(error.f_str());
    }
    configuration config;
    units unit;
    sensors sensor;

    config.studentName = doc["STUDENT_NAME"];
    config.refreshRate = doc["REFRESH_RATE"];
    config.wifiSSID = doc["NETWORK"]["SSID"];
    config.wifiPASS = doc["NETWORK"]["PASSWORD"];
    config.neoPixelRGB = doc["NEO_PIXEL"]["RGB"]["STS"];
    config.neoPixelRed = doc["NEO_PIXEL"]["RGB"]["RED"];
    config.neoPixelGrn = doc["NEO_PIXEL"]["RGB"]["GREEN"];
    config.neoPixelBlu = doc["NEO_PIXEL"]["RGB"]["BLUE"];
    config.neoPixelBri = doc["NEO_PIXEL"]["RGB"]["BRIGHTNESS"];
    config.neoPixelHSV = doc["NEO_PIXEL"]["HSV"]["STS"];
    config.neoPixelHue = doc["NEO_PIXEL"]["HSV"]["HUE"];
    config.neoPixelSat = doc["NEO_PIXEL"]["HSV"]["SATURATION"];
    config.neoPixelVal = doc["NEO_PIXEL"]["HSV"]["VALUE"];

    unit.feet = doc["UNITS"]["ALTITUDE"]["FEET"] | false;
    unit.meters = doc["UNITS"]["ALTITUDE"]["METERS"] | false;
    unit.pascal = doc["UNITS"]["PRESSURE"]["PASCAL"] | false;
    unit.mbar = doc["UNITS"]["PRESSURE"]["MBAR"] | false;
    unit.kpa = doc["UNITS"]["PRESSURE"]["K_PASCAL"] | false;
    unit.inhg = doc["UNITS"]["PRESSURE"]["IN_HG"] | false;
    unit.mmhg = doc["UNITS"]["PRESSURE"]["MM_HG"] | false;
    unit.psi = doc["UNITS"]["PRESSURE"]["PSI"] | false;
    unit.celsius = doc["UNITS"]["TEMPERATURE"]["CELSIUS"] | false;
    unit.fahrenheit = doc["UNITS"]["TEMPERATURE"]["FAHRENHEIT"] | false;

    sensor.ens160 = doc["SENSORS"]["ENS160"] | false;
    sensor.lps22 = doc["SENSORS"]["LPS22"] | false;
    sensor.ltr390 = doc["SENSORS"]["LTR390"] | false;
    sensor.max1704 = doc["SENSORS"]["MAX17048"] | false;
    sensor.scd40 = doc["SENSORS"]["SCD40"] | false;
    sensor.sht41 = doc["SENSORS"]["SHT41"] | false;
    sensor.veml7700 = doc["SENSORS"]["VEML7700"] | false;

    file.close();

    Serial.println(config.studentName);
    Serial.println(config.refreshRate);
    Serial.println(config.wifiSSID);
    Serial.println(config.wifiPASS);
  }
  else{
    Serial.print("\nCould not open file");
  }
}

void loop() {
  
}
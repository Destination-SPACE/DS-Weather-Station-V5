/*******************************************************************************
DESTINATION WEATHER FEATHER WING EXAMPLE - DEMO

This is an example sketch for the Destination Weather FeatherWing remote sensing
platform to test the microSD card reader.

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

void setup() {
    //Setup Digitial IO
    pinMode(BUTTON0, INPUT_PULLUP);
    pinMode(BUTTON1, INPUT_PULLDOWN);
    pinMode(BUTTON2, INPUT_PULLDOWN);
    pinMode(LED, OUTPUT);

    //Initialize core loops
    //xTaskCreatePinnedToCore(loop0, "Task0", 1000, NULL, 0, &Task0, 0); // Task Function, Name of Task, Stack Size of Task, Parameter of the Task, Priority of the Task, Task Handle, Core Number
    //xTaskCreatePinnedToCore(loop1, "Task0", 1000, NULL, 0, &Task1, 0); // Task Function, Name of Task, Stack Size of Task, Parameter of the Task, Priority of the Task, Task Handle, Core Number

    PIXEL_FEATHER.begin(); // Initialize built-in NeoPixel
    PIXEL_WING.begin(); // Initialize FeatherWing NeoPixel

    //USB_MSC.setID("DSPACE", "SD Card", "1.0");
    //USB_MSC.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
    //USB_MSC.setUnitReady(false);
    //USB_MSC.begin();

    Serial.begin(115200);
    Wire.begin();

    if(SD_CD == HIGH){
    Serial.print("\n\033[48;5;1mSD Card not Inserted\033[0m");
        while(SD_CD == HIGH){
            delay(10);
        }
    }

    Serial.print("\nInitializing SD card ... ");

    /*if(!SD.begin(SD_CS, SD_SCK_MHZ(50))){
    Serial.print("\n\033[48;5;1mInitialization Failed\033[0m");
        while(1){
            delay(10);
        }
    }*/
    
    //Serial.print("\n\nImporting config.yaml...");
    //const char* YAML_CONTENT = READ_YAML("/config.yaml");
    //Serial.print("\n\nYAML File Imported!\nDeserializing YAML Content...");
    //DESERIALIZE_YAML_TO_JSON_OBJECT(YAML_CONTENT);
    //Serial.print("\n\nYAML File Deserialized!");

    Serial.print("\n\nInitializing Sensors...");
    INITIALIZE_SENSORS(SENSOR_ENS160, SENSOR_GUVA_B, SENSOR_LPS22, SENSOR_LTR390, SENSOR_MAX17048, SENSOR_SCD40, SENSOR_SHT41, SENSOR_VEML7700, REFRESH_RATE);
    Serial.print("\nSensor Initialization Complete!");

    Serial.print("\n\nInitializing TFT Display...");
    INITIALIZE_TFT();
    Serial.print("\n\nTFT Initialization Complete!");
    Serial.print("\n\n+==============================================================================+\n|  TIME  | TEMP | HUM |  HI  | PRES | ALT | CO2 | TVOC |  AQI  |  UVI  |  LUX  |\n|hh:mm:ss| (°C) | (%) | (°C) | hPa. | (m) |(ppm)|(ppb.)|(0-300)|(0-+11)|(k-lux)|\n+==============================================================================+");
}

/*void loop0(void * parameter){
    for(;;){
      int time_prev = millis();
      ENS160_AQI, ENS160_eCO2, ENS160_TVOC, LPS22_ALTITUDE, LPS22_PRESSURE, LPS22_TEMPERATURE, LTR390_RAW_UV, LTR390_UVI, MAX17048_CHARGE_RATE, MAX17048_PERCENTAGE, MAX17048_VOLTAGE, SCD40_CO2, SCD40_HUMIDITY, SCD40_TEMPERATURE, SHT41_ABSOLUTE_HUMIDITY, SHT41_HEAT_INDEX,SHT41_HUMIDITY, SHT41_TEMPERATURE, VEML7700_LUX = GET_SENSOR_DATA(SENSOR_ENS160, SENSOR_GUVA_B, SENSOR_LPS22, SENSOR_LTR390, SENSOR_MAX17048, SENSOR_SCD40, SENSOR_SHT41, SENSOR_VEML7700);
      
      char buffer[1024];

      sprintf(buffer,"\n|20:12:12| %4.1f |%5.2f|%6.2f| %4.0f |%5.1f| %3.0f | %4.0f | %5.1f | %5.2f |%7.3f|", SHT41_TEMPERATURE, SHT41_HUMIDITY, LPS22_PRESSURE, LPS22_ALTITUDE, SCD40_CO2, ENS160_TVOC, ENS160_AQI, LTR390_UVI, VEML7700_LUX);

      Serial.print(buffer);

      while(true){
        if(millis() - time_prev >= 1000) break;
        delay(5);
      }
      
    }
}

void loop1(void * parameter){
    for(;;){

    }
}*/

void loop() {
  int time_prev = millis();
  ENS160_AQI, ENS160_eCO2, ENS160_TVOC, LPS22_ALTITUDE, LPS22_PRESSURE, LPS22_TEMPERATURE, LTR390_RAW_UV, LTR390_UVI, MAX17048_CHARGE_RATE, MAX17048_PERCENTAGE, MAX17048_VOLTAGE, SCD40_CO2, SCD40_HUMIDITY, SCD40_TEMPERATURE, SHT41_ABSOLUTE_HUMIDITY, SHT41_HEAT_INDEX,SHT41_HUMIDITY, SHT41_TEMPERATURE, VEML7700_LUX = GET_SENSOR_DATA(SENSOR_ENS160, SENSOR_GUVA_B, SENSOR_LPS22, SENSOR_LTR390, SENSOR_MAX17048, SENSOR_SCD40, SENSOR_SHT41, SENSOR_VEML7700);
      
  char buffer[1024];

  sprintf(buffer,"\n|20:12:12| %4.1f |%5.2f|%6.2f| %4.0f |%5.1f| %3.0f | %4.0f | %5.1f | %5.2f |%7.3f|", SHT41_TEMPERATURE, SHT41_HUMIDITY, LPS22_PRESSURE, LPS22_ALTITUDE, SCD40_CO2, ENS160_TVOC, ENS160_AQI, LTR390_UVI, VEML7700_LUX);

  Serial.print(buffer);

  while(true){
    if(millis() - time_prev >= 1000) break;
    delay(5);
  }
}

/*const char* READ_YAML(const char* filename){
    if(!file.open(filename)){
        Serial.print("\nError opening file");

        return "";
    }

    size_t size = file.fileSize();
    char* buffer = (char*)malloc(size + 1);

    if(!buffer){
        Serial.print("\nMemory allocation failed");
        file.close();

        return "";
    }

    file.read(buffer, size);
    buffer[size] = '\0';
    file.close();

    return buffer;

}

void DESERIALIZE_YAML_TO_JSON_OBJECT(const char* YAML_CONTENT){
    StaticJsonDocument<2048> JSON_DOC;
    JsonObject JSON_OBJECT = JSON_DOC.to<JsonObject>();
    auto ERROR = deserializeYml(JSON_OBJECT, YAML_CONTENT);
    if(ERROR){
        Serial.printf("Unable to deserialize YAML to JsonObject: %s", ERROR.c_str() );

        return;
    }
    
    STUDENT_NAME = JSON_OBJECT["STUDENT_NAME"].as<const char*>();
    REFRESH_RATE = JSON_OBJECT["REFRESH_RATE"].as<int>();

    SSID = JSON_OBJECT["NETWORK"]["SSID"].as<const char*>();
    PASSWORD = JSON_OBJECT["NETWORK"]["PASSWORD"].as<const char*>();

    NEO_PIXEL_RGB = JSON_OBJECT["NEO_PIXEL"]["RGB"].as<bool>();
    NEO_RED = JSON_OBJECT["NEO_PIXEL"]["RGB"]["RED"].as<int>();
    NEO_GREEN = JSON_OBJECT["NEO_PIXEL"]["RGB"]["GREEN"].as<int>();
    NEO_BLUE = JSON_OBJECT["NEO_PIXEL"]["RGB"]["BLUE"].as<int>();
    NEO_BRIGHTNESS = JSON_OBJECT["NEO_PIXEL"]["RGB"]["BRIGHTNESS"].as<int>();
    NEO_PIXEL_HSV = JSON_OBJECT["NEO_PIXEL"]["HSV"].as<bool>();
    NEO_HUE = JSON_OBJECT["NEO_PIXEL"]["HSV"]["HUE"].as<int>();
    NEO_SATURATION = JSON_OBJECT["NEO_PIXEL"]["HSV"]["SATURATION"].as<int>();
    NEO_VALUE = JSON_OBJECT["NEO_PIXEL"]["HSV"]["VALUE"].as<int>();

    UNITS_FEET = JSON_OBJECT["UNITS"]["ALTITUDE"]["FEET"].as<bool>();
    UNITS_METERS = JSON_OBJECT["UNITS"]["ALTITUDE"]["METERS"].as<bool>();
    UNITS_PASCAL = JSON_OBJECT["UNITS"]["PRESSURE"]["PASCAL"].as<bool>();
    UNITS_MBAR = JSON_OBJECT["UNITS"]["PRESSURE"]["MBAR"].as<bool>();
    UNITS_K_PASCAL = JSON_OBJECT["UNITS"]["PRESSURE"]["K_PASCAL"].as<bool>();
    UNITS_IN_HG = JSON_OBJECT["UNITS"]["PRESSURE"]["IN_HG"].as<bool>();
    UNITS_MM_HG = JSON_OBJECT["UNITS"]["PRESSURE"]["MM_HG"].as<bool>();
    UNITS_PSI = JSON_OBJECT["UNITS"]["PRESSURE"]["PSI"].as<bool>();
    UNITS_CELCIUS = JSON_OBJECT["UNITS"]["TEMPERATURE"]["CELCIUS"].as<bool>();
    UNITS_FAHRENHEIT = JSON_OBJECT["UNITS"]["TEMPERATURE"]["FAHRENHEIT"].as<bool>();

    SENSOR_ENS160 = JSON_OBJECT["SENSORS"]["ENS160"].as<bool>();
    SENSOR_GUVA_B = JSON_OBJECT["SENSORS"]["GUVA_B"].as<bool>();
    SENSOR_LPS22 = JSON_OBJECT["SENSORS"]["LPS22"].as<bool>();
    SENSOR_LTR390 = JSON_OBJECT["SENSORS"]["LTR390"].as<bool>();
    SENSOR_MAX17048 = JSON_OBJECT["SENSORS"]["MAX17048"].as<bool>();
    SENSOR_SCD40 = JSON_OBJECT["SENSORS"]["SCD40"].as<bool>();
    SENSOR_SHT41 = JSON_OBJECT["SENSORS"]["SHT41"].as<bool>();
    SENSOR_VEML7700 = JSON_OBJECT["SENSORS"]["VEML7700"].as<bool>();
}

int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize){
  bool rc;

  #if SD_FAT_VERSION >= 20000
    rc = SD.card()->readSectors(lba, (uint8_t*) buffer, bufsize/512);
  #else
    rc = SD.card()->readBlocks(lba, (uint8_t*) buffer, bufsize/512);
  #endif

  return rc ? bufsize : -1;
}

int32_t msc_write_cb (uint32_t lba, uint8_t* buffer, uint32_t bufsize){
  bool rc;

  digitalWrite(LED_BUILTIN, HIGH);

  #if SD_FAT_VERSION >= 20000
    rc = SD.card()->writeSectors(lba, buffer, bufsize/512);
  #else
    rc = SD.card()->writeBlocks(lba, buffer, bufsize/512);
  #endif

  return rc ? bufsize : -1;
}

void msc_flush_cb (void){
  #if SD_FAT_VERSION >= 20000
    SD.card()->syncDevice();
  #else
    sd.card()->syncBlocks();
  #endif

  SD.cacheClear();

  FS_CHANGED = true;

  digitalWrite(LED_BUILTIN, LOW);
}*/
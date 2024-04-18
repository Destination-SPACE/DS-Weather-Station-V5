/*******************************************************************************
DESTINATION WEATHER FEATHER WING EXAMPLE - DEMO

This is an example sketch for the Destination Weather FeatherWing remote sensing
platform to test the microSD card reader.

modified 2024-03-12
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
#include "setup.h"

TaskHandle_t Task0;
TaskHandle_t Task1;

SdFat SD;
SdFile file;

int hh, mm, ss;
int clock_timer = 0;
char fileName[13] = FILE_BASE_NAME "00.csv";

//Declare system components
Adafruit_NeoPixel PIXEL_FEATHER(1, NEO_PIXEL_FEATHER);
Adafruit_NeoPixel PIXEL_WING(1, NEO_PIXEL_WING, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.print("Destination Weather Station v5!");

  //Setup Digital IO
  Serial.print("\n\nSetting up digital IO...");
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLDOWN);
  pinMode(BUTTON2, INPUT_PULLDOWN);
  pinMode(LED, OUTPUT);
  pinMode(LPS22_INT, INPUT);
  pinMode(SD_CD, INPUT);
  pinMode(NEO_PIXEL_FEATHER, OUTPUT);
  pinMode(NEO_PIXEL_WING, OUTPUT);
  Serial.print("\nDigital IO setup complete!");

  //Initialize core loops
  Serial.print("\nInitializing cores...");
  xTaskCreatePinnedToCore(loop0, "Task0", 10000, NULL, 0, &Task0, 0);
  xTaskCreatePinnedToCore(loop1, "Task1", 1000, NULL, 0, &Task1, 1);
  Serial.print("\nCores initialized!");

  Serial.print("\nInitializing NeoPixels...");
  PIXEL_FEATHER.begin(); // Initialize built-in NeoPixel
  PIXEL_WING.begin(); // Initialize FeatherWing NeoPixel
  Serial.print("\nNeoPixels initialized!");
  
  Serial.print("\nI2C bus...");
  Wire.begin();
  Serial.print("\nI2C bus online!");

  Serial.print("\nInitializing TFT display...");
  initializeTFT();
  Serial.print("\nTFT display initialized!");

  Serial.print("\nChecking for SD card...");
  const uint8_t baseNameSize = sizeof(FILE_BASE_NAME) - 1;
  if(digitalRead(SD_CD) == LOW){
    Serial.print("\nPlease insert microSD card.");
    while(digitalRead(SD_CD) == LOW){
      delay(500);
    }
  }
  else{
    Serial.print("\nSD card detected!");
  }

  Serial.print("\nInitializing SD card...");
  if(!SD.begin(SD_CS, SD_SCK_MHZ(50))){
    Serial.print("\nSD card not responding.");
    while(true){
      if(SD.begin(SD_CS, SD_SCK_MHZ(50))){
        break;
      }
      delay(100);
    }
  }
  Serial.print("\nSD card initialized!");

  Serial.print("\nCreating CSV file...");
  if(baseNameSize > 6){
    Serial.print("\n\nFile base name too long.");
  }
  while(SD.exists(fileName)){
    if(fileName[baseNameSize + 1] != '9'){
      fileName[baseNameSize + 1]++;
    }
    else if(fileName[baseNameSize] != '9'){
      fileName[baseNameSize + 1] = '0';
      fileName[baseNameSize]++;
    }
    else{
      Serial.print("\n\nCan't create file name.");
    }
  }
  if(file.open(fileName, O_WRONLY | O_CREAT | O_EXCL)){
    file.println(F("Time (s),Temp (C),Hum (%),HI (C), Dew Point (C), PRES (hPa),ALT (m),CO2 (ppm), eCO2 (ppm), TVOC (ppb),AQI (1-5), RAW UV, UVI (0-11),LUX (k-lux)\n"));
    Serial.print("\nCSV file created!");
  }
  else{
    Serial.print("\n\nError opening file.");
  }

  Serial.print("\nImporting config.yaml...");
  deserializeYAML();
  Serial.print("\nYAML file deserialized!");

  config = getConfig();
  Serial.print("\nConfigurations imported!");

  Serial.print("\nConfiguring settings...");
  unit = getUnits();
  sen = getSensors();

  Serial.print("\nInitializing sensors...");
  sen = initializeSensors(sen);

  Serial.print("\n\nInitialization complete!");

  Serial.print("\n\n+==============================================================================+\n|  TIME  | TEMP | HUM |  HI  | PRES | ALT | CO2 | TVOC |  AQI  |  UVI  |  LUX  |\n|hh:mm:ss| (°C) | (%) | (°C) | hPa  | (m) |(ppm)|(ppb.)| (1-5) |(0-+11)|(k-lux)|\n+==============================================================================+");

}

void loop0(void * parameter){
  for(;;){
    int time_prev = millis();
    param = getSensorData(sen);

    //Clock timer
    ss = (millis() - clock_timer)/1000; // Set seconds to how much time has elapsed since first data read
    if(ss >= 60){ // If seconds is 60+, add 1 minutes and reset seconds variable
      clock_timer = millis();
      ss = ss - 60;
      mm = mm + 1;
      if(mm >= 60){ // If minutes is 60+, add 1 hours and reset minutes and seconds
        ss = ss - 60;
        mm = mm - 60;
        hh = hh + 1;
      }
    }
    
    char buffer[1024];

    sprintf(buffer,"\n|%02d:%02d:%02d| %4.1f |%5.2f|%6.2f| %4.0f |%5.1f| %4.0f| %4.0f | %5.1f | %5.2f |%7.3f|", hh, mm, ss, param.tempSHT, param.humdSHT, param.heatIndex, param.pres, param.alt, param.CO2, param.tvoc, param.aqi, param.uvi, param.alsLTR);

    Serial.print(buffer);

    char fileBuffer[1024];

    sprintf(fileBuffer,"%02d:%02d:%02d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", hh, mm, ss, param.tempSHT, param.humdSHT, param.heatIndex, param.dewPoint, param.pres, param.alt, param.CO2, param.eCO2, param.tvoc, param.aqi, param.uvRaw, param.uvi, param.alsVEML);

    while(true){
      if(millis() - time_prev >= config.refreshRate) break;
      delay(5);
    }
  }
}

void loop1(void * parameter){
    for(;;){

    }
}

void loop() {
  
}
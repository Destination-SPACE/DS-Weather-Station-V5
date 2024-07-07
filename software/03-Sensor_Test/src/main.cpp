/*******************************************************************************
DESTINATION WEATHER FEATHER WING EXAMPLE - SENSOR TEST

This is an example sketch for the Destination Weather FeatherWing remote sensing
platform to test the microSD card reader.

modified 2024-05-13
by Madison Gleydura

MIT LICENSE AGREEMENT

Copyright © 2024 Destination SPACE Inc.

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

int refreshRate = 500; // Milliseconds
int clockTimer, hh, mm, ss;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Wire.begin();
  
  pinMode(SD_CD, INPUT);

  if(!digitalRead(SD_CD)){
    Serial.print("\nPlease insert SD card...");
    while(true);
  }

  if(!SD.begin(SD_CS, SD_SCK_MHZ(50))){
    Serial.print("\nSD card initialization failed...");
  }
  else{
    Serial.print("\nSD card initialization complete!");
  }

  sen = getSensors();
  unit = getUnits();

  Serial.print("\n\nInitializing Sensors...");
  sen = INITIALIZE_SENSORS(sen, refreshRate);
  Serial.print("\nSensor Initialization Complete!");

  char buffer[1024];
  sprintf(buffer,"\n\n+==============================================================================+\n|  TIME  | TEMP | HUM |  HI  | PRES | ALT | CO2 | TVOC |  AQI  |  UVI  |  LUX  |\n|hh:mm:ss| %s |(pct)| %s |%s|%s|(ppm)|(ppb.)| (1-5) |(0-+11)|(k-lux)|\n+==============================================================================+", temp, temp, pres, alt); // Print header to terminal
  Serial.print(buffer);

  clockTimer = 0;
  hh = 0;
  mm = 0;
  ss = 0;
}

void loop() {
  int previousTime = millis(); // Reset the clock
  param = GET_SENSOR_DATA(sen); // Get sensor data (see sensors.cpp)

  //Clock timer
  ss = (millis() - clockTimer)/1000; // Set seconds to how much time has elapsed since first data read
  if(ss >= 60){ // If seconds is 60+, add 1 minutes and reset seconds variable
    clockTimer = millis();
    ss = ss - 60;
    mm = mm + 1;
    if(mm >= 60){ // If minutes is 60+, add 1 hours and reset minutes and seconds
      ss = ss - 60;
      mm = mm - 60;
      hh = hh + 1;
    }
  }
      
  char buffer[1024]; // Create a 1024 bit character buffer

  sprintf(buffer,"\n|%02d:%02d:%02d| %4.1f |%5.2f| %4.1f | %4.0f |%5.1f| %4.0f| %4.0f |   %1.0f   | %5.2f |%6.2f|", hh, mm, ss, param.tempSHT, param.humdSHT, param.heatIndex, param.pres, param.alt, param.CO2, param.tvoc, param.aqi, param.uviLTR, param.alsVEML); // Add sensor data to the buffer

  Serial.print(buffer); // Print sensor data

  while(true){
    if(millis() - previousTime >= refreshRate){ // Check if the delay period has elapsed
      break;
    }
    delay(5);
  }
}
/*******************************************************************************
DESTINATION WEATHER FEATHER WING EXAMPLE - SENSOR TEST

This is an example sketch for the Destination Weather FeatherWing remote sensing
platform to test the microSD card reader.

modified 2024-03-17
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

int REFRESH_RATE = 0.5; //Seconds
//units unit;
//sensors sen;
//parameters param;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Wire.begin();

  sen = getSensors();

  Serial.print("\n\nInitializing Sensors...");
  sen = INITIALIZE_SENSORS(sen, REFRESH_RATE);
  Serial.print("\nSensor Initialization Complete!");

  Serial.print("\n\n+==============================================================================+\n|  TIME  | TEMP | HUM |  HI  | PRES | ALT | CO2 | TVOC |  AQI  |  UVI  |  LUX  |\n|hh:mm:ss| (°C) | (%) | (°C) | hPa. | (m) |(ppm)|(ppb.)|(0-300)|(0-+11)|(k-lux)|\n+==============================================================================+");
}

void loop() {
  int time_prev = millis();
  param = GET_SENSOR_DATA(sen);
      
  char buffer[1024];

  sprintf(buffer,"\n|20:12:12| %4.1f |%5.2f|%6.2f| %4.0f |%5.1f| %4.0f| %4.0f | %5.1f | %5.2f |%7.3f|", param.tempSHT, param.humdSHT, param.heatIndex, param.pres, param.alt, param.CO2, param.tvoc, param.aqi, param.uviLTR, param.alsLTR);

  Serial.print(buffer);

  while(true){
    if(millis() - time_prev >= REFRESH_RATE) break;
    delay(5);
  }
}
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
#include <Wire.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SHT4x.h>

Adafruit_LPS22 LPS22;
Adafruit_SHT4x SHT41 = Adafruit_SHT4x();

sensor_id

void setup() {
  Serial.print(115200);
  
  while(!Serial) {
    delay(10);
  }

  char buffer[256];
  sprintf(buffer, "\n\n+=================================================+\n|  Destination Weather FeatherWing Sensor Test    |\n+=================================================+");
  Serial.print(buffer);

  LPS22.begin_I2C(LPS2X_I2CADDR_DEFAULT)

  if(!LPS22.begin_I2C() && !SHT41.begin()) {
    Serial.print("\n\033[48;5;1mCouldn't find LPS22\033[0m");
    Serial.print("\n\033[48;5;1mCouldn't find SHT41\033[0m");
    while(1){
      delay(10);
    }
  }
  else if(!LPS22.begin_I2C() && SHT41.begin()){
    Serial.print("\n\033[48;5;1mCouldn't find LPS22\033[0m");
    while(1){
      delay(10);
    }
  }
  else if(LPS22.begin_I2C() && !SHT41.begin()){
    Serial.print("\n\033[48;5;1mCouldn't find SHT41\033[0m");
    while(1){
      delay(10);
    }
  }
  else{
    Serial.print("\nFound LPS22");
    Serial.print("\nFound SHT41");
  }

  LPS22.setDataRate(LPS22_RATE_10_HZ);
  SHT41.setPrecision(SHT4X_HIGH_PRECISION);
  SHT41.setHeater(SHT4X_NO_HEATER);
  
  sprintf(buffer, "\n\n+=================================================+\n| Temperature (C) | Pressure (hPa) | Humidity (%)  |\n+-----------------+----------------+--------------+");
  Serial.print(buffer);
}

void loop() {
  sensors_event_t humidity, pressure, temp1, temp2;
  
  SHT41.getEvent(&humidity, &temp1);
  LPS22.getEvent(&pressure, &temp2);

  char buffer[64];
  sprintf(buffer, "\n|     %6.2f      |    %8.3f    |     %4.1f     |", temp1.temperature, pressure.pressure, humidity.relative_humidity);
  Serial.print(buffer);

  delay(500);
}
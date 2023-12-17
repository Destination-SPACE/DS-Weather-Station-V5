#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SHT4x.h>

Adafruit_LPS22 LPS22;
Adafruit_SHT4x SHT41 = Adafruit_SHT4x();

void setup() {
  Serial.print(115200);
  
  while(!Serial) {
    delay(10);
  }

  char buffer[256];
  sprintf(buffer, "\n\n+=================================================+\n|  Destination Weather FeatherWing Sensor Test  +\n+=================================================+");
  Serial.print(buffer);

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
  
  sprintf(buffer, "\n\n+=================================================+\n| Temperature (C) | Pressure (hPa) | Humidity (%) |\n+-----------------+----------------+--------------+");
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
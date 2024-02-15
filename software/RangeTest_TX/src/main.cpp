#include <Arduino.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Adafruit_SHT4x.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define LED 

Adafruit_SHT4x SHT41 = Adafruit_SHT4x();
Adafruit_LPS22 LPS22;

const char* ssid = "DS-WX-STN-1";
const char* password = "password";


AsyncWebServer server(80);

std::string readTemp(){
  sensors_event_t humidity, temp;
  SHT41.getEvent(&humidity, &temp);

  return std::to_string(temp.temperature);
}

std::string readHumidity(){
  sensors_event_t humidity, temp;
  SHT41.getEvent(&humidity, &temp);

  return std::to_string(humidity.relative_humidity);
}

std::string readPressure(){
  sensors_event_t temp, pressure;
  LPS22.getEvent(&pressure, &temp);

  return std::to_string(pressure.pressure);
}

void setup() {
  Serial.begin(115200);

  if(!SHT41.begin()){
    Serial.print("\nSHT41 not found...");
    while(true){
      delay(10);
    }
  }
  if(!LPS22.begin_I2C()){
    Serial.print("\nLPS22 not found...");
    while(true){
      delay(10);
    }
  }

  SHT41.setPrecision(SHT4X_HIGH_PRECISION);
  SHT41.setHeater(SHT4X_NO_HEATER);
  LPS22.setDataRate(LPS22_RATE_10_HZ);

  Serial.print("Setting Access Point...");
  
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();

  Serial.print("\nAP IP:\t");
  Serial.print(IP);

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "test/plain", readTemp().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "test/plain", readHumidity().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "test/plain", readPressure().c_str());
  });

  server.begin();  
}

void loop() {
  
}

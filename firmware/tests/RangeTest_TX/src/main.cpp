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

const char* ssid = "DS-WX-STN-01";
const char* password = "password";

uint32_t tempData, humdData, presData;
uint32_t dataTemp, dataHumd, dataPres;

AsyncWebServer server(80);

uint32_t getTemperature(){
  sensors_event_t shtHumidity, shtTemperature;
  SHT41.getEvent(&shtHumidity, &shtTemperature);
  tempData = shtTemperature.temperature;

  return tempData;
}

uint32_t getHumidity(){
  sensors_event_t shtHumidity, shtTemperature;
  SHT41.getEvent(&shtHumidity, &shtTemperature);
  humdData = shtHumidity.relative_humidity;

  return humdData;
}


uint32_t getPressure(){
  sensors_event_t lpsPressure, lpsTemperature;
  LPS22.getEvent(&lpsPressure, &lpsTemperature);
  presData = lpsPressure.pressure;

  return presData;
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
    dataTemp = getTemperature();
    char tempSend[16];
    dtostrf(dataTemp, 4, 2, tempSend);

    request->send_P(200, "test/plain", tempSend);
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    dataHumd = getHumidity();
    char humdSend[16];
    dtostrf(dataHumd, 4, 2, humdSend);
    request->send_P(200, "test/plain", humdSend);
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    dataPres = getPressure();
    char presSend[16];
    dtostrf(dataPres, 4, 2, presSend);
    request->send_P(200, "test/plain", presSend);
  });

  server.begin();
}

void loop() {
  
}

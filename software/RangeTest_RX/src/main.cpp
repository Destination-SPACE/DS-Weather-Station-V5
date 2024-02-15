#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "DS-WX-STN-01";
const char* password = "password";

const char* serverTemp = "http://192.168.4.1/temperature";
const char* serverHumd = "http://192.168.4.1/humidity";
const char* serverPres = "http://192.168.4.1/pressure";

std::string temperature, humidity, pressure;
std::string httpGetRequest(const char*);

unsigned long previousMillis = 0;
const long interval = 5000;


void setup() {
  Serial.begin(115200);
  while(!Serial){
    delay(10);
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n\nCOnnected to WiFi network with IP Address: ");
  Serial.print(WiFi.localIP());
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    if(WiFi.status() == WL_CONNECTED){
      temperature = httpGetRequest(serverTemp);
      humidity = httpGetRequest(serverHumd);
      pressure = httpGetRequest(serverPres);
      
      char buffer[256];
      sprintf(buffer, "\nTemperature:\t%s*C\nHumidity:\t%s%\nPressure:\t%shPa\n", temperature, humidity, pressure);
      Serial.print(buffer);

      previousMillis = currentMillis;
    }
    else{
      Serial.print("\nWiFi Disconnected");
    }
  }
}

std::string httpGetRequest(const char* serverName) {
  HTTPClient http;
  http.begin(serverName);
  int httpResponseCode = http.GET();

  std::string payload = "--";

  if(httpResponseCode > 0){
    Serial.print("\nHTTP Response Code: ");
    Serial.print(httpResponseCode);
    payload = std::string(http.getString().c_str());
  }
  else{
    Serial.print("\nError Code: ");
    Serial.print(httpResponseCode);
  }

  http.end();

  return payload;
}
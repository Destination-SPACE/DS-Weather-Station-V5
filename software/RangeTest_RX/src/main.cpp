#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "DS-WX-STN-01";
const char* password = "password";

const char* temperature_server = "http://192.168.4.1/temperature";
const char* humidity_server = "http://192.168.4.1/humidity";
const char* pressure_server = "http://192.168.4.1/pressure";

unsigned long previousMillis = 0;
const long interval = 5000;

String temperature;
String humidity;
String pressure;

String HTTP_Request(const char*);

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
  unsigned long current_time = millis();
  if(current_time - previousMillis >= interval) {
    if(WiFi.status()== WL_CONNECTED ){
      int rssi = WiFi.RSSI();
      temperature = HTTP_Request(temperature_server);
      humidity = HTTP_Request(humidity_server);
      pressure = HTTP_Request(pressure_server);
      char buffer[128];
      sprintf(buffer, "\nTemperature:\t%s*C\nHumidity:\t%s%\nPressure:\t%shPa\nRSSI:\t\t%ddBm\n", temperature, humidity, pressure);
      Serial.print(buffer);
      previousMillis = current_time;
    }
    else{
      Serial.println("WiFi got disconnected!");
    }
  }
}

String HTTP_Request(const char* server_name) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, server_name);
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}
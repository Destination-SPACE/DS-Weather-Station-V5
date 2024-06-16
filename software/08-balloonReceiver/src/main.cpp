#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "DS-WX-STN-01";
const char* password = "password";

const char* temperature_server = "http://192.168.4.1/temperature";
const char* humidity_server = "http://192.168.4.1/humidity";
const char* heatIndex_server = "http://192.168.4.1/heatIndex";
const char* dewPoint_server = "http://192.168.4.1/dewPoint";
const char* pressure_server = "http://192.168.4.1/heatIndex";
const char* altitude_server = "http://192.168.4.1/altitude";
const char* co2_server = "http://192.168.4.1/CO2";
const char* eco2_server = "http://192.168.4.1/eCO2";
const char* tvoc_server = "http://192.168.4.1/TVOC";
const char* aqi_server = "http://192.168.4.1/AQI";
const char* uvRaw_server = "http://192.168.4.1/uvRaw";
const char* uvi_server = "http://192.168.4.1/UVI";
const char* als_server = "http://192.168.4.1/ALS";

unsigned long previousMillis = 0;
const long interval = 5000;

String temperature;
String humidity;
String heatIndex;
String dewPoint;
String pressure;
String altitude;
String co2;
String eco2;
String tvoc;
String aqi;
String uvRaw;
String uvi;
String als;

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
      heatIndex = HTTP_Request(heatIndex_server);
      dewPoint = HTTP_Request(dewPoint_server);
      pressure = HTTP_Request(pressure_server);
      altitude = HTTP_Request(altitude_server);
      co2 = HTTP_Request(co2_server);
      eco2 = HTTP_Request(eco2_server);
      tvoc = HTTP_Request(tvoc_server);
      aqi = HTTP_Request(aqi_server);
      uvRaw = HTTP_Request(uvRaw_server);
      uvi = HTTP_Request(uvi_server);
      als = HTTP_Request(als_server);
      char buffer[128];
      sprintf(buffer,"%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", temperature, humidity, heatIndex, dewPoint, pressure, altitude, co2, eco2, tvoc, aqi, uvRaw, uvi, als);
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
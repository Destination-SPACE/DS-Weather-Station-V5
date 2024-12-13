#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

Adafruit_SHT4x SHT41 = Adafruit_SHT4x();

sensors_event_t SHT41_HUMIDITY_SEN;
sensors_event_t SHT41_TEMPERATURE_SEN;

// Replace with your network credentials
const char *ssid = "ESP32-Weather-AP";
const char *password = "password";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  while(!Serial){
    delay(10);
  }
  Wire.begin();

  if(!SHT41.begin()){
    Serial.println("Sensor not found!");
  }

  SHT41.setPrecision(SHT4X_HIGH_PRECISION);
  SHT41.setHeater(SHT4X_NO_HEATER);

  // Start WiFi in AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Print the AP IP address
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  // Check if a client is available
  WiFiClient client = server.available();

  SHT41.getEvent(&SHT41_HUMIDITY_SEN, &SHT41_TEMPERATURE_SEN);

  float humidity = SHT41_HUMIDITY_SEN.relative_humidity;
  float temperature = SHT41_TEMPERATURE_SEN.temperature;

  if (client) {
    Serial.println("New client connected");

    // Send weather sensor data
    char buffer[64];
    sprintf(buffer, "Temperature: %0.2f, Humidity: %0.2f", temperature, humidity);
    client.println(buffer);
    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
  }

  delay(1000); // Delay before checking for new clients again
}
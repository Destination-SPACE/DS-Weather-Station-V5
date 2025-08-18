#include <Arduino.h>
#include <WiFi.h>
#include <string.h>

// Replace with your network credentials
//const char *ssid = "ESP32-Weather-AP";
//const char *password = "password";

String ssidStr;
String passwordStr;

void setup() {
  Serial.begin(115200);
  while(!Serial){
    delay(10);
  }

  String ssidStr;
  String passwordStr;

  Serial.print("\nEnter WiFi SSID: ");
  while(true){
    if(Serial.available() > 0){
        ssidStr = Serial.readString();
        break;
    }
  }

  Serial.print("\nEnter WiFi password: ");
  while(true){
    if(Serial.available() > 0){
        passwordStr = Serial.readString();
        break;
    }
  }

  const char *ssid = ssidStr.c_str();
  const char *password = passwordStr.c_str();

  // Connect to WiFi network
  WiFi.begin(ssid, password);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Replace with the IP address of the ESP32 AP
  const char *host = "192.168.4.1";

  // Establish a connection with the server
  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("Connection failed");
    return;
  }

  // Send HTTP request
  client.print("GET / HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(host);
  client.print("\r\n");
  client.print("Connection: close\r\n\r\n");

  // Wait for response
  while (client.connected() || client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

  // Close the connection
  client.stop();

  delay(5000); // Wait 5 seconds before the next request
}
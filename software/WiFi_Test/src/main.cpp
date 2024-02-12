#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "DS-WX-4843";
const char* password = "password";

String savedSSID;
String savedPassword;
bool saved = false;

WebServer server(80);

void handleRoot() {
  // Send HTML content as response
  server.send(200, "text/html",
    "<!DOCTYPE html>\
    <html>\
      <head>\
        <meta charset='UTF-8'>\
        <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
        <title>Wi-Fi Configuration</title>\
      </head>\
      <body>\
        <h1>Enter Wi-Fi Credentials</h1>\
        <form action='/save' method='post'>\
          <label for='ssid'>SSID:</label><br>\
          <input type='text' id='ssid' name='ssid'><br>\
          <label for='password'>Password:</label><br>\
          <input type='password' id='password' name='password'><br><br>\
          <input type='submit' value='Submit'>\
        </form>\
      </body>\
    </html>"
  );
}

void handleFormSubmission() {
  // Retrieve form parameters
  savedSSID = server.arg("ssid");
  savedPassword = server.arg("password");
  saved = true;

  // Do something with the credentials (e.g., save to EEPROM)
  // This is where you would save the credentials to NVM

  // Send response
  server.send(200, "text/plain", "Credentials saved.");
}

void setup() {
  Serial.begin(115200);

  while(!Serial){
    delay(10);
  }

  WiFi.softAP(ssid, password);

  IPAddress apIP = WiFi.softAPIP();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleFormSubmission);

  server.begin();
  Serial.println("HTTP server started");
  Serial.println(apIP);
}

void loop() {
  server.handleClient();

  if(saved){
    Serial.println(savedSSID);
    Serial.println(savedPassword);
    while(true){
      delay(10);
    }
  }
}
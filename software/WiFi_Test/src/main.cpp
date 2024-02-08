#include <Arduino.h>
#include <SPI.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <WebServer.h>

const char *esp_ssid = "DS-WX-046589";  // Weather Station's SSID
const char *esp_password = "password";  // Weather Station's Password

bool formSubmit;
String ssid, password;

void handleRoot();
String handleFormSubmission();

IPAddress ip(192,168,4,1);          // IP address
IPAddress subnet(255, 255, 255, 0); // Subnet mask

WebServer server(80);  // Port 80

void setup() {
  Serial.begin(115200);

  while(!Serial){
    delay(10);
  }
  

  if(!SPIFFS.begin(true)){
    Serial.println("An error occured while mounting SPIFFS");
    return;
  }

  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP(esp_ssid, esp_password);

  IPAddress apIP = WiFi.softAPIP();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleFormSubmission);
  server.begin();

  Serial.print("\n\nHTTP server started\n");
  Serial.print(apIP);
}

void loop() {
  server.handleClient();
  if(formSubmit){
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("PASS: ");
    Serial.print(password);
  }
}

void handleRoot(){
  File file = SPIFFS.open("/network-config.html","r");
  if(file){
    server.streamFile(file, "text/html");
    file.close();
  }
  else{
    server.send(404, "text/plain", "network-config.html not found");
  }
}

String handleFormSubmission(){
  ssid = server.arg("ssid");
  password = server.arg("password");

  formSubmit = true;

  server.send(200, "text/plain", "Credentials saved");

  return ssid, password;
}
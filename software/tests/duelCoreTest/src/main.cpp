#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  while(!Serial);
  delay(5000);
  Serial.print("Setup() running on core ");
  Serial.println(xPortGetCoreID());
}

void loop() {
  Serial.print("loop() running on core ");
  Serial.println(xPortGetCoreID());
  delay(1000);
}
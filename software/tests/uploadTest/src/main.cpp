#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  while(!Serial);
}

void loop() {
  Serial.print("\nHello World!");
  delay(1000);
}
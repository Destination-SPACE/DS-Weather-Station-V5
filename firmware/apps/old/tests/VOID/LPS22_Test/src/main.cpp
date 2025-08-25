#include <Arduino.h>
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while(!Serial) delay(10);
  
}

void loop() {
  Wire.beginTransmission(0x5D);
  Wire.write(0x0F);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(0x5D);
  Wire.requestFrom(0x5D,2);
  byte msb = Wire.read();
  byte lsb = Wire.read();
  Wire.endTransmission();
  uint16_t whoAmI = msb << 8 | lsb;
  Serial.println(whoAmI, HEX);

  delay(5000);
}
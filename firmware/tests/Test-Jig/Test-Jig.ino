#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define LED_GRN 24
#define LED_AMB 24
#define LED_RED 28

#define BOARD_DETECT 25
#define EEPROM_SCIO 6
#define NEO_PIXEL_WEATHER_WING 5
#define LPS_INT 4

Adafruit_LPS22 LPS22;
Adafruit_SHT4x SHT41 = Adafruit_SHT4x();
Adafruit_NeoPixel PIXEL_WEATHER_WING(1, NEO_PIXEL_WEATHER_WING, NEO_GRB + NEO_KHZ800);

void setup() {
  while(!Serial){
    delay(10);
  }
  Wire.begin();

  pinMode(LED_GRN, OUTPUT);
  pinMode(LED_AMB, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  PIXEL_WEATHER_WING.begin();

  if(!LPS22.begin_I2C()){
    Serial.print("\n\nLSP22 Not Found");
    digitalWrite(LED_RED, HIGH);
    while(true){
      delay(100);
    }
  }
  else{
    LPS22.setDataRate(LPS22_RATE_10_HZ);
  }
  if(!SHT41.begin()){
    Serial.print("\n\nSHT41 Not Found");
    digitalWrite(LED_RED, HIGH);
    while(true){
      delay(100);
    }
  }
  else{
    SHT41.setPrecision(SHT4X_HIGH_PRECISION);
    SHT41.setHeater(SHT4X_NO_HEATER);
  }
  Serial.print("\nPass");
  digitalWrite(LED_GRN, HIGH);
  static uint8_t hue = 0;
  while(true){
    PIXEL_WEATHER_WING.setPixelColor(0, PIXEL_WEATHER_WING.ColorHSV(hue, 255, 255));
    PIXEL_WEATHER_WING.show();
    delay(20);
    hue += 1;
  }
}

void loop() {
}

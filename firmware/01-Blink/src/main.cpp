#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NEO_PIXEL_DEFAULT PIN_NEOPIXEL
#define NEO_PIXEL_WEATHER_WING 10
#define DELAY_VALUE 500

Adafruit_NeoPixel PIXEL_BUILTIN(1, NEO_PIXEL_DEFAULT);
Adafruit_NeoPixel PIXEL_WEATHER_WING(1, NEO_PIXEL_WEATHER_WING, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  PIXEL_BUILTIN.begin();
  PIXEL_WEATHER_WING.begin();
}

void loop() {
  PIXEL_BUILTIN.clear();
  PIXEL_WEATHER_WING.clear();

  PIXEL_BUILTIN.show();
  PIXEL_WEATHER_WING.show();

  digitalWrite(LED_BUILTIN, LOW);

  delay(DELAY_VALUE);

  PIXEL_BUILTIN.setPixelColor(0, PIXEL_BUILTIN.Color(0, 150, 0));
  PIXEL_WEATHER_WING.setPixelColor(0, PIXEL_WEATHER_WING.Color(0, 150, 0));

  PIXEL_BUILTIN.show();
  PIXEL_WEATHER_WING.show();

  digitalWrite(LED_BUILTIN, HIGH);
  
  delay(DELAY_VALUE);
}
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NEO_PIXEL_DEFAULT PIN_NEOPIXEL
#define NEO_PIXEL_WEATHER_WING 10
#define DELAY_VALUE 500

int led = LED_BUILTIN;

Adafruit_NeoPixel PIXEL_BUILTIN(1, NEO_PIXEL_DEFAULT);
Adafruit_NeoPixel PIXEL_WEATHER_WING(1, NEO_PIXEL_WEATHER_WING, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(led, OUTPUT);
  PIXEL_BUILTIN.begin();
  PIXEL_WEATHER_WING.begin();
}

void loop() {
  PIXEL_BUILTIN.clear();
  PIXEL_WEATHER_WING.clear();
  
  PIXEL_BUILTIN.setPixelColor(0, PIXEL_BUILTIN.Color(0, 150, 0));
  PIXEL_WEATHER_WING.setPixelColor(0, PIXEL_WEATHER_WING.Color(0, 150, 0));

  digitalWrite(led, HIGH);
  PIXEL_BUILTIN.show();
  PIXEL_WEATHER_WING.show();
  
  delay(DELAY_VALUE);
}
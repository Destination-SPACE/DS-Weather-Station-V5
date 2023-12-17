/*******************************************************************************
DESTINATION WEATHER FEATHER WING EXAMPLE - DEMO

This is an example sketch for the Destination Weather FeatherWing remote sensing
platform to test the microSD card reader.

modified 2023-12-17
by Madison Gleydura

MIT LICENSE AGREEMENT

Copyright © 2023 Destination SPACE Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SdFat.h>
#include <SPI.h>
#include <Wire.h>
#include <YAMLDuino.h>

#define LED LED_BUILTIN
#define LPS22_INT 7
#define SD_CS 5
#define SD_CD 6
#define NEO_PIXEL_FEATHER PIN_NEOPIXEL
#define NEO_PIXEL_WING 10

Adafruit_LPS22 LPS22;
Adafruit_NeoPixel PIXEL_FEATHER(1, NEO_PIXEL_FEATHER);
Adafruit_NeoPixel PIXEL_WING(1, NEO_PIXEL_WING, NEO_GRB + NEO_KHZ800);
Adafruit_SHT4x SHT41 = Adafruit_SHT4x();
Adafruit_ST7789 TFT = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
Adafruit_USBD_MSC USB_MSC;

SdFat SD;
SdFile ROOT;
SdFile FILE;

bool FS_CHANGED;

void setup() {
  
}

void loop() {
  
}

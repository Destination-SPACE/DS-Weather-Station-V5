#ifndef _setup_h
#define _setup_h

//System libraries
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ST7789.h>
//#include <Adafruit_TinyUSB.h>
#include <ArduinoJson.h>
#include <SdFat.h>
#include <SPI.h>
#include <Wire.h>

//Sensor libraries
#include <Adafruit_LPS2X.h>
#include <Adafruit_SHT4x.h>
#include <ScioSense_ENS160.h>
#include <Adafruit_LTR390.h>
#include <Adafruit_MAX1704X.h>
#include <SensirionI2CScd4x.h>
#include <Adafruit_VEML7700.h>

//Define system GPIO pins
#define BUTTON0 0
#define BUTTON1 1
#define BUTTON2 2
#define LED LED_BUILTIN
#define LPS22_INT 13
#define SD_CS 5
#define SD_CD 6
#define NEO_PIXEL_FEATHER PIN_NEOPIXEL
#define NEO_PIXEL_WING 10
#define FILE_BASE_NAME "Log_"
#define TFT_CS 42
#define TFT_DC 40
#define TFT_RST 41
#define TFT_BACKLIGHT 45
#define TFT_I2C_POWER 7
//#define EEPROM_SCIO 9

//Units
typedef struct{
    bool feet;
    bool meters;
    bool pascal;
    bool mbar;
    bool kpa;
    bool inhg;
    bool mmhg;
    bool psi;
    bool celsius;
    bool fahrenheit;
} units;

//Senors
typedef struct{
    bool ens160;
    bool lps22;
    bool ltr390;
    bool max1704;
    bool scd40;
    bool sht41;
    bool veml7700;
} sensors;

//Parameters
typedef struct{
    float alsLTR;       // Ambient light - LTR390
    float alsVEML;      // Ambient light - VEML700
    float alt;          // Altitude - LPS22
    float aqi;          // Air Quality Index (Umweltbundesamt)
    float battPercent;  // Battery cell percentage - MAX1704
    float chrgRate;     // Battery charge rate - MAX1704
    float CO2;          // Carbon-Dioxide - (SCD40)
    float dewPoint;     // Dew point - SHT41
    float eCO2;         // Carbon-Dioxide (ENS160)
    float heatIndex;    // Heat-Index - SHT41
    float humdABS;      // Absolute humidity - SHT41
    float humdSCD;      // Relative humidity - SCD40
    float humdSHT;      // Relative humidity - SHT41
    float pres;         // Absolute pressure - LPS22
    float tempLPS;      // Temperature - LPS22
    float tempSCD;      // Temperature - SCD40
    float tempSHT;      // Temperature - SHT41
    float tvoc;         // Total Volatile Organic Compounds - ENS160
    float uvRaw;        // Raw UV Measurements - LTR390
    float uviLTR;          // UV Index - LTR390
    float voltage;      // Battery cell voltage - MAX1704
} parameters;

//Configuration
typedef struct{
    const char* studentName;
    int refreshRate;
    const char* wifiSSID;
    const char* wifiPASS;
    bool neoPixelRGB;
    bool neoPixelHSV;
    int neoPixelRed;
    int neoPixelGrn;
    int neoPixelBlu;
    int neoPixelBri;
    int neoPixelHue;
    int neoPixelSat;
    int neoPixelVal;
} configuration;

units getUnits(void);
sensors getSensors(void);
configuration getConfig(void);

//Set variable types for functions
sensors initializeSensors(sensors);
parameters getSensorData(sensors);
void initializeTFT(void);
void loop0(void * parameter);
void loop1(void * parameter);

extern units unit;
extern sensors sen;
extern parameters param;
extern configuration config;
extern SdFat SD;
extern SdFile file;
extern String alt, pres, temp, alt_file, pres_file, temp_file;

//int32_t msc_read_cb(uint32_t, void*, uint32_t);
//int32_t msc_write_cb(uint32_t, uint8_t*, uint32_t);
//void msc_flush_cb();

#endif
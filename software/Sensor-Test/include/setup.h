#ifndef _setup_h
#define _setup_h

//System libraries
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>
#include <SdFat.h>
#include <ArduinoJson.h>

//Sensor libraries
#include <Adafruit_LPS2X.h>
#include <Adafruit_SHT4x.h>
#include <ScioSense_ENS160.h>
#include <Adafruit_LTR390.h>
#include <SensirionI2CScd4x.h>
#include <Adafruit_VEML7700.h>

#define SD_CS 5
#define SD_CD 6

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
    bool scd40;
    bool sht41;
    bool veml7700;
} sensors;

//Parameters
typedef struct{
    float alsLTR;    // Ambient light - LTR390
    float alsVEML;   // Ambient light - VEML700
    float alt;       // Altitude
    float aqi;       // Air Quality Index (NowCast)
    float CO2;       // Carbon-Dioxide (SCD40)
    float dewPoint;  // Dew Point - SHT41
    float eCO2;      // Carbon-Dioxide (ENS160)
    float heatIndex; // Heat-Index
    float humdABS;   // Absolute Humidity
    float humdSCD;   // Relative Humidity - SCD40
    float humdSHT;   // Relative Humidity - SHT41
    float pres;      // Absolute Pressure - LPS22
    float tempLPS;   // Temperature - LPS22
    float tempSCD;   // Temperature - SCD40
    float tempSHT;   // Temperature - SHT41
    float tvoc;      // Total Volatile Organic Compounds - ENS160
    float uvRaw;     // Raw UV Measurements - LTR390
    float uviLTR;    // UV Index - LTR390
    //float uviGUVAB;// UV Index - GUVA/B
} parameters;

extern SdFat SD;
extern SdFile file;

units getUnits(void);
sensors getSensors(void);

//Set variable types for functions
sensors INITIALIZE_SENSORS(sensors, int);
parameters GET_SENSOR_DATA(sensors);

extern units unit;
extern sensors sen;
extern parameters param;

#endif
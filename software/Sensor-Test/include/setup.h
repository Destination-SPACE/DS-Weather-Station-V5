#ifndef _setup_h
#define _setup_h

//System libraries
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

//Sensor libraries
#include <Adafruit_LPS2X.h>
#include <Adafruit_SHT4x.h>
#include <ScioSense_ENS160.h>
#include <Adafruit_LTR390.h>
#include <SensirionI2CScd4x.h>
#include <Adafruit_VEML7700.h>

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

units getUnits(void);
sensors getSensors(void);

//Set variable types for functions
sensors INITIALIZE_SENSORS(sensors, int);
parameters GET_SENSOR_DATA(sensors);

extern units unit;
extern sensors sen;
extern parameters param;

//extern bool UNITS_FEET, UNITS_METERS, UNITS_PASCAL, UNITS_MBAR, UNITS_K_PASCAL, UNITS_IN_HG, UNITS_MM_HG, UNITS_PSI, UNITS_CELCIUS, UNITS_FAHRENHEIT;

//Sensors
//extern bool SENSOR_ENS160, SENSOR_GUVA_B, SENSOR_LPS22, SENSOR_LTR390, SENSOR_SCD40, SENSOR_SHT41, SENSOR_VEML7700;

//Set data types
//extern float ENS160_AQI, ENS160_eCO2, ENS160_TVOC, LPS22_ALTITUDE, LPS22_PRESSURE, LPS22_TEMPERATURE, LTR390_RAW_UV, LTR390_UVI, SCD40_CO2, SCD40_HUMIDITY, SCD40_TEMPERATURE, SHT41_ABSOLUTE_HUMIDITY, SHT41_HEAT_INDEX, SHT41_HUMIDITY, SHT41_TEMPERATURE, VEML7700_LUX;

#endif
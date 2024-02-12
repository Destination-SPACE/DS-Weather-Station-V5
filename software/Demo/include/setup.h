//System libraries
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ST7789.h>
//#include <Adafruit_TinyUSB.h>
//#include <ArduinoJson.h>
//#include <SdFat.h>
#include <SPI.h>
#include <Wire.h>
//#include <YAMLDuino.h>

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
#define LPS22_INT 7
#define SD_CS 5
#define SD_CD 6
#define NEO_PIXEL_FEATHER PIN_NEOPIXEL
#define NEO_PIXEL_WING 10

//Declare core tasks
//TaskHandle_t Task0, Task1;
//SemaphoreHandle_t baton;

//SdFat SD;
//SdFile file;

//bool FS_CHANGED;

//Set variable types for functions
void INITIALIZE_SENSORS(bool, bool, bool, bool, bool, bool, bool, bool, int);
void INITIALIZE_TFT();
void loop0(void * parameter);
void loop1(void * parameter);

float GET_SENSOR_DATA(bool, bool, bool, bool, bool, bool, bool, bool);

//int32_t msc_read_cb(uint32_t, void*, uint32_t);
//int32_t msc_write_cb(uint32_t, uint8_t*, uint32_t);
//void msc_flush_cb();

//const char* READ_YAML(const char*);
//void DESERIALIZE_YAML_TO_JSON_OBJECT(const char*);

//Set YAML variable types
extern const char* STUDENT_NAME;
extern int REFRESH_RATE;

//WiFi credentials
extern const char* SSID;
extern const char* PASSWORD;

//NeoPixels
extern bool NEO_PIXEL_RGB;
extern int NEO_RED;
extern int NEO_GREEN;
extern int NEO_BLUE;
extern int NEO_BRIGHTNESS;
extern bool NEO_PIXEL_HSV;
extern int NEO_HUE;
extern int NEO_SATURATION;
extern int NEO_VALUE;

//Units
extern bool UNITS_FEET, UNITS_METERS, UNITS_PASCAL, UNITS_MBAR, UNITS_K_PASCAL, UNITS_IN_HG, UNITS_MM_HG, UNITS_PSI, UNITS_CELCIUS, UNITS_FAHRENHEIT;

//Sensors
extern bool SENSOR_ENS160, SENSOR_GUVA_B, SENSOR_LPS22, SENSOR_LTR390, SENSOR_MAX17048, SENSOR_SCD40, SENSOR_SHT41, SENSOR_VEML7700;

//Set data types
extern float ENS160_AQI, ENS160_eCO2, ENS160_TVOC, LPS22_ALTITUDE, LPS22_PRESSURE, LPS22_TEMPERATURE, LTR390_RAW_UV, LTR390_UVI, MAX17048_CHARGE_RATE, MAX17048_PERCENTAGE, MAX17048_VOLTAGE, SCD40_CO2, SCD40_HUMIDITY, SCD40_TEMPERATURE, SHT41_ABSOLUTE_HUMIDITY, SHT41_HEAT_INDEX,SHT41_HUMIDITY, SHT41_TEMPERATURE, VEML7700_LUX;

extern float ENS160_AQI_PREV;
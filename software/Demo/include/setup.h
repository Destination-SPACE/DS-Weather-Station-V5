//Framework library
#include <Arduino.h>

//Default libraries
#include <SPI.h>
#include <Wire.h>

//System libraries
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SdFat.h>
#include <SPI.h>
#include <Wire.h>
#include <YAMLDuino.h>

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


//Declare system components
Adafruit_NeoPixel PIXEL_FEATHER(1, NEO_PIXEL_FEATHER);
Adafruit_NeoPixel PIXEL_WING(1, NEO_PIXEL_WING, NEO_GRB + NEO_KHZ800);
Adafruit_ST7789 TFT = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
Adafruit_USBD_MSC USB_MSC;

//Declare core tasks
TaskHandle_t Task0, Task1;
SemaphoreHandle_t baton;

//Declare sensors
ScioSense_ENS160 ENS160(ENS160_I2CADDR_1);
Adafruit_LPS22 LPS22;
Adafruit_LTR390 LTR390 = Adafruit_LTR390();
Adafruit_MAX17048 MAX17048;
SensirionI2CScd4x SCD40;
Adafruit_SHT4x SHT41 = Adafruit_SHT4x();
Adafruit_VEML7700 VEML7700 = Adafruit_VEML7700();


SdFat SD;
SdFile ROOT;
SdFile FILE;

bool FS_CHANGED;

//Set variable types for functions
void INITIALIZE_SENSORS(bool, bool, bool, bool, bool, bool, bool, bool, int);
void INITIALIZE_TFT();
void loop0(void * parameter);
void loop1(void * parameter);

float GET_SENSOR_DATA(bool, bool, bool, bool, bool, bool, bool, bool);

//Set YAML variable types
const char* STUDENT_NAME;
int REFRESH_RATE;

//WiFi credentials
const char* SSID;
const char* PASSWORD;

//NeoPixels
bool NEO_PIXEL_RGB;
int NEO_RED;
int NEO_GREEN;
int NEO_BLUE;
int NEO_BRIGHTNESS;
bool NEO_PIXEL_HSV;
int NEO_HUE;
int NEO_SATURATION;
int NEO_VALUE;

//Units
bool UNITS_FEET, UNITS_METERS, UNITS_PASCAL, UNITS_MBAR, UNITS_K_PASCAL, UNITS_IN_HG, UNITS_MM_HG, UNITS_PSI, UNITS_CELCIUS, UNITS_FAHRENHEIT;

//Sensors
bool SENSOR_ENS160, SENSOR_ENS160, SENSOR_ENS160, SENSOR_GUVA_B, SENSOR_LPS22, SENSOR_LTR390, SENSOR_MAX17048, SENSOR_SCD40, SENSOR_SHT41, SENSOR_VEML7700;

//Set data types
float ENS160_AQI, ENS160_eCO2, ENS160_TVOC, LPS22_ALTITUDE, LPS22_PRESSURE, LPS22_TEMPERATURE, LTR390_RAW_UV, LTR390_UVI, MAX17048_CHARGE_RATE, MAX17048_PERCENTAGE, MAX17048_VOLTAGE, SCD40_CO2, SCD40_HUMIDITY, SCD40_TEMPERATURE, SHT41_ABSOLUTE_HUMIDITY, SHT41_HEAT_INDEX,SHT41_HUMIDITY, SHT41_TEMPERATURE, VEML7700_LUX;

float ENS160_AQI_PREV = 0.0f;
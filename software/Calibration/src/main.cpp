#include <Arduino.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_LPS22 LPS22;
Adafruit_SHT4x SHT41 = Adafruit_SHT4x();

//EEPROM Instructions
#define EEPROM_SCIO 13 //device GPIO
uint8_t EEPROM_READ = 0x3C;
uint8_t EEPROM_CCRD = 0x06;
uint8_t EEPROM_WRITE = 0x6C;
uint8_t EEPROM_WREN = 0x96;

#define SHT_CAL_TEMP_MSB_ADDR 0
#define SHT_CAL_TEMP_LSB_ADDR 1
#define SHT_CAL_HUM_MSB_ADDR 2
#define SHT_CAL_HUM_LSB_ADDR 3

void setup() {
  Serial.begin(115200);
  while(!Serial){
    delay(10);
  }
  Wire.begin();
  Serial.print("+==============================================================================+\n|                Destination Weather Station v5.1 - Calibration                |\n+==============================================================================+");
  Serial.print("\n\nInitializing Sensors...");
  if(!LPS22.begin_I2C()){
    Serial.print("\nLPS22 Not Found");
    while(true);
  }
  else{
    LPS22.setDataRate(LPS22_RATE_10_HZ);
  }
  if(!SHT41.begin()){
    Serial.print("\nSHT41 Not Found");
    while(true);
  }
  else{
    SHT41.setPrecision(SHT4X_HIGH_PRECISION);
    SHT41.setHeater(SHT4X_NO_HEATER);
  }

  Serial.print("\n\nType 'help' for a list of commands");
}

void loop() {
  String cmdInput = userInput();
  processUserInput(cmdInput);
}

String userInput(){
  while(true){
    if(Serial.available() > 0){
      char userInputChar = Serial.read();
      if(userInputChar == 27){
        break;
      }
      else{
        String userInputString = String(userInputChar);
        while(Serial.available() > 0){
          char nextChar = Serial.read();
          if(nextChar == '\n'){
            break;
          }
          userInputString += nextChar;
        }
        return userInputString;
        break;
      }
    }
    delay(10);
  }
}

void processUserInput(String userInputString){
  if(userInputString == "SET_LPS_CAL"){
    SET_LPS_CAL();
  }
  else if(userInputString == "SET_SHT_CAL_TEMP"){
    SET_SHT_CAL_TEMP();
  }
  else if(userInputString == "SET_SHT_CAL_HUM"){
    SET_SHT_CAL_HUM();
  }
  else if(userInputString == "GET_SHT_CAL_TEMP"){
    GET_SHT_CAL_TEMP();
  }
  else if(userInputString == "GET_SHT_CAL_HUM"){
    GET_SHT_CAL_HUM();
  }
  else if(userInputString == "GET_PRES"){
    GET_PRES();
  }
  else if(userInputString == "GET_TEMP"){
    GET_TEMP();
  }
  else if(userInputString == "GET_HUM"){
    GET_HUM();
  }
  else if(userInputString == "GET_PRES"){
    GET_PRES();
  }
  else if(userInputString == "GET_PRES"){
    GET_PRES();
  }
  else if(userInputString == "CLEAR"){
    CLEAR();
  }
  else if(userInputString == "help"){
    help();
  }
  else{
    Serial.print("Invalid Command. Type 'help' for a list of commands");
  }
}

void SET_LPS_CAL(){
  Serial.print("\nEnter one-point calibration value: ");
  String opcString = userInput();
  float opcFloat = atof(opcString.c_str());

  sensors_event_t LPS22_TEMPERATURE_SEN, LPS22_PRESSURE_SEN;
  LPS22.getEvent(&LPS22_PRESSURE_SEN, &LPS22_TEMPERATURE_SEN);
  float LPS22_PRESSURE = LPS22_PRESSURE_SEN.pressure;

  float offsetFloat = opcFloat - LPS22_PRESSURE;
  int16_t offsetInt16 = static_cast<int16_t>(offsetFloat);
  uint16_t twosComplement = ~offsetInt16 + 1; // Invert all bits and add 1 to find the 2's complement
  uint8_t MSB = static_cast<uint8_t>(twosComplement & 0xFF);
  uint8_t LSB = static_cast<uint8_t>((twosComplement >> 8) & 0xFF);
  
  Wire.beginTransmission(0x5D);
  Wire.write(0x19);
  Wire.write(MSB);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(0x5D);
  Wire.write(0x18);
  Wire.write(LSB);
  Wire.endTransmission();
}

void SET_SHT_CAL_TEMP(){
  Serial.print("\nEnter one-point calibration value: ");
  String opcString = userInput();
  float opcFloat = atof(opcString.c_str());

  sensors_event_t SHT41_TEMPERATURE_SEN, SHT41_HUMIDITY_SEN;
  SHT41.getEvent(&SHT41_HUMIDITY_SEN, &SHT41_TEMPERATURE_SEN);
  float SHT41_TEMPERATURE = SHT41_TEMPERATURE_SEN.temperature;

  float offsetFloat = (opcFloat - SHT41_TEMPERATURE)*100; // Multiply by 100 to keep precision
  int16_t offsetInt16 = static_cast<int16_t>(offsetFloat);
  uint16_t twosComplement = ~offsetInt16 + 1; // Invert all bits and add 1 to find the 2's complement
  uint8_t MSB = static_cast<uint8_t>(twosComplement & 0xFF);
  uint8_t LSB = static_cast<uint8_t>((twosComplement >> 8) & 0xFF);

  EEPROM.write(SHT_CAL_TEMP_MSB_ADDR, MSB);
  EEPROM.write(SHT_CAL_TEMP_LSB_ADDR, LSB);
  EEPROM.commit();
}

void SET_SHT_CAL_HUM(){
  Serial.print("\nEnter one-point calibration value: ");
  String opcString = userInput();
  float opcFloat = atof(opcString.c_str());

  sensors_event_t SHT41_TEMPERATURE_SEN, SHT41_HUMIDITY_SEN;
  SHT41.getEvent(&SHT41_HUMIDITY_SEN, &SHT41_TEMPERATURE_SEN);
  float SHT41_HUMIDITY = SHT41_HUMIDITY_SEN.relative_humidity;

  float offsetFloat = (opcFloat - SHT41_HUMIDITY)*100; // Multiply by 100 to keep precision
  int16_t offsetInt16 = static_cast<int16_t>(offsetFloat);
  uint16_t twosComplement = ~offsetInt16 + 1; // Invert all bits and add 1 to find the 2's complement
  uint8_t MSB = static_cast<uint8_t>(twosComplement & 0xFF);
  uint8_t LSB = static_cast<uint8_t>((twosComplement >> 8) & 0xFF);

  EEPROM.write(SHT_CAL_HUM_MSB_ADDR, MSB);
  EEPROM.write(SHT_CAL_HUM_LSB_ADDR, LSB);
  EEPROM.commit();
}

//Get Commands
void GET_SHT_CAL_TEMP(){
  uint8_t MSB = EEPROM.read(SHT_CAL_TEMP_MSB_ADDR);
  uint8_t LSB = EEPROM.read(SHT_CAL_TEMP_LSB_ADDR);
  uint16_t twosCompliment = (MSB << 8) | (LSB & 0xFF);
  int16_t offsetInt16 = ~twosCompliment + 1;
  float offsetFloat = static_cast<float>(offsetInt16) / 100.00; // Divide by 100 to recover precision
  Serial.print("\n\nSHT41 Offset Temperature: ");
  Serial.print(offsetFloat, 2);
}

void GET_SHT_CAL_HUM(){
  uint8_t MSB = EEPROM.read(SHT_CAL_HUM_MSB_ADDR);
  uint8_t LSB = EEPROM.read(SHT_CAL_HUM_LSB_ADDR);
  uint16_t twosCompliment = (MSB << 8) | (LSB & 0xFF);
  int16_t offsetInt16 = ~twosCompliment + 1;
  float offsetFloat = static_cast<float>(offsetInt16) / 100.00; // Divide by 100 to recover precision
  Serial.print("\n\nSHT41 Offset Humidity: ");
  Serial.print(offsetFloat, 2);
}

void GET_PRES(){
  sensors_event_t LPS22_TEMPERATURE_SEN, LPS22_PRESSURE_SEN;
  LPS22.getEvent(&LPS22_PRESSURE_SEN, &LPS22_TEMPERATURE_SEN);
  float LPS22_PRESSURE = LPS22_PRESSURE_SEN.pressure;
  Serial.print("Absolute Pressure [hPa]: ");
  Serial.print(LPS22_PRESSURE,2);
  Serial.print("\n\n");
}

void GET_TEMP(){
  sensors_event_t SHT41_TEMPERATURE_SEN, SHT41_HUMIDITY_SEN;
  SHT41.getEvent(&SHT41_HUMIDITY_SEN, &SHT41_TEMPERATURE_SEN);
  float SHT41_TEMPERATURE = SHT41_TEMPERATURE_SEN.temperature;

  Serial.print("Ambient Temperature [°C]: ");
  Serial.print(SHT41_TEMPERATURE,2);
  Serial.print("\n\n");
}

void GET_HUM(){
  sensors_event_t SHT41_TEMPERATURE_SEN, SHT41_HUMIDITY_SEN;
  SHT41.getEvent(&SHT41_HUMIDITY_SEN, &SHT41_TEMPERATURE_SEN);
  float SHT41_HUMIDITY = SHT41_HUMIDITY_SEN.relative_humidity;

  Serial.print("Relative Humidity [%]: ");
  Serial.print(SHT41_HUMIDITY,2);
  Serial.print("\n\n");
}

void CLEAR(){
  Serial.print("\033[2J");
  Serial.print("\033[H");
}

void help(){
  Serial.print("\nSET_LPS_CAL - Set Pressure Offset");
  Serial.print("\nSET_SHT_CAL_TEMP - Set Temperature Offset");
  Serial.print("\nSET_SHT_CAL_HUM - Set Humidity Offset");
  Serial.print("\nGET_SHT_CAL_TEMP - Get Temperature Offset");
  Serial.print("\nGET_SHT_CAL_HUM - Get Humidity Offset");
  Serial.print("\nGET_PRES - Get Pressure Sensor Value");
  Serial.print("\nGET_TEMP - Get Temperature Value");
  Serial.print("\nGET_HUM - Get Humidity Value");
  Serial.print("\nCLEAR - Clear the Terminal");
}
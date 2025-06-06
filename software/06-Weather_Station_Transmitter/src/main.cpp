/*******************************************************************************
DESTINATION WEATHER FEATHER WING EXAMPLE - DEMO

This is an example sketch for the Destination Weather FeatherWing remote sensing
platform to test the microSD card reader.

modified 2024-07-16
by Madison Gleydura

MIT LICENSE AGREEMENT

Copyright © 2024 Destination SPACE Inc.

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
#include "setup.h"

TaskHandle_t Task0;
TaskHandle_t Task1;

WiFiServer server(80);

int clock_timer, hh, mm, ss;
char fileName[13] = FILE_BASE_NAME "00.csv";

//Declare system components
Adafruit_NeoPixel PIXEL_FEATHER(1, NEO_PIXEL_FEATHER);
Adafruit_NeoPixel PIXEL_WING(1, NEO_PIXEL_WING, NEO_GRB + NEO_KHZ800);

void task0code(void * parameter);
void task1code(void * parameter);

sensors_event_t LPS22_TEMPERATURE_SEN, LPS22_PRESSURE_SEN, SHT41_TEMPERATURE_SEN, SHT41_HUMIDITY_SEN;


void setup() {
  Serial.begin(115200);
  //while(!Serial);
  Serial.print("Destination Weather Station v5!");

  //Setup Digital IO
  Serial.print("\n\nSetting up digital IO...");
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLDOWN);
  pinMode(BUTTON2, INPUT_PULLDOWN);
  pinMode(LED, OUTPUT);
  pinMode(LPS22_INT, INPUT);
  pinMode(NEO_PIXEL_FEATHER, OUTPUT);
  pinMode(NEO_PIXEL_WING, OUTPUT);
  pinMode(SD_CD, INPUT);
  Serial.print("\nDigital IO setup complete!");

  Serial.print("\nInitializing NeoPixels...");
  PIXEL_FEATHER.begin(); // Initialize built-in NeoPixel
  PIXEL_WING.begin(); // Initialize FeatherWing NeoPixel
  Serial.print("\nNeoPixels initialized!");
  
  Serial.print("\nI2C bus...");
  Wire.begin();
  Serial.print("\nI2C bus online!");

  Serial.print("\nInitializing TFT display...");
  initializeTFT();
  Serial.print("\nTFT display initialized!");

  Serial.print("\nChecking for SD card...");
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(0,0);
  tft.print("Checking for SD card...");

  const uint8_t baseNameSize = sizeof(FILE_BASE_NAME) - 1;
  if(!digitalRead(SD_CD)){
    Serial.print("\nPlease insert microSD card.");
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0,0);
    tft.print("Please insert microSD card.");
    while(true){
      if(digitalRead(SD_CD) == 0){
        break;
      }
    }
  }
  else{
    Serial.print("\nSD card detected!");
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0,0);
    tft.print("SD card detected");
  }

  Serial.print("\nInitializing SD card...");
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  tft.print("Initialized SD card...");
  if(!SD.begin(SD_CS, SD_SCK_MHZ(50))){
    Serial.print("\nSD card not responding.");
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0,0);
    tft.print("SD card not responding.");
    while(true){
      if(SD.begin(SD_CS, SD_SCK_MHZ(50))){
        break;
      }
      delay(100);
    }
  }
  Serial.print("\nSD card initialized!");
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  tft.print("SD card initialized!");

  Serial.print("\nImporting configuration settings...");
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  tft.print("Importing configuration settings...");
  config = getConfig();
  Serial.print("\nDone.");
  Serial.print("\nImporting sensor settings...");
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  tft.print("Importing sensor settings...");
  sen = getSensors();
  Serial.print("\nDone.");
  Serial.print("\nInitializing sensors...\n");
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  tft.print("Initializing sensors...");
  sen = initializeSensors(sen);
  Serial.print("\nDone.");

  Serial.print("\nCreating CSV file...");
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  tft.print("Creating CSV file...");
  if(baseNameSize > 6){
    Serial.print("\n\nFile base name too long.");
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0,0);
    tft.print("File base name too long.");
  }
  while(SD.exists(fileName)){
    if(fileName[baseNameSize + 1] != '9'){
      fileName[baseNameSize + 1]++;
    }
    else if(fileName[baseNameSize] != '9'){
      fileName[baseNameSize + 1] = '0';
      fileName[baseNameSize]++;
    }
    else{
      Serial.print("\n\nCan't create file name.");
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(0,0);
      tft.print("Can't create file name.");
    }
  }
  if(file.open(fileName, O_RDWR | O_CREAT | O_AT_END)){
    char buffer[512];
    file.println("Time (s),Temp C,Hum (%),HI C, Dew Point C, PRES mbar,ALT m,CO2 (ppm), eCO2 (ppm), TVOC (ppb),AQI (1-5), RAW UV, UVI (0-11),LUX (k-lux)\n");
    file.close();
    Serial.print("\nCSV file created!");
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0,0);
    tft.print("CSV file created!");
  }
  else{
    Serial.print("\n\nError opening file.");
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0,0);
    tft.print("Error opening file.");
  }

  const char* ssid = config.wifiSSID.c_str();
  const char* pass = config.wifiPASS.c_str();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  server.begin();


  //Initialize core loops
  Serial.print("\nInitializing cores...");
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  tft.print("Initializing cores...");
  xTaskCreatePinnedToCore(task0code, "Task0", 10000, NULL, 1, &Task0, 0);
  delay(500);
  xTaskCreatePinnedToCore(task1code, "Task1", 10000, NULL, 1, &Task1, 1);
  delay(500);
  Serial.print("\nCores initialized!");

  Serial.print("\n\nInitialization complete!");
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  tft.print("Initialization complete!");
}

void task0code(void * parameter){
  delay(5000);
  for(;;){
    int time_prev = millis();
    param = getSensorData(sen);

    //Clock timer
    ss = (millis() - clock_timer)/1000; // Set seconds to how much time has elapsed since first data read
    if(ss >= 60){ // If seconds is 60+, add 1 minutes and reset seconds variable
      clock_timer = millis();
      ss = ss - 60;
      mm = mm + 1;
      if(mm >= 60){ // If minutes is 60+, add 1 hours and reset minutes and seconds
        ss = ss - 60;
        mm = mm - 60;
        hh = hh + 1;
      }
    }

    char fileBuffer[1024];

    sprintf(fileBuffer,"%02d:%02d:%02d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", hh, mm, ss, param.tempSHT, param.humdSHT, param.heatIndex, param.dewPoint, param.pres, param.alt, param.CO2, param.eCO2, param.tvoc, param.aqi, param.uvRaw, param.uviLTR, param.alsVEML);

    if(file.open(fileName, O_RDWR | O_CREAT | O_AT_END)){
      file.println(fileBuffer);
      
      file.close();
    }

    char wifiBuffer[1024];
    sprintf(wifiBuffer,"\n|%02d:%02d:%02d| %4.1f |%5.2f| %4.1f | %4.0f |%5.1f| %4.0f| %4.0f |   %1.0f   | %5.2f |%6.2f|", hh, mm, ss, param.tempSHT, param.humdSHT, param.heatIndex, param.pres, param.alt, param.CO2, param.tvoc, param.aqi, param.uviLTR, param.alsVEML);

    WiFiClient client = server.available();
    if(client){
      client.println(wifiBuffer);
      client.stop();
    }

    while(true){
      if(millis() - time_prev >= config.refreshRate){
         break;
      }
      delay(5);
    }
  }
}

void task1code(void * parameter){
  delay(5000);
  sensorScr();
  for(;;){
    homeScreen:
    dispSensor();
    btnState = readButtons();

    if(btnState.BTN0_State){  // Expansion sensor screen
      btnState.BTN0_State = false;
      expansionScr();
      while(true){
        dispExpansion();
        btnState = readButtons();

        if(btnState.BTN0_State){  // System health screen
          btnState.BTN0_State = false;
          sysHealthScr();
          while(true){
            dispSysHealth();
            btnState = readButtons();

            if(btnState.BTN0_State){  // Device information screen
              btnState.BTN0_State = false;
              infoScr();
              while(true){
                btnState = readButtons();

                if(btnState.BTN0_State){
                  btnState.BTN0_State = false;
                  sensorScr();
                  goto homeScreen;
                }
                else if(btnState.BTN1_State){
                  btnState.BTN1_State = false;
                }
                else if(btnState.BTN2_State){
                  btnState.BTN2_State = false;
                  goto homeScreen;
                }
              }
            }
            else if(btnState.BTN1_State){
              btnState.BTN1_State = false;
            }
            else if(btnState.BTN2_State){
              btnState.BTN2_State = false;
              sensorScr();
              goto homeScreen;
            }
            else{
            }
          }
        }
        else if(btnState.BTN1_State){
          btnState.BTN1_State = false;
        }
        else if(btnState.BTN2_State){
          btnState.BTN2_State = false;
          sensorScr();
          goto homeScreen;
        }
        else{
        }
      }
    }
    else if(btnState.BTN1_State){
      btnState.BTN1_State = false;
    }
    else if(btnState.BTN2_State){
      btnState.BTN2_State = false;
      sensorScr();
      goto homeScreen;
    }
    else{
    }
  }
}

void loop() {
}
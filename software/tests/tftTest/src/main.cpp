#include <Arduino.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>

#define BUTTON0 0
#define BUTTON1 1
#define BUTTON2 2
#define TFT_CS 42
#define TFT_DC 40
#define TFT_RST 41
#define TFT_BACKLIGHT 45
#define TFT_I2C_POWER 7

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

typedef struct{
  bool BTN0_State;
  bool BTN1_State;
  bool BTN2_State;
} buttonStates;

buttonStates buttonState;

bool stop = false;

buttonStates readButtons(void);
void sensorScreen();
void expansionScreen();
void systemHealth();
void navBar();

void setup() {
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLDOWN);
  pinMode(BUTTON2, INPUT_PULLDOWN);

  pinMode(TFT_BACKLIGHT, OUTPUT);
  pinMode(TFT_I2C_POWER, OUTPUT);

  digitalWrite(TFT_BACKLIGHT, HIGH);
  digitalWrite(TFT_I2C_POWER, HIGH);

  tft.init(135, 240); // Define TFT resolution
  tft.setRotation(3); // CCW orientation
  sensorScreen();
}

void loop() {
  sensor_screen:
  buttonState = readButtons();

  if(buttonState.BTN0_State){
    buttonState.BTN0_State = false;
    expansionScreen();
    while(true){  // Expansion Sensor Measurements
      buttonState = readButtons();

      if(buttonState.BTN0_State){
        buttonState.BTN0_State = false;
        systemHealth();
        while(true){  // System Health
          buttonState = readButtons();

          if(buttonState.BTN0_State){
            buttonState.BTN0_State = false;
            
          }
          else if(buttonState.BTN1_State){

          }
          else if(buttonState.BTN2_State){
            sensorScreen();
            goto sensor_screen;
          }
          else{
          }
        }
      }
      else if(buttonState.BTN1_State){

      }
      else if(buttonState.BTN2_State){
        sensorScreen();
        goto sensor_screen;
      }
      else{
      }
    }
  }
  else if(buttonState.BTN1_State){
    buttonState.BTN1_State = false;
    navBar();
    while(true){
      if(buttonState.BTN0_State){
            
      }
      else if(buttonState.BTN1_State){

      }
      else if(buttonState.BTN2_State){
        sensorScreen();
        goto sensor_screen;
      }
      else{
      }
    }
  }
  else if(buttonState.BTN2_State){

  }
  else{
  }
}

buttonStates readButtons(){
  buttonState.BTN0_State = false;
  buttonState.BTN1_State = false;
  buttonState.BTN2_State = false;
  uint32_t lastDebounceTime;

  if(!digitalRead(BUTTON0) || digitalRead(BUTTON1) || digitalRead(BUTTON2)){
    lastDebounceTime = millis();
  }

  while(true){
    if((millis() - lastDebounceTime) > 50){
      if(!digitalRead(BUTTON0)){
        buttonState.BTN0_State = true;
        break;
      }
      else if(digitalRead(BUTTON1)){
        buttonState.BTN1_State = true;
        break;
      }
      else if(digitalRead(BUTTON2)){
        buttonState.BTN2_State = true;
        break;
      }
      else{
        break;
      }
    }
  }

  return buttonState;
}

void sensorScreen(){
  navBar();
  tft.setTextSize(2);
  tft.setCursor(20,0);
  tft.setTextColor(ST77XX_RED);
  tft.println("Sen. Measurements");
  tft.setCursor(20,40);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Temperature:");
  tft.setCursor(168,40);
  tft.print("18.8\367C");
  tft.setCursor(20,60);
  tft.println("Pressure:");
  tft.setCursor(132,60);
  tft.print("1013.2hPa");
  tft.setCursor(20,80);
  tft.println("Humidity:");
  tft.setCursor(204,80);
  tft.print("58%");
  delay(200);
}

void expansionScreen(){
  navBar();
  tft.setTextSize(2);
  tft.setCursor(20,0);
  tft.setTextColor(ST77XX_RED);
  tft.print("Exp. Measurements");
  tft.setCursor(20,20);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("CO2:");
  tft.setCursor(20,40);
  tft.print("TVOC:");
  tft.setCursor(20,60);
  tft.print("AQI:");
  tft.setCursor(20,80);
  tft.print("UVI:");
  tft.setCursor(20,100);
  tft.print("ALS:");
}

void systemHealth(){
  navBar();
  tft.setCursor(20,0);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_RED);
  tft.println("System Health");
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Battery Percentage: 45%");
  tft.println("Voltage: 3.8V");
  tft.println("Recording Data: No");
  delay(200);
}

void navBar(){
  tft.fillScreen(ST77XX_BLACK);
  tft.drawChar(0,0,0x5D,ST77XX_WHITE,0,2);
  tft.drawChar(0,61,0x3E,ST77XX_WHITE,0,2);
  tft.drawChar(0,119,0x7F,ST77XX_WHITE,0,2);

  tft.drawRect(174,119,44,16,ST77XX_WHITE);
  tft.fillRect(218,123,4,8,ST77XX_WHITE);
  tft.fillRect(175,120,42,14,ST77XX_GREEN);
  
  tft.drawLine(233,125,240,118,ST77XX_YELLOW);
  tft.fillRect(233,125,7,2,ST77XX_YELLOW);
  tft.drawLine(233,134,240,126,ST77XX_YELLOW);

  delay(200);
}
#include "setup.h"

btnStates btnState;
uint32_t debounceTime = 50;
char  tempBuffer[64],
      presBuffer[64],
      humdBuffer[64],
      CO2Buffer[64],
      TVOCBuffer[64],
      AQIBuffer[64],
      UVIBuffer[64],
      ALSBuffer[64],
      BattPctBuffer[64],
      BattVBuffer[64],
      NameBuffer[64],
      *tempBufferPrev,
      *presBufferPrev,
      *humdBufferPrev,
      *CO2BufferPrev,
      *TVOCBufferPrev,
      *AQIBufferPrev,
      *UVIBufferPrev,
      *ALSBufferPrev,
      *BattPctBufferPrev,
      *BattVBufferPrev;



void initializeTFT(){
  pinMode(TFT_BACKLIGHT, OUTPUT);
  pinMode(TFT_I2C_POWER, OUTPUT);

  digitalWrite(TFT_BACKLIGHT, HIGH);
  digitalWrite(TFT_I2C_POWER, HIGH);

  tft.init(135, 240); // Define TFT resolution
  tft.setRotation(3); // CCW orientation
  tft.fillScreen(ST77XX_BLACK); // Clear screen
}

void sensorScr(){
    navBar();
    tft.setTextSize(2);
    tft.setCursor(20,0);
    tft.setTextColor(ST77XX_RED);
    tft.print("WX. Measurements");
    tft.setCursor(20,40);
    tft.setTextColor(ST77XX_WHITE);
    tft.print("Temp:");
    tft.setCursor(20,60);
    tft.print("Pres:");
    tft.setCursor(20,80);
    tft.print("Humd:");
    delay(200);
}

void expansionScr(){
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
    delay(200);
}

void sysHealthScr(){
    navBar();
    tft.setTextSize(2);
    tft.setCursor(20,0);
    tft.setTextColor(ST77XX_RED);
    tft.print("System Health");
    tft.setCursor(20,40);
    tft.setTextColor(ST77XX_WHITE);
    tft.print("Batt [%]:");
    tft.setCursor(20,60);
    tft.print("Batt [V]:");
    tft.setCursor(20,80);
    tft.print("Charging?");
    delay(200);
}

void infoScr(){
  navBar();

  tft.setTextSize(2);
  tft.setCursor(20,0);
  tft.setTextColor(ST77XX_RED);
  tft.print("Information");
  tft.setCursor(20,40);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Destination SPACE");
  tft.setCursor(20,60);
  tft.print("Weather Station v5");
  tft.setCursor(20,80);
  tft.print(config.studentName);
}

void dispSensor(){
  sprintf(tempBuffer, "%.1f\367%s", param.tempSHT, tempDisp);
  sprintf(presBuffer, "%5.1f%s", param.pres, presDisp);
  sprintf(humdBuffer, "%.1f%%", param.humdSHT);

  int tempSize = std::string(tempBuffer).length()*12;
  int presSize = std::string(presBuffer).length()*12;
  int humdSize = std::string(humdBuffer).length()*12;

  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setCursor((240-tempSize),40);
  tft.print(tempBuffer);
  tft.setCursor((240-presSize),60);
  tft.print(presBuffer);
  tft.setCursor((240-humdSize),80);
  tft.print(humdBuffer);

  delay(100);
}

void dispExpansion(){
  sprintf(CO2Buffer, "%.0fppm", param.CO2);
  sprintf(TVOCBuffer, "%.0fppb", param.tvoc);
  sprintf(AQIBuffer, "%.0f", param.aqi);
  sprintf(UVIBuffer, "%.0f", param.uviLTR);
  sprintf(ALSBuffer, "%.2f%k-lux", param.alsVEML);

  int CO2Size = std::string(CO2Buffer).length()*12;
  int TVOCSize = std::string(TVOCBuffer).length()*12;
  int AQISize = std::string(AQIBuffer).length()*12;
  int UVISize = std::string(UVIBuffer).length()*12;
  int ALSSize = std::string(ALSBuffer).length()*12;

  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setCursor((240-CO2Size),20);
  tft.print(CO2Buffer);
  tft.setCursor((240-TVOCSize),40);
  tft.print(TVOCBuffer);
  tft.setCursor((240-AQISize),60);
  tft.print(AQIBuffer);
  tft.setCursor((240-UVISize),80);
  tft.print(UVIBuffer);
  tft.setCursor((240-ALSSize),100);
  tft.print(ALSBuffer);

  delay(100);
}

void dispSysHealth(){
  sprintf(BattPctBuffer, "%0.2f%%", param.battPercent);
  sprintf(BattVBuffer, "%0.2fV", param.voltage);

  char chgBuffer[32];
  

  if(tud_mounted() == true){
    sprintf(chgBuffer, "Yes");
  }
  else if(tud_mounted() == false) {
    sprintf(chgBuffer, "No");
  }
  else{
    sprintf(chgBuffer, "N/A");
  }

  int BattPctSize = std::string(BattPctBuffer).length()*12;
  int BattVSize = std::string(BattVBuffer).length()*12;
  int recSize = std::string(chgBuffer).length()*12;

  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setCursor((240-BattPctSize),40);
  tft.print(BattPctBuffer);
  tft.setCursor((240-BattVSize),60);
  tft.print(BattVBuffer);
  tft.setCursor((240-recSize),80);
  tft.print(chgBuffer);

  delay(100);
}

void navBar(){
    tft.fillScreen(ST77XX_BLACK);

    int battPct = param.battPercent;

    tft.drawChar(0,0,0x5D,ST77XX_WHITE,0,2);
    tft.drawChar(0,61,0x3E,ST77XX_WHITE,0,2);
    tft.drawChar(0,119,0x7F,ST77XX_WHITE,0,2);

    tft.drawRect(192,119,44,16,ST77XX_WHITE);
    tft.fillRect(236,123,4,8,ST77XX_WHITE);

    if(battPct >= 66){
      tft.fillRect(193,120,42,14,ST77XX_GREEN);
    }
    else if(battPct >= 33 && battPct < 66){
      tft.fillRect(193,120,28,14,ST77XX_YELLOW);
    }
    else{
      tft.fillRect(193,120,14,14,ST77XX_RED);
    }

    delay(200);
}

btnStates readButtons(){
  btnState.BTN0_State = false;
  btnState.BTN1_State = false;
  btnState.BTN2_State = false;
  uint32_t lastDebounceTime;

  if(!digitalRead(BUTTON0) || digitalRead(BUTTON1) || digitalRead(BUTTON2)){
    lastDebounceTime = millis();
  }

  while(true){
    if((millis() - lastDebounceTime) > 50){
      if(!digitalRead(BUTTON0)){
        btnState.BTN0_State = true;
        break;
      }
      else if(digitalRead(BUTTON1)){
        btnState.BTN1_State = true;
        break;
      }
      else if(digitalRead(BUTTON2)){
        btnState.BTN2_State = true;
        break;
      }
      else{
        break;
      }
    }
  }

  return btnState;
}
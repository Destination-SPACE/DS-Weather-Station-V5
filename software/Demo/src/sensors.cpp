#include "setup.h"
#include <Adafruit_Sensor.h>

void INITIALIZE_SENSORS(bool SENSOR_ENS160, bool SENSOR_GUVA_B, bool SENSOR_LPS22, bool SENSOR_LTR390, bool SENSOR_MAX17048, bool SENSOR_SCD40, bool SENSOR_SHT41, bool SENSOR_VEML7700, int DATA_RATE){
    if(SENSOR_ENS160){
        if(!ENS160.begin()) Serial.print("\n\nENS160 Not Found"); SENSOR_ENS160 = false;
        if(!ENS160.setMode(ENS160_OPMODE_STD)) Serial.print("\n\nENS160 Mode-Set Failure - Restart Device"); while(1) delay(10);

        if(1/DATA_RATE > 1) Serial.print("\n\nRefresh Rate Too Fast! Decrease Speed"); while(1) delay(10);
        
        //Get device validity flag
        Wire.beginTransmission(ENS160_I2CADDR_1);
        Wire.requestFrom(ENS160_REG_DATA_STATUS,1);
        byte STATUS = Wire.read();
        Wire.endTransmission();
        bool VALIDITY_BIT1 = (STATUS & (1 << 1)) != 0;
        bool VALIDITY_BIT2 = (STATUS & (1 << 2)) != 0;
        int VALIDITY = (VALIDITY_BIT1 << 1) | VALIDITY_BIT2;

        //Check the status of the device
        switch(VALIDITY){
            case 0:     // Normal operation 
                break;
            case 1:     // Warm-up phase
                Serial.print("\n\nENS160 Warming Up...");
                for(int i = 180; i > 0; i--){
                    Serial.printf("\nT- %d s", i);
                    delay(1000);
                }
                break;
            case 2:     // Initial start-up phase
                Serial.print("\n\nENS160 Initial Start-up...");
                for(int i = 3600; i > 0; i--){
                    Serial.printf("\nT- %d s", i);
                }
                break;
            default:
                Serial.print("\n\nENS160 Validity Flag Retrieval  Failure - Restart Device"); while(1) delay(10);
        }
    }
    if(SENSOR_GUVA_B){
        //tbd
    }
    if(SENSOR_LPS22){
        if(!LPS22.begin_I2C()) Serial.print("\n\nLSP22 Not Found"); SENSOR_LPS22 = false;
        if(1/DATA_RATE > 10) LPS22.setDataRate(LPS22_RATE_10_HZ);
        else if(1/DATA_RATE > 50) LPS22.setDataRate(LPS22_RATE_50_HZ);
        else Serial.print("\n\nRefresh Rate Too Fast! Decrease Speed"); while(1) delay(10);
    }
    if(SENSOR_LTR390){
        if(!LTR390.begin()) Serial.print("\n\nLTR390 Not Found"); SENSOR_LTR390 = false;
        if(1/DATA_RATE > 2) Serial.print("\n\nRefresh Rate Too Fast! Decrease Speed"); while(1) delay(10);
        LTR390.setMode(LTR390_MODE_UVS);
        LTR390.setGain(LTR390_GAIN_18);
        LTR390.setResolution(LTR390_RESOLUTION_20BIT);
        LTR390.setThresholds(100, 1000);
    }
    if(SENSOR_MAX17048){
        if(!MAX17048.begin()) Serial.print("\n\nMAX17048 Not Found"); SENSOR_MAX17048 = false;
        MAX17048.setAlertVoltages(2.0, 4.2);
    }
    if(SENSOR_SCD40){
        SCD40.begin(Wire);
        delay(10);
        if(!SCD40.startPeriodicMeasurement()) Serial.print("\n\nSCD40 Not Found"); SENSOR_SCD40 = false;
        SCD40.startPeriodicMeasurement();
    }
    if(SENSOR_SHT41){
        if(!SHT41.begin()) Serial.print("\n\nSHT41 Not Found"); SENSOR_SHT41 = false;
        SHT41.setPrecision(SHT4X_HIGH_PRECISION);
        SHT41.setHeater(SHT4X_NO_HEATER);
    }
    if(SENSOR_VEML7700){
        if(!VEML7700.begin()) Serial.print("\n\nVEML7700 Not Found"); SENSOR_VEML7700 = false;
    }
}

void INITIALIZE_TFT(){
    pinMode(TFT_BACKLITE, OUTPUT);
    pinMode(TFT_I2C_POWER, OUTPUT);

    digitalWrite(TFT_BACKLITE, HIGH);
    digitalWrite(TFT_I2C_POWER, HIGH);

    TFT.init(135, 240); // Define TFT resolution
    TFT.setRotation(3); // CCW orientation
    TFT.fillScreen(ST77XX_BLACK); // Clear screen
}

float GET_SENSOR_DATA(bool SENSOR_ENS160, bool SENSOR_GUVA_B, bool SENSOR_LPS22, bool SENSOR_LTR390, bool SENSOR_MAX17048, bool SENSOR_SCD40, bool SENSOR_SHT41, bool SENSOR_VEML7700){
    if(SENSOR_ENS160){
        if(ENS160.available()){
            ENS160.measure(true);
            ENS160.measureRaw(true);

            float ENS160_AQI = ENS160.getAQI();
            float ENS160_eCO2 = ENS160.geteCO2();
            float ENS160_HP0 = ENS160.getHP0();
            float ENS160_HP1 = ENS160.getHP1();
            float ENS160_HP2 = ENS160.getHP2();
            float ENS160_HP3 = ENS160.getHP3();
        }
    }
    if(SENSOR_GUVA_B){

    }
    if(SENSOR_LPS22){
        sensors_event_t LPS22_TEMPERATURE, LPS22_PRESSURE;
        LPS22.getEvent(&LPS22_PRESSURE, &LPS22_TEMPERATURE);
    }
    if(SENSOR_LTR390){
        if(LTR390.newDataAvailable()){
            float LTR390_RAW_UV = LTR390.readUVS();
            float LTR390_UVI = LTR390_RAW_UV / 2300;
        }
    }
    if(SENSOR_MAX17048){
        float MAX17048_VOLTAGE = MAX17048.cellVoltage();
        float MAX17048_PERCENTAGE = MAX17048.cellPercent();
        float MAX17048_CHARGE_RATE = MAX17048.chargeRate();
    }
    if(SENSOR_SCD40){
        bool isDataReady = false;
        if(!SCD40.getDataReadyFlag(isDataReady)){
            uint16_t SCD40_CO2;
            float SCD40_TEMPERATURE, SCD40_HUMIDITY;
            uint16_t error = SCD40.readMeasurement(SCD40_CO2, SCD40_TEMPERATURE, SCD40_HUMIDITY);
            if(error){
                float SCD40_CO2 = 0;
                float SCD40_TEMPERATURE = 0;
                float SCD40_HUMIDITY = 0;
            }
            else{
                float SCD40_CO2;
                float SCD40_TEMPERATURE;
                float SCD40_HUMIDITY;
            }
        }
    }
}
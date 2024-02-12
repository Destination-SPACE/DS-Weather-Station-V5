#include "DS_11xx.h"

void PROM::begin(int t_e, int pin)
{
    EEPROM_PIN = pin;
    pinMode(EEPROM_PIN, INPUT);
    EEPROM_ADDR = addr;                         // Define the device address for the EEPROM
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 10, true);
    timerAlarmEnable(timer);
}

void PROM::setAddress(uint8_t addr)
{
    EEPROM_ADDR = addr;     // Define the device address for the EEPROM

}

void PROM::readData(uint16_t DAT_ADDR)
{
    uint8_t receivedData = receiveData();
}





//PRIVATE FUNCTIONS

uint8_t PROM::receiveData()     //Read from EEPROM
{
    noInterrupts();             // Disable interrupts
    reading = true;             // Initialize reading
    uint8_t receivedData = 0;

    for(int i = 0; i < 8; i++){
        timerAlarmWrite(timer, T_E, true);      // Set alarm for the bit-period

        while(!receivedBit){                    // Wait until data is ready to be read
            delayMicroseconds(1);
        }

        receivedData |= (receivedBit << i);     // Add bit to the data byte
        receivedBit = false;                    // Reset receivedBit flag
    }

    reading = false;    // Reset reading flag
    interrupts();       // Re-enable interrupts
    
    return receivedData;
}

void PROM::transmitData(uint8_t data)
{
    
}

void PROM::standbyPulse()           // Send a standby pulse
{
    noInterrupts();                 // Disable interrupts
    reading = false;                // Indicate transmitting

    digitalWrite(EEPROM_PIN, HIGH);         // Set the line high
    timerAlarmWrite(timer, T_STBY, false);  // Set the timer to expire after it finishes
    timerAlarmEnable(timer);                // Enable the timer
    digitalWrite(EEPROM_PIN, LOW);          // Set the line low

    Interrupts();                   // Re-enable interrupts
}

void PROM::startHeader()            // Send the start header
{
    noInterrupts();                 // Disable interrupts
    reading = false;                // Indicate transmitting

    digitalWrite(EEPROM_PIN, LOW);          // Set the line low
    timerAlarmWrite(timer, T_HDR, false);   // Set the timer to expire after it finishes
    timerAlarmEnable(timer);                // Enable the timer
    
    transmitBit = false;                    // Start with a low start bit
    for(int i = 0; i < 8; i++){
        timerAlarmWrite(timer, T_E, true);  // Set alarm for the bit-period
        while(timerAlarmEnabled(timer));    // Wait for the timer to expire
        
        transmitBit = (ST_HDR >> i) & 0x01; // Move to the next bit
    }

    reading = true;         // Reset reading flag
    interrupts();           // Re-enable interrupts    
}

bool PROM::acknowlege(bool ack)     // Send an acknowlegment bit
{
    noInterrupts();                 // Disable interrupts
    reading = false;                // Indicate transmitting

    digitalWrite(EEPROM_PIN, ack);      // Send the acknowlegment bit
    timerAlarmWrite(timer, T_E, false); // Set the timer to expire after it finishes
    timerAlarmEnable(timer);            // Enable the time
    digitalWrite(EEPROM_PIN, LOW);      // Pull the pin low

    reading = true;
    //NEED TO INSERT SOMETHING TO CHECK FOR SAK/noSAK
    SAK = digitalRead(EEPROM_PIN);

    return SAK;
}

void PROM::sendDeviceAddress()
{
    noInterrupts();
    reading = false;

    transmitBit = false;
    for(int i = 0; i < 8; i++){
        timerAlarmWrite(timer, T_E, true);
        while(timerAlarmEnabled(timer));

        transmitBit = (EEPROM_ADDR >> i) & 0x01;
    }

    reading = true;
    interrupts();
}

void PROM::sendCommand()
{

}

void IRAM_ATTR onTimer()
{
    if(reading){
        receivedBit = digitalRead(EEPROM_PIN);
    }
    else{
        digitalWrite(EEPROM_PIN, transmitBit);
        transmitBit = !transmitBit;
    }
}
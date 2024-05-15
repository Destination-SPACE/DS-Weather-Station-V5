//#ifdef

#include <Arduino.h>
#include <esp32-hal-timer.h>

#define EEPROM_PIN 10       // Set the EEPROM GPIO to digital pin 10
#define EEPROM_ADDR 0x0A    // Default address for the EEPROM (0xA1 alternate)

//Instruction Commands
uint8_t READ = 0x03;    // Read data from memory array beginning at specified address
uint8_t CRRD = 0x06;    // Read data from current location in memory array
uint8_t WRITE = 0x6C;   // Write data to memory array begining at specified address
uint8_t WREN = 0x96;    // Set the write enable latch (enable write operations)
uint8_t WRDI = 0x91;    // Reset the write enable latch (disable write operations)
uint8_t RDSR = 0x05;    // Read STATUS register
uint8_t WRSR = 0x6E;    // Write STATUS register
uint8_t ERAL = 0x6D;    // Write '0x00' to entire array
uint8_t SETAL = 0x67;   // Write '0xFF' to entire array

//Timing (in microseconds)
int F_BUS = 20;         // Serial bus frequency (kHz)
int T_E = 50;           // Bit period
int T_STBY = 600;       // Standby pulse time
int T_SS = 10;          // Start header setup time
int T_HDR = 5;          // Start header low pulse time
int T_SP = 0;           // Input filter spike suppression (SCIO)
int T_WC_WRSR = 5000;   // Write cycle time (Write, WRSR commands)
int T_WC_ERAL = 10000;  // Write cycle time (ERAL, SETAL commands)

//Data Transfer Codes
uint8_t ST_HDR = 0x55;  // Start header
bool MAK = 1;           // Master acknowlege bit
bool noMAK = 0;         // Master non-acknowlege bit (used to terminate current operation)
bool SAK;               // Slave acknowlege bit

//Timer Definitions
hw_timer_t *timer = NULL;
volatile bool transmitBit = false;
volatile bool reading = false;
volatile bool receivedBit = false;

class PROM
{
    public:
        void begin(int);    // Set bit-period


        void setAddress(uint8_t addr);  // Define the slave address of the EEPROM
        
        uint8_t readData(uint16_t DAT_ADDR);    // Read data from an address
        uint8_t currentAddressReadData(void);   // Reads data from the last memory array location accessed
        uint8_t writeData(uint16_t DAT_ADDR);   // Write data to an address

        void writeEnable(void);     // Enable write operations
        void writeDisable(void);    // Disable write operations

        uint8_t readStatus(uint8_t DAT_STS);    // Read from the status register
        uint8_t writeStatus(uint8_t DAT_STS);   // Write address protection bits

        void eraseAll(void);    // Write '0x00' to the entire memory array
        void setAll(void);      // Write '0xFF' to the entire memory array

    private:
        uint8_t receiveData(void);          // Read data from the data line
        void transmitByte(uint8_t data);    // Transmit data on the data line

        void standbyPulse(void);            // Transmit a standby pulse
        void startHeader(void);             // Transmit the start header
        bool acknowledge(bool ack);         // Send an acknowlegment bit
        void sendDeviceAddress(void);       // Send the device address
        void sendCommand(uint8_t cmd);      // Send command to the EEPROM


};

#endif
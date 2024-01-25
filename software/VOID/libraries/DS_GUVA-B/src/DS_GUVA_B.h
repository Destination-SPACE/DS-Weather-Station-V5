#ifdef DS_GUVA_B_H
#define DS_GUVA_B_H

#include <Arduino.h>
#include <Wire.h>

//I2C address for ADC
#define ADS_I2CADDR_DEFAULT     0x1E
#define ADS_I2CADDR_ALTERNATE   0x18

//Driver for the Destination SPACE GUVA/GUVB breakout board
class DS_GUVA_B {
    public:
        DS_GUVA_B(uint8_t address = ADS_I2CADDR_DEFAULT, TwoWire &wirePort = Wire);
        virtual ~DS_GUVA_B();

        bool begin();
        void end();

        bool write(uint8_t reg, uint8_t val);
        bool write(uint8_t val);
        bool read(uint8_t reg, uint8_t *val);
        bool readFirst(uint16_t *val);
        bool read2Ch(uint16_t *CH0, uint16_t *CH1);

    private:
        TwoWire& _wire;
        uint8_t _i2c_address;
        
        //Reset registers
        #define WKEY                    0x17
        #define DEVICE_RESET            0x14

        //Functional mode select registers
        #define OFFSET_CAL              0x15
        #define OPMODE_SEL              0x1C
        #define OPMODE_i2CMODE_STATUS   0x00

        //Input config register
        #define CHANNEL_INPUT_CFG       0x24

        //Analog MUX and sequencer registers
        #define AUTO_SEQ_CHEN           0x20
        #define START_SEQUENCE          0x1E
        #define ABORT_SEQUENCE          0x1F
        #define SEQUENCE_STATUS         0x04

        //Oscillator and timing control registers
        #define OSC_SEL                 0x18
        #define nCLK_SEL                0x19

        //Data buffer control registers
        #define DATA_BUFFER_OPMODE      0x2C
        #define DOUT_FORMAT_CFG         0x28
        #define DATA_BUFFER_STATUS      0x01

        //Acumulator control registers
        #define ACC_EN                  0x30
        #define ACC_CH0_LSB             0x08
        #define ACC_CH0_MSB             0x09
        #define ACC_CH1_LSB             0x0A
        #define ACC_CH1_MSB             0x0B
        #define ACCUMULATOR_STATUS      0x02

        //Digital window comparator registers
        #define ALERT_DWC_EN            0x37
        #define ALERT_CHEN              0x34
        #define DWC_HTH_CH0_MSB         0x39
        #define DWC_HTH_CH0_LSB         0x38
        #define DWC_LTH_CH0_MSB         0x3B
        #define DWC_LTH_CH0_LSB         0x3A
        #define DWC_HYS_CH0             0x40
        #define DWC_HTH_CH1_MSB         0x3D
        #define DWC_HTH_CH1_LSB         0x3C
        #define DWC_LTH_CH1_MSB         0x3F
        #define DWC_LTH_CH1_LSB         0x3E
        #define DWC_HYS_CH1             0x41
        #define PRE_ALT_MAX_EVENT_COUNT 0x36
        #define ALERT_TRIG_CHID         0x03
        #define ALERT_LOW_FLAGS         0x0C
        #define ALERT_HIGH_FLAGS        0x0E

}



#endif
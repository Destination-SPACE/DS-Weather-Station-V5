#include <DS_GUVA_B.h>

DS_GUVA_B::DS_GUVA_B(uint8_t address, TwoWire &wirePort) : _i2c_address(address), _wire(wirePort){}

DS_GUVA_B::~DS_GUVA_B(){}

bool DS_GUVA_B::begin(){
    _wire.begin();
    uint8_t regVal;

    if(!read(OPMODE_i2CMODE_STATUS, &regval)){
        return(false);
    }

    return(true);
}

void DS_GUVA_B::end(){
    _wire.end();
}

bool DS_GUVA_B::write(uint8_t val){
    _wire.beginTransmission(_i2c_address);
    _wire.write(val);
    
    return(_wire.endTransmission() == 0);
}


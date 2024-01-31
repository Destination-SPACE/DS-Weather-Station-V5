#include "DS_11xx.h"

void PROM::begin(int t_e, int pin)
{
    pinMode(pin, INPUT);
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 10, true);
    timerAlarmEnable(timer);
}








void IRAM_ATTR onTimer(){}
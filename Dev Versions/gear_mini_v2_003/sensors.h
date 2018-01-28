#ifndef SENSORS_H
#define SENSORS_H

#include "Arduino.h"

void initialiseADC();
void readAnalog(uint8_t AinCH);
void readDigitalIn(uint8_t DinCH);
void getAnalogBYoffset(uint8_t AinCH, uint8_t readloop);

unsigned int tempReading;
    
#endif

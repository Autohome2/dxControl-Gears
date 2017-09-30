#ifndef STORAGE_H
#define STORAGE_H

#include <EEPROM.h>

void writeConfig();
void loadConfig();
uint8_t NVMEMread(uint16_t address);
void NVMEMwrite(uint16_t address, uint8_t dataByte);

/*
Current layout of EEPROM data (Version 1) is as follows (All sizes are in bytes):
internal memory sizes
nano == 1K
uno == 
meag2560 == 
teensy3.2 == 
teensy3.5 ==
stm32103 == 0 ! external
|---------------------------------------------------|
|Byte # |Size | Description                         |
|---------------------------------------------------|
| 0     |1    | Data structure version              |
| 10    |128  | configPage1
| 200   |256  | configpage2
| 900   |999  | gearboxconfig and pinconfig
*/

#define EEPROM_CONFIG1_START 10
#define EEPROM_CONFIG1_END 138     // 128 slots
#define EEPROM_CONFIG2_START 200
#define EEPROM_CONFIG2_END 456     //256 slots

byte EEPROM_loc;

#endif // STORAGE_H

/* 
 *  
 *  GearControl V0.001
GrearControl is a Transmission controller based on mini gpio for speeduino and can be used standalone or in conjunction with the Speeduino open source ECU 
Copyright (C) Darren Siepka
A full copy of the license may be found in the projects root directory 

Speeduino is Copyright (C) Josh Stewart
A full copy of the license may be found in the speeduino projects root directory

*/
//https://developer.mbed.org/handbook/C-Data-Types
#include <stdint.h>
//************************************************
#include "globals.h"
#include "utils.h"
#include "comms.h"
#include "auxiliaries.h"
#include "sensors.h"
#include "directcomms.h"
#include "storage.h"
#include "timers.h"
#include "display.h"
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>

// setup which serial port connects to the speeduino
#if defined(ARDUINO_AVR_MEGA2560)
  // mega pin assignments
  #define SERIALLink Serial3
  
#elif defined(ARDUINO_AVR_PRO)
  // Pro Mini assignments
  #include <SoftwareSerial.h>
SoftwareSerial mysoftSerial(A4, A5);  //(10, 11);  // RX, Tx   connect to Serial3 on the Speeduino Mega
 #define SERIALLink mysoftSerial

#elif defined(ARDUINO_Nucleo_64)//(CORE_STM32)
  #define SERIALLink Serial2 
#endif
  
/*
 Note for softSerial config:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
*/

struct config1 configPage1;
struct config2 configPage2;
struct statuses currentStatus;
volatile int mainLoopCount;

  // U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2_1(U8G2_R0, /* cs=*/ 7, /* dc=*/ 6, /* reset=*/ 8);
   U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI u8g2_0(U8G2_R0, /* cs=*/ 7, /* dc=*/ 6, /* reset=*/ 8);

void setup() {
  // put your setup code here, to run once:
 loadConfig();
 setGearboxConfig();
 setPinMapping(configPage1.pinLayout);
 setUsageflags();

 configure_display_type();
//initialise
 initialiseADC();
 initialiseTimers();
    
 Serial.begin(115200);       // your console serial port
 SERIALLink.begin(115200);  

  mainLoopCount = 0;
  BIT_SET(currentStatus.systembits, 0); //set system ready flag
  initialOutputs();   //run the initialoutputs once to begin to preset initial on states 
               //     currentStatus.dev1 = boxOutput[1];//gear_port_Enabled[1];
               //   currentStatus.dev2 = BIT_CHECK(configPage1.FunctionActive, (0));//boxOutput[2];//gear_port_Enabled[2];
currentStatus.digOut = 0;               
}

      
void loop() {
  // put your main code here, to run repeatedly:
      mainLoopCount++;
    
      //Check for any requets from serial. Serial operations are checked under 2 scenarios:
      // 1) Every 64 loops (64 Is more than fast enough for TunerStudio). This function is equivalent to ((loopCount % 64) == 1) but is considerably faster due to not using the mod or division operations
      // 2) If the amount of data in the serial buffer is greater than a set threhold (See globals.h). This is to avoid serial buffer overflow when large amounts of data is being sent
      if ( ((mainLoopCount & 31) == 1) or (SERIALLink.available() > 32) )
      {
          if (SERIALLink.available() > 0)      // if SERIALLink has data then do the command subroutine
        {
          command();
        }
      }     
        if ( ((mainLoopCount & 31) == 1) or (Serial.available() > 32) )
          {
            if (Serial.available() > 0)      // if Serial has data then do the command subroutine
              {
                direct_serial_command();
              }
          }
        
       if ((mainLoopCount & 31) == 1) //Every 32 loops read analog and digital inputs
            {
              for (byte Achan = 1; Achan <17 ; Achan++)
                  {
                    if (pinAin[Achan] < 255) {readAnalog(Achan);}        // read the analog inputs
                  }

              if(!BIT_CHECK(currentStatus.testIO_hardware, 1))
                {
                  for (byte Dinchan = 1; Dinchan <17 ; Dinchan++)
                      {
                        if (pinIn[Dinchan] < 255) { readDigitalIn(Dinchan);}        // if pin is not set to 0 then is in use so read the digital input
                      }
                }          
            }

       if ((mainLoopCount & 31) == 1) //Every 32 loops do outputs
            {
            driveOutputs();
            selectorInput(); 
            gearStatus();
            gearOutput();
            }
            
       if ((mainLoopCount & 63) == 1) //Every 32 loops do outputs
            {
            update_display();
            }
                               
}




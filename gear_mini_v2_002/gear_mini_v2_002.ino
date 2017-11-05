/* 
 *  
 *  GearControl V2.002
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
#include "remotecomms.h"
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>

//configure spi memory options

#if defined CORE_STM32//(MCU_STM32F103C8)
    #if defined (USE_EXT_FRAM)
        #include "Adafruit_FRAM_SPI.h"
        /* Example code for the Adafruit SPI FRAM breakout */
        uint8_t FRAM_CS = PA15;//10;
        //Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_CS);  // use hardware SPI
        uint8_t FRAM_SCK= PB3;//13;
        uint8_t FRAM_MISO = PB4;//12;
        uint8_t FRAM_MOSI = PB5;//11;
        //Or use software SPI, any pins!
        Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_SCK, FRAM_MISO, FRAM_MOSI, FRAM_CS);
    #endif
    
    #if defined (USE_EXT_FLASH)
        #include <SPI.h>
        #include "stm32_flash.h"
        SPIClass SPI_2(2); //Create an instance of the SPI Class called SPI_2 that uses the 2nd SPI Port   
    #endif

    #if defined (USE_EXT_EEPROM)
        #include <SPI.h>
        #include "stm32_ext_eeprom.h"
        SPIClass SPI_2(2); //Create an instance of the SPI Class called SPI_2 that uses the 2nd SPI Port   
  #endif
#endif
  
// setup which serial port connects to the speeduino
#if defined(ARDUINO_AVR_MEGA2560)
    // mega pin assignments
    #define SERIALLink Serial3
  
#elif defined(ARDUINO_AVR_PRO)
    // Pro Mini assignments
    #include <SoftwareSerial.h>
    SoftwareSerial mysoftSerial(A4, A5);  //(10, 11);  // RX, Tx   connect to Serial3 on the Speeduino Mega
    /*
    Note for softSerial config:
    Not all pins on the Mega and Mega 2560 support change interrupts,
    so only the following can be used for RX:
    10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

    Not all pins on the Leonardo and Micro support change interrupts,
    so only the following can be used for RX:
    8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
    */
    #define SERIALLink mysoftSerial

#elif defined (CORE_STM32)
      #if defined(MCU_STM32F103C8)
         #define SERIALLink Serial2
      #endif
      
#elif defined(ARDUINO_Nucleo_64)//(CORE_STM32)
    #define SERIALLink Serial2 
#endif

//setup which serial port connects to TS
#if defined(ARDUINO_AVR_MEGA2560)
    HardwareSerial &CONSOLE_SERIALLink = Serial;
    //Serial.begin(115200);       // your console serial port 
    //Serial3.begin(115200);          //the external serial stream port

#elif defined (CORE_STM32)
      #if defined(MCU_STM32F103C8)
          #define CONSOLE_SERIALLink Serial1 
      #endif    

#elif defined(ARDUINO_Nucleo_64)//(CORE_STM32)
    #define SERIALLink Serial1 
#endif  

struct config1 configPage1;
struct config2 configPage2;
struct config3 configPage3;
struct statuses currentStatus;

volatile int mainLoopCount;
volatile byte Achanloop;
volatile byte diginchanloop;
//volatile byte driveoutloop;
volatile byte EXinchanloop;

//configure display initialise options
#if defined USE_SSD1106
   U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2_1(U8G2_R0, /* cs=*/ DISPLAY_CS, /* dc=*/ DISPLAY_DC, /* reset=*/ DISPLAY_RESET);   
#endif  
#if defined USE_SSH1106
   U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI u8g2_0(U8G2_R0, /* cs=*/ DISPLAY_CS, /* dc=*/ DISPLAY_DC, /* reset=*/ DISPLAY_RESET);    
#endif

void setup() {
  // put your setup code here, to run once:
#if defined (CORE_STM32)
    #if defined(USE_EXT_FLASH)
    init_stm32_flash(1);
    CONSOLE_SERIALLink.print("ok");
    #endif

    #if defined(USE_EXT_EEPROM)
    init_stm32_ext_eeprom(1);
    #endif
#endif
    
 loadConfig();
 setGearboxConfig();
 setPinMapping(configPage1.pinLayout);
 setUsageflags();

 configure_display_type();
//initialise
 initialiseADC();
 initialiseTimers();
    
 CONSOLE_SERIALLink.begin(115200);       // your console serial port
 SERIALLink.begin(115200);               // the serial port connecting to Speeduino 
 
  mainLoopCount = 0;
  BIT_SET(currentStatus.systembits, 0); //set system ready flag
  initialOutputs();   //run the initialoutputs once to begin to preset initial on states 
      
  currentStatus.digOut = 0;               
}

      
void loop() {
  // put your main code here, to run repeatedly:
      mainLoopCount++;
      LOOP_TIMER = TIMER_mask;
      //Check for any requests from serial. Serial operations are checked under 2 scenarios:
      // 1) Every 64 loops (64 Is more than fast enough for TunerStudio). This function is equivalent to ((loopCount % 64) == 1) but is considerably faster due to not using the mod or division operations
      // 2) If the amount of data in the serial buffer is greater than a set threhold (See globals.h). This is to avoid serial buffer overflow when large amounts of data is being sent
     
      if (BIT_CHECK(TIMER_mask, BIT_TIMER_25HZ))
        {
          if (SERIALLink.available() > 0)      // if SERIALLink has data then do the remote serial command subroutine
            {
              remote_serial_command();
            }

          if (CONSOLE_SERIALLink.available() > 0)      // if Serial has data then do the direct serial command subroutine(Typical TS link)
            {
              direct_serial_command();
            }
          BIT_CLEAR(TIMER_mask, BIT_TIMER_25HZ);
        }
          
      if  (SERIALLink.available() > 32)         //untimed serial check in case of buffer overflow
        {
          if (SERIALLink.available() > 0)      // if SERIALLink has data then do the remote serial command subroutine
            {
              remote_serial_command();
            }
        }     
        
     if  (CONSOLE_SERIALLink.available() > 32)          //untimed serial check in case of buffer overflow
        {
          if (CONSOLE_SERIALLink.available() > 0)      // if Serial has data then do the direct serial command subroutine(Typical TS link)
            {
              direct_serial_command();
            }
        }
                      
     if(BIT_CHECK(LOOP_TIMER, BIT_TIMER_4HZ)) //4 hertz
        {
          //Nothing here currently
          BIT_CLEAR(TIMER_mask, BIT_TIMER_4HZ);                         
        }

     if(BIT_CHECK(LOOP_TIMER, BIT_TIMER_10HZ)) //10 hertz
        {
          //Nothing here currently
          BIT_CLEAR(TIMER_mask, BIT_TIMER_10HZ);                         
        }
        
     if(BIT_CHECK(LOOP_TIMER, BIT_TIMER_15HZ)) //15 hertz
        {
          //Nothing here currently
          BIT_CLEAR(TIMER_mask, BIT_TIMER_15HZ);
          for (byte Achan = 1; Achan <17 ; Achan++)
             {
              if (pinAin[Achan] < 255) {readAnalog(Achan);}        // if analog pin is in use read the analog inputs
             }    

          if(!BIT_CHECK(currentStatus.testIO_hardware, 1))
             {
              for (byte Dinchan = 1; Dinchan <17 ; Dinchan++)
                 {
                  if (pinIn[Dinchan] < 255) { readDigitalIn(Dinchan);}        // if pin is not set to 0 then is in use so read the digital input
                 }
             }          

          driveOutputs();
          selectorInput(); 
          gearStatus();
          gearOutput();
          update_display();                                   
        }
        
     if(BIT_CHECK(LOOP_TIMER, BIT_TIMER_30HZ)) //30 hertz
       {
        //Nothing here currently
        BIT_CLEAR(TIMER_mask, BIT_TIMER_30HZ);                         
       }

}        

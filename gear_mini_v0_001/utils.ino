/*
  Returns how much free dynamic memory exists (between heap and stack)
*/
#include "utils.h"

int freeRam ()
{
//#if defined(CORE_AVR)
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
//#elif defined(CORE_TEENSY)
//  uint32_t stackTop;
//  uint32_t heapTop;

  // current position of the stack.
//  stackTop = (uint32_t) &stackTop;

  // current position of heap.
//  void* hTop = malloc(1);
//  heapTop = (uint32_t) hTop;
//  free(hTop);

  // The difference is the free, available ram.
//  return (uint16_t)stackTop - heapTop;
//#elif defined(CORE_STM32)
  //Figure this out some_time
//  return 0;
//#endif
}

void setGearboxConfig()
{
  uint8_t gearID = configPage1.gearbox_type & B00011111;  //need this filter to block out the unsed bits which could be randomly set
    switch (gearID)
    {
      case 0: //A340E
            //outputs
            shiftsolenoid[1] = 1;            // shift solenoid active == 1 , pwm == 100, and  not used == 255
            shiftsolenoid[2] = 1;
            shiftsolenoid[3] = 255;
            shiftsolenoid[4] = 255;
            shiftsolenoid[5] = 255;
            shiftsolenoid[6] = 255;
            shiftsolenoid[7] = 255;
            shiftsolenoid[8] = 255;
            lockup_solenoid[1] = 1;     //
            lockup_solenoid[2] = 255;     // lockup solenoid pin active
            starter_inhibit = 1;        //inhibit relay use enable/disable
            spareOut[12] = 255;
            spareOut[13] = 255;
            spareOut[14] = 255;
            spareOut[15] = 255;
            spareOut[16] = 255;
            spareOut[17] = 255;
            spareOut[18] = 255;
            spareOut[19] = 255;
            spareOut[20] = 255;
            spareOut[21] = 255;
            spareOut[22] = 255;
            spareOut[23] = 255;
            spareOut[24] = 255;
            spareOut[25] = 255;
            spareOut[26] = 255;
            spareOut[27] = 255;
            spareOut[28] = 255;
            spareOut[29] = 255;
            spareOut[30] = 255;
            spareOut[31] = 255;
            spareOut[32] = 255;
            //shift pattern
            gear[1] = 1;                // solenoid pattern eg 1 is only shiftsolenoid1
            gear[2] = 3;                // solenoid pattern eg 3 is both shiftsolenoid1 and solenoid2
            gear[3] = 2;                // shiftsolenoid pattern eg 2 is only solenoid2
            gear[4] = 255;
            gear[5] = 255;
            gear[6] = 255;
            gear[7] = 255;
            gear[8] = 255;   
            rev_gear = 1;

      break;

      case 1: //AODE
            shiftsolenoid[1] = 1;            // shift solenoid active == 1 else not used == 255
            shiftsolenoid[2] = 1;
            shiftsolenoid[3] = 1;
            shiftsolenoid[4] = 1;
            shiftsolenoid[5] = 255;
            shiftsolenoid[6] = 255;
            shiftsolenoid[7] = 255;
            shiftsolenoid[8] = 255;
            lockup_solenoid[1] = 1;     //
            lockup_solenoid[2] = 255;     // lockup solenoid pin active
            starter_inhibit = 1;        //inhibit relay use enable/disable
            spareOut[12] = 255;
            spareOut[13] = 255;
            spareOut[14] = 255;
            spareOut[15] = 255;
            spareOut[16] = 255;
            spareOut[17] = 255;
            spareOut[18] = 255;
            spareOut[19] = 255;
            spareOut[20] = 255;
            spareOut[21] = 255;
            spareOut[22] = 255;
            spareOut[23] = 255;
            spareOut[24] = 255;
            spareOut[25] = 255;
            spareOut[26] = 255;
            spareOut[27] = 255;
            spareOut[28] = 255;
            spareOut[29] = 255;
            spareOut[30] = 255;
            spareOut[31] = 255;
            spareOut[32] = 255;
            gear[1] = 1;                // solenoid pattern eg 1 is only shiftsolenoid1
            gear[2] = 3;                // solenoid pattern eg 3 is both shiftsolenoid1 and solenoid2
            gear[3] = 2;                // shiftsolenoid pattern eg 2 is only solenoid2
            gear[4] = 0;
            gear[5] = 255;
            gear[6] = 255;
            gear[7] = 255;
            gear[8] = 255;   
            rev_gear = 1;
                              
      break;

      case 2: //5R55s
            //outputs
            shiftsolenoid[1] = 1;            // shift shiftsolenoid active == 1 else not used == 255
            shiftsolenoid[2] = 1;
            shiftsolenoid[3] = 1;
            shiftsolenoid[4] = 1;
            shiftsolenoid[5] = 1;
            shiftsolenoid[6] = 255;
            shiftsolenoid[7] = 255;
            shiftsolenoid[8] = 255;
            lockup_solenoid[1] = 1;     //
            lockup_solenoid[2] = 255;     // lockup solenoid pin active
            starter_inhibit = 1;        //inhibit relay use enable/disable
            spareOut[12] = 255;
            spareOut[13] = 255;
            spareOut[14] = 255;
            spareOut[15] = 255;
            spareOut[16] = 255;
            spareOut[17] = 255;
            spareOut[18] = 255;
            spareOut[19] = 255;
            spareOut[20] = 255;
            spareOut[21] = 255;
            spareOut[22] = 255;
            spareOut[23] = 255;
            spareOut[24] = 255;
            spareOut[25] = 255;
            spareOut[26] = 255;
            spareOut[27] = 255;
            spareOut[28] = 255;
            spareOut[29] = 255;
            spareOut[30] = 255;
            spareOut[31] = 255;
            spareOut[32] = 255;
            //shift pattern
            gear[1] = 1;                // solenoid pattern 
            gear[2] = 3;                // solenoid pattern 
            gear[3] = 5;                // solenoid pattern 
            gear[4] = 2;
            gear[5] = 4;
            gear[6] = 255;
            gear[7] = 255;
            gear[8] = 255;
            rev_gear = 1;
            
      break;
      
      case 3: //HR16DE 4speed
            //outputs
            shiftsolenoid[1] = 1;            // shift shiftsolenoid active == 1 else not used == 255
            shiftsolenoid[2] = 1;
            shiftsolenoid[3] = 255;
            shiftsolenoid[4] = 255;
            shiftsolenoid[5] = 255;
            shiftsolenoid[6] = 255;
            shiftsolenoid[7] = 255;
            shiftsolenoid[8] = 255;
            lockup_solenoid[1] = 1;     //
            lockup_solenoid[2] = 255;     // lockup solenoid pin active
            starter_inhibit = 1;        //inhibit relay use enable/disable
            spareOut[12] = 255;
            spareOut[13] = 255;
            spareOut[14] = 255;
            spareOut[15] = 255;
            spareOut[16] = 255;
            spareOut[17] = 255;
            spareOut[18] = 255;
            spareOut[19] = 255;
            spareOut[20] = 255;
            spareOut[21] = 255;
            spareOut[22] = 255;
            spareOut[23] = 255;
            spareOut[24] = 255;
            spareOut[25] = 255;
            spareOut[26] = 255;
            spareOut[27] = 255;
            spareOut[28] = 255;
            spareOut[29] = 255;
            spareOut[30] = 255;
            spareOut[31] = 255;
            spareOut[32] = 255;
            //shift pattern
            gear[1] = 3;                // solenoid pattern 
            gear[2] = 2;                // solenoid pattern 
            gear[3] = 0;                // solenoid pattern 
            gear[4] = 1;
            gear[5] = 255;
            gear[6] = 255;
            gear[7] = 255;
            gear[8] = 255;
            rev_gear = 1;
            
      break;
    }
}

void setPinMapping(byte boardID)
{
    switch (boardID)
  {
    case 0:   //pro-mini demo V0.001
      pinOut[1] = 2; //
      pinOut[2] = 3; //
      pinOut[3] = 4; //
      pinOut[4] = 255; //
      pinOut[5] = 255; //
      pinOut[6] = 255; //
      pinOut[7] = 255; //
      pinOut[8] = 255; //
      pinOut[9] = 255; //
      pinOut[10] = 255; //
      pinOut[11] = 255; //
      pinOut[12] = 255; //
      pinOut[13] = 255; //
      pinOut[14] = 255; //
      pinOut[15] = 255; //
      pinOut[16] = 255; //

      pinIn[1] = 5;
      pinIn[2] = 6;
      pinIn[3] = 7;
      pinIn[4] = 8;
      pinIn[5] = 9;
      pinIn[6] = A3;
      pinIn[7] = 255;
      pinIn[8] = 255;
      pinIn[9] = 255;
      pinIn[10] = 255;
      pinIn[11] = 255;
      pinIn[12] = 255;
      pinIn[13] = 255;
      pinIn[14] = 255;
      pinIn[15] = 255;
      pinIn[16] = 255;
             
      pinAin[1] = A0;
      pinAin[2] = A1;
      pinAin[3] = A2;
      pinAin[4] = 255;
      pinAin[5] = 255;
      pinAin[6] = 255;
      pinAin[7] = 255;
      pinAin[8] = 255;
      pinAin[9] = 255;
      pinAin[10] = 255;
      pinAin[11] = 255;
      pinAin[12] = 255;
      pinAin[13] = 255;
      pinAin[14] = 255;
      pinAin[15] = 255;
      pinAin[16] = 255;
      
      break;
      
  #if defined(ARDUINO_AVR_MEGA2560)
      case 1:   // mega2560 demo V0.001
      pinOut[1] = 33; //
      pinOut[2] = 35; //
      pinOut[3] = 37; //
      pinOut[4] = 39; //
      pinOut[5] = 41; //
      pinOut[6] = 43; //
      pinOut[7] = 255; //
      pinOut[8] = 255; //
      pinOut[9] = 255; //
      pinOut[10] = 255; //
      pinOut[11] = 255; //
      pinOut[12] = 255; //
      pinOut[13] = 13; //
      pinOut[14] = 255; //
      pinOut[15] = 255; //
      pinOut[16] = 255; //

      pinIn[1] = 28;
      pinIn[2] = 30;
      pinIn[3] = 32;
      pinIn[4] = 34;
      pinIn[5] = 36;
      pinIn[6] = 38;
      pinIn[7] = 40;
      pinIn[8] = 42;
      pinIn[9] = 44;
      pinIn[10] = 46;
      pinIn[11] = 48;
      pinIn[12] = 255;
      pinIn[13] = 255;
      pinIn[14] = 255;
      pinIn[15] = 255;
      pinIn[16] = 255;
             
      pinAin[1] = A0;
      pinAin[2] = A1;
      pinAin[3] = A2;
      pinAin[4] = A3;
      pinAin[5] = 255;
      pinAin[6] = 255;
      pinAin[7] = 255;
      pinAin[8] = 255;
      pinAin[9] = 255;
      pinAin[10] = 255;
      pinAin[11] = 255;
      pinAin[12] = 255;
      pinAin[13] = 255;
      pinAin[14] = 255;
      pinAin[15] = 255;
      pinAin[16] = 255;
      
      break;
  #endif      

  #if defined(ARDUINO_AVR_MEGA2560)
      case 3:   // mega2560 using Speeduino V0.3 board
      pinOut[1] = 8; //
      pinOut[2] = 9; //
      pinOut[3] = 10; //
      pinOut[4] = 11; //
      pinOut[5] = 28; //
      pinOut[6] = 24; //
      pinOut[7] = 40; //
      pinOut[8] = 36; //
      pinOut[9] = 4; //
      pinOut[10] = 5; //
      pinOut[11] = 6; //
      pinOut[12] = 255; //
      pinOut[13] = 255; //
      pinOut[14] = 255; //
      pinOut[15] = 255; //
      pinOut[16] = 255; //

      pinIn[1] = 12;
      pinIn[2] = 255;
      pinIn[3] = 255;
      pinIn[4] = 255;
      pinIn[5] = 255;
      pinIn[6] = 255;
      pinIn[7] = 255;
      pinIn[8] = 255;
      pinIn[9] = 255;
      pinIn[10] = 255;
      pinIn[11] = 255;
      pinIn[12] = 255;
      pinIn[13] = 255;
      pinIn[14] = 255;
      pinIn[15] = 255;
      pinIn[16] = 255;
             
      pinAin[1] = A0;
      pinAin[2] = A1;
      pinAin[3] = A2;
      pinAin[4] = A3;
      pinAin[5] = A8;
      pinAin[6] = 255;
      pinAin[7] = 255;
      pinAin[8] = 255;
      pinAin[9] = 255;
      pinAin[10] = 255;
      pinAin[11] = 255;
      pinAin[12] = 255;
      pinAin[13] = 255;
      pinAin[14] = 255;
      pinAin[15] = 255;
      pinAin[16] = 255;
      
      break;
  #endif

  #if defined(ARDUINO_AVR_MEGA2560)
      case 4:   // mega2560 using Speeduino V0.4 board
      pinOut[1] = 8; //
      pinOut[2] = 9; //
      pinOut[3] = 10; //
      pinOut[4] = 11; //
      pinOut[5] = 40; //
      pinOut[6] = 38; //
      pinOut[7] = 52; //
      pinOut[8] = 50; //
      pinOut[9] = 4; //
      pinOut[10] = 5; //
      pinOut[11] = 6; //
      pinOut[12] = 7; //
      pinOut[13] = 45; //
      pinOut[14] = 47; //
      pinOut[15] = 255; //
      pinOut[16] = 255; //

      pinIn[1] = 12;
      pinIn[2] = 255;
      pinIn[3] = 255;
      pinIn[4] = 255;
      pinIn[5] = 255;
      pinIn[6] = 255;
      pinIn[7] = 255;
      pinIn[8] = 255;
      pinIn[9] = 255;
      pinIn[10] = 255;
      pinIn[11] = 255;
      pinIn[12] = 255;
      pinIn[13] = 255;
      pinIn[14] = 255;
      pinIn[15] = 255;
      pinIn[16] = 255;
             
      pinAin[1] = A0;
      pinAin[2] = A1;
      pinAin[3] = A2;
      pinAin[4] = A3;
      pinAin[5] = A8;
      pinAin[6] = 255;
      pinAin[7] = 255;
      pinAin[8] = 255;
      pinAin[9] = 255;
      pinAin[10] = 255;
      pinAin[11] = 255;
      pinAin[12] = 255;
      pinAin[13] = 255;
      pinAin[14] = 255;
      pinAin[15] = 255;
      pinAin[16] = 0;
      
      break;
  #endif
  }


//set pinmode states      
      if (pinOut[1] < 255){pinMode(pinOut[1], OUTPUT);}
      if (pinOut[2] < 255){pinMode(pinOut[2], OUTPUT);}
      if (pinOut[3] < 255){pinMode(pinOut[3], OUTPUT);}
      if (pinOut[4] < 255){pinMode(pinOut[4], OUTPUT);}
      if (pinOut[5] < 255){pinMode(pinOut[5], OUTPUT);}
      if (pinOut[6] < 255){pinMode(pinOut[6], OUTPUT);}
      if (pinOut[7] < 255){pinMode(pinOut[7], OUTPUT);}
      if (pinOut[8] < 255){pinMode(pinOut[8], OUTPUT);}
      if (pinOut[9] < 255){pinMode(pinOut[9], OUTPUT);}
      if (pinOut[10] < 255){pinMode(pinOut[10], OUTPUT);}
      if (pinOut[11] < 255){pinMode(pinOut[11], OUTPUT);}
      if (pinOut[12] < 255){pinMode(pinOut[12], OUTPUT);}
      if (pinOut[13] < 255){pinMode(pinOut[13], OUTPUT);}
      if (pinOut[14] < 255){pinMode(pinOut[14], OUTPUT);}
      if (pinOut[15] < 255){pinMode(pinOut[15], OUTPUT);}
      if (pinOut[16] < 255){pinMode(pinOut[16], OUTPUT);}
            
      if (pinIn[1] < 255){pinMode(pinIn[1], INPUT);}
      if (pinIn[2] < 255){pinMode(pinIn[2], INPUT);}
      if (pinIn[3] < 255){pinMode(pinIn[3], INPUT);}
      if (pinIn[4] < 255){pinMode(pinIn[4], INPUT);}
      if (pinIn[5] < 255){pinMode(pinIn[5], INPUT);}
      if (pinIn[6] < 255){pinMode(pinIn[6], INPUT);}
      if (pinIn[7] < 255){pinMode(pinIn[7], INPUT);}
      if (pinIn[8] < 255){pinMode(pinIn[8], INPUT);}
      if (pinIn[9] < 255){pinMode(pinIn[9], INPUT);}
      if (pinIn[10] < 255){pinMode(pinIn[10], INPUT);}
      if (pinIn[11] < 255){pinMode(pinIn[11], INPUT);}
      if (pinIn[12] < 255){pinMode(pinIn[12], INPUT);}
      if (pinIn[13] < 255){pinMode(pinIn[13], INPUT);}
      if (pinIn[14] < 255){pinMode(pinIn[14], INPUT);}
      if (pinIn[15] < 255){pinMode(pinIn[15], INPUT);}
      if (pinIn[16] < 255){pinMode(pinIn[16], INPUT);}
                  
      if (pinAin[1] < 255){ pinMode(pinAin[1], INPUT);}
      if (pinAin[2] < 255){ pinMode(pinAin[2], INPUT);}
      if (pinAin[3] < 255){ pinMode(pinAin[3], INPUT);}
      if (pinAin[4] < 255){ pinMode(pinAin[4], INPUT);}
      if (pinAin[5] < 255){ pinMode(pinAin[5], INPUT);}
      if (pinAin[6] < 255){ pinMode(pinAin[6], INPUT);}
      if (pinAin[7] < 255){ pinMode(pinAin[7], INPUT);}
      if (pinAin[8] < 255){ pinMode(pinAin[8], INPUT);}
      if (pinAin[9] < 255){ pinMode(pinAin[9], INPUT);}
      if (pinAin[10] < 255){ pinMode(pinAin[10], INPUT);}
      if (pinAin[11] < 255){ pinMode(pinAin[11], INPUT);}
      if (pinAin[12] < 255){ pinMode(pinAin[12], INPUT);}
      if (pinAin[13] < 255){ pinMode(pinAin[13], INPUT);}
      if (pinAin[14] < 255){ pinMode(pinAin[14], INPUT);}
      if (pinAin[15] < 255){ pinMode(pinAin[15], INPUT);}
      if (pinAin[16] < 255){ pinMode(pinAin[16], INPUT);}
}

void setUsageflags()
{
//setup pin use flags for digouts
    for(byte p=1; p<17; p++)
        {
        if (pinOut[p] <255)
          {
            BIT_SET(configPage1.DoutchanActive, (p-1));
          }
        else
          {  
            BIT_CLEAR(configPage1.DoutchanActive, (p-1));    
          }
        }

// setup pin use flags for dig ins
    for(byte p=1; p<17; p++)
        {
        if (pinIn[p] < 255)
          {
            BIT_SET(configPage1.DinchanActive, (p-1));
          }
        else
          {  
            BIT_CLEAR(configPage1.DinchanActive, (p-1));    
          }
        }

//seup pin use flags for analog ins
      for(byte p=1; p<17; p++)
        {
        if (pinAin[p] < 255)
          {
            BIT_SET(configPage1.AinchanActive, (p-1));
          }
        else
          {  
            BIT_CLEAR(configPage1.AinchanActive, (p-1));    
          }
        }

//seup gearbox configuration flags for  valid outputs

        if (shiftsolenoid[1] < 255){BIT_SET(configPage1.FunctionActive, 0);}          //if the shiftsolenoid is in use ie !255 then set the functionactive flag so TS knows its used 
          else{BIT_CLEAR(configPage1.FunctionActive, 0);}
          
        if (shiftsolenoid[2] < 255){BIT_SET(configPage1.FunctionActive, 1);}
          else{BIT_CLEAR(configPage1.FunctionActive, 1);}
          
        if (shiftsolenoid[3] < 255){BIT_SET(configPage1.FunctionActive, 2);}
          else{BIT_CLEAR(configPage1.FunctionActive, 2);}
          
        if (shiftsolenoid[4] < 255){BIT_SET(configPage1.FunctionActive, 3);}
          else{BIT_CLEAR(configPage1.FunctionActive, 3);}
          
        if (shiftsolenoid[5] < 255){BIT_SET(configPage1.FunctionActive, 4);}
          else{BIT_CLEAR(configPage1.FunctionActive, 4);}
          
        if (shiftsolenoid[6] < 255){BIT_SET(configPage1.FunctionActive, 5);}
          else{BIT_CLEAR(configPage1.FunctionActive, 5);}
          
        if (shiftsolenoid[7] < 255){BIT_SET(configPage1.FunctionActive, 6);}
          else{BIT_CLEAR(configPage1.FunctionActive, 6);}
          
        if (shiftsolenoid[8] < 255){BIT_SET(configPage1.FunctionActive, 7);}
          else{BIT_CLEAR(configPage1.FunctionActive, 7);}
        
        if (lockup_solenoid[1] < 255){BIT_SET(configPage1.FunctionActive, 8);} else{  BIT_CLEAR(configPage1.FunctionActive, 8); }
        if (lockup_solenoid[2] < 255){BIT_SET(configPage1.FunctionActive, 9);} else{  BIT_CLEAR(configPage1.FunctionActive, 9); }          
        
        if (starter_inhibit < 255){BIT_SET(configPage1.FunctionActive, (10));} else{  BIT_CLEAR(configPage1.FunctionActive, (10)); }
        
        if (spareOut[12] < 255){BIT_SET(configPage1.FunctionActive, (11));} else{  BIT_CLEAR(configPage1.FunctionActive, (11)); }
        if (spareOut[13] < 255){BIT_SET(configPage1.FunctionActive, (12));} else{  BIT_CLEAR(configPage1.FunctionActive, (12)); }
        if (spareOut[14] < 255){BIT_SET(configPage1.FunctionActive, (13));} else{  BIT_CLEAR(configPage1.FunctionActive, (13)); }
        if (spareOut[15] < 255){BIT_SET(configPage1.FunctionActive, (14));} else{  BIT_CLEAR(configPage1.FunctionActive, (14)); }
        if (spareOut[16] < 255){BIT_SET(configPage1.FunctionActive, (15));} else{  BIT_CLEAR(configPage1.FunctionActive, (15)); }
        if (spareOut[17] < 255){BIT_SET(configPage1.FunctionActive_2, (0));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (0)); }
        if (spareOut[18] < 255){BIT_SET(configPage1.FunctionActive_2, (1));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (1)); }
        if (spareOut[19] < 255){BIT_SET(configPage1.FunctionActive_2, (2));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (2)); }
        if (spareOut[20] < 255){BIT_SET(configPage1.FunctionActive_2, (3));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (3)); }
        if (spareOut[21] < 255){BIT_SET(configPage1.FunctionActive_2, (4));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (4)); }
        if (spareOut[22] < 255){BIT_SET(configPage1.FunctionActive_2, (5));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (5)); }
        if (spareOut[23] < 255){BIT_SET(configPage1.FunctionActive_2, (6));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (6)); }
        if (spareOut[24] < 255){BIT_SET(configPage1.FunctionActive_2, (7));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (7)); }
        if (spareOut[25] < 255){BIT_SET(configPage1.FunctionActive_2, (8));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (8)); }
        if (spareOut[26] < 255){BIT_SET(configPage1.FunctionActive_2, (9));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (9)); }
        if (spareOut[27] < 255){BIT_SET(configPage1.FunctionActive_2, (10));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (10)); }
        if (spareOut[28] < 255){BIT_SET(configPage1.FunctionActive_2, (11));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (11)); }
        if (spareOut[29] < 255){BIT_SET(configPage1.FunctionActive_2, (12));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (12)); }
        if (spareOut[30] < 255){BIT_SET(configPage1.FunctionActive_2, (13));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (13)); }
        if (spareOut[31] < 255){BIT_SET(configPage1.FunctionActive_2, (14));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (14)); }
        if (spareOut[32] < 255){BIT_SET(configPage1.FunctionActive_2, (15));} else{  BIT_CLEAR(configPage1.FunctionActive_2, (15)); }
        
//setup use flags for gear active
    for(byte p=1; p < 9; p++)
        {
        if (gear[p] < 255)
          {
            BIT_SET(configPage1.GearActive, p-1);
          }
        else
          {  
            BIT_CLEAR(configPage1.GearActive, p-1);    
          }
        }

//setup gear_port_Enabled array values , gear_port_Enabled uses [0] to [15]
// boxoutput holds the pinout[x] value for the specific output function  
      boxOutput[0] = configPage1.shiftsolenoid_1 & B00011111;
      boxOutput[1] = configPage1.shiftsolenoid_2 & B00011111;
      boxOutput[2] = configPage1.shiftsolenoid_3 & B00011111;
      boxOutput[3] = configPage1.shiftsolenoid_4 & B00011111;
      boxOutput[4] = configPage1.shiftsolenoid_5 & B00011111;
      boxOutput[5] = configPage1.shiftsolenoid_6 & B00011111;
      boxOutput[6] = configPage1.shiftsolenoid_7 & B00011111;
      boxOutput[7] = configPage1.shiftsolenoid_8 & B00011111;
      boxOutput[8] = configPage1.lockup_solenoid_1 & B00011111;
      boxOutput[9] = configPage1.lockup_solenoid_2 & B00011111;
      boxOutput[10] = configPage1.starter_inhibit_out & B00011111;
      boxOutput[11] = configPage1.boxOutput_11 & B00011111; 
      boxOutput[12] = configPage1.boxOutput_12 & B00011111; 
      boxOutput[13] = configPage1.boxOutput_13 & B00011111; 
      boxOutput[14] = configPage1.boxOutput_14 & B00011111; 
      boxOutput[15] = configPage1.boxOutput_15 & B00011111; 
      
    //  for (byte ge = 1; ge < 17 ; ge++)         //ge = 1 is start value , ge < 17 is one more thnan last value
    //      {
          //  byte bo = boxOutput[ge] &B00011111;
    //         if(BIT_CHECK(configPage1.FunctionActive, (ge-1)) == 1){gear_port_Enabled[(boxOutput[ge])] = 1; }
    //      }

            if(BIT_CHECK(configPage1.FunctionActive, (0)) == 1){gear_port_Enabled[(boxOutput[0])] = 1; }
          
            if(BIT_CHECK(configPage1.FunctionActive, (1)) == 1){gear_port_Enabled[(boxOutput[1])] = 1; }
            
            if(BIT_CHECK(configPage1.FunctionActive, (2)) == 1){gear_port_Enabled[(boxOutput[2])] = 1; }
           
            if(BIT_CHECK(configPage1.FunctionActive, (3)) == 1){gear_port_Enabled[(boxOutput[3])] = 1; }
         
            if(BIT_CHECK(configPage1.FunctionActive, (4)) == 1){gear_port_Enabled[(boxOutput[4])] = 1; }
           
            if(BIT_CHECK(configPage1.FunctionActive, (5)) == 1){gear_port_Enabled[(boxOutput[5])] = 1; }
         
            if(BIT_CHECK(configPage1.FunctionActive, (6)) == 1){gear_port_Enabled[(boxOutput[6])] = 1; }
           
            if(BIT_CHECK(configPage1.FunctionActive, (7)) == 1){gear_port_Enabled[(boxOutput[7])] = 1; }
           
            if(BIT_CHECK(configPage1.FunctionActive, (8)) == 1){gear_port_Enabled[(boxOutput[8])] = 1; }
           
            if(BIT_CHECK(configPage1.FunctionActive, (9)) == 1){gear_port_Enabled[(boxOutput[9])] = 1; }

            if(BIT_CHECK(configPage1.FunctionActive, (10)) == 1){gear_port_Enabled[(boxOutput[10])] = 1; }

            activestate[0] = configPage1.shiftsolenoid_1 & B00100000;
            activestate[1] = configPage1.shiftsolenoid_2 & B00100000;
            activestate[2] = configPage1.shiftsolenoid_3 & B00100000;
            activestate[3] = configPage1.shiftsolenoid_4 & B00100000;
            activestate[4] = configPage1.shiftsolenoid_5 & B00100000;
            activestate[5] = configPage1.shiftsolenoid_6 & B00100000;
            activestate[6] = configPage1.shiftsolenoid_7 & B00100000;
            activestate[7] = configPage1.shiftsolenoid_8 & B00100000;
            activestate[8] = configPage1.lockup_solenoid_1 & B00100000;
            activestate[9] = configPage1.lockup_solenoid_2 & B00100000;
            activestate[10] = configPage1.starter_inhibit_out & B00100000;

}



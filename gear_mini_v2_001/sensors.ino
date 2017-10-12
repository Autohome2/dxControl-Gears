#include "sensors.h"

void initialiseADC()
{
   //This sets the ADC (Analog to Digitial Converter) to run at 1Mhz, greatly reducing analog read times (MAP/TPS) when using the standard analogRead() function
    //1Mhz is the fastest speed permitted by the CPU without affecting accuracy
    //Please see chapter 11 of 'Practical Arduino' (http://books.google.com.au/books?id=HsTxON1L6D4C&printsec=frontcover#v=onepage&q&f=false) for more detail
     BIT_SET(ADCSRA,ADPS2);
     BIT_CLEAR(ADCSRA,ADPS1);
     BIT_CLEAR(ADCSRA,ADPS0); 
}

void readAnalog(uint8_t AinCH)
{
    tempReading = analogRead( pinAin[AinCH]);   //read the adc channel
    tempReading = analogRead( pinAin[AinCH]);   //read it a second time to get a more stable /faster read
    currentStatus.Analog[AinCH] = tempReading;
    
}

void readDigitalIn(uint8_t DinCH)
{
  byte pinread = 0;
  pinread = digitalRead(pinIn[DinCH]);
  if ( pinread == 1)
    {
      BIT_SET(currentStatus.digIn, (DinCH-1));
    }
  else
    {  
      BIT_CLEAR(currentStatus.digIn, (DinCH-1));    
    }
    
//currentStatus.IO  
}

void getAnalogBYoffset(uint8_t AinCH, uint8_t readloop)
{
         switch (AinCH)
              {
              case 17:
                    currentStatus.currentInputvalue[readloop] = currentStatus.Analog[1];
              break;

              case 19:
                    currentStatus.currentInputvalue[readloop] = currentStatus.Analog[2];
              break;
            
              case 21:
                    currentStatus.currentInputvalue[readloop] = currentStatus.Analog[3];
              break;

              case 23:
                    currentStatus.currentInputvalue[readloop] = currentStatus.Analog[4];
              break;
      
              case 25:
                    currentStatus.currentInputvalue[readloop] = currentStatus.Analog[5];
              break;

              case 27:
                    currentStatus.currentInputvalue[readloop] = currentStatus.Analog[6];
              break;
            
              case 29:
                    currentStatus.currentInputvalue[readloop] = currentStatus.Analog[7];
              break;

              case 31:
                    currentStatus.currentInputvalue[readloop] = currentStatus.Analog[8];
              break;
              }  
}


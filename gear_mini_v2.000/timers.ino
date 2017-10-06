/*
Timers are used for having actions performed repeatedly at a fixed interval (Eg every 100ms)
They should not be confused with Schedulers, which are for performing an action once at a given point of time in the future

Timers are typically low resolution (Compared to Schedulers), with maximum frequency currently being approximately every 10ms
*/
#include "timers.h"
#include "globals.h"

#if defined(CORE_AVR)
 // #include <avr/wdt.h>
#endif

void initialiseTimers()
{
//#if defined(CORE_AVR) //AVR chips use the ISR for this
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)|| defined (ARDUINO_AVR_PRO)
   //Configure Timer2 for our low-freq interrupt code.
 //  TCCR2B = 0x00;          //Disbale Timer2 while we set it up
  // TCNT2  = 131;           //Preload timer2 with 131 cycles, leaving 125 till overflow. As the timer runs at 125Khz, this causes overflow to occur at 1Khz = 1ms
 //  TIFR2  = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
 //  TIMSK2 = 0x01;          //Timer2 Set Overflow Interrupt enabled.
 //  TCCR2A = 0x00;          //Timer2 Control Reg A: Wave Gen Mode normal
   /* Now configure the prescaler to CPU clock divided by 128 = 125Khz */
 //  TCCR2B |= (1<<CS22)  | (1<<CS20); // Set bits
 //  TCCR2B &= ~(1<<CS21);             // Clear bit

   //Setup Timer2 to fire every 1ms
  TCCR2B = 0x00;        //Disbale Timer2 while we set it up
  TCNT2  = 130;         //Reset Timer Count to 130 out of 255
  TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
  TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
  TCCR2A = 0x00;        //Timer2 Control Reg A: Wave Gen Mode normal
  TCCR2B = 0x05;        //Timer2 Control Reg B: Timer Prescaler set to 128

   //Enable the watchdog timer for 2 second resets (Good reference: https://tushev.org/articles/arduino/5/arduino-and-watchdog-timer)
   //wdt_enable(WDTO_2S); //Boooooooooo WDT is currently broken on Mega 2560 bootloaders :(

#elif defined (CORE_TEENSY)
   //Uses the PIT timer on Teensy.
   lowResTimer.begin(oneMSInterval, 1000);

#elif defined(CORE_STM32)
  Timer4.setPeriod(1000);  // Set up period
  // Set up an interrupt
  Timer4.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  Timer4.attachInterrupt(1, oneMSInterval);

#endif

}

//Timer2 Overflow Interrupt Vector, called when the timer overflows.
//Executes every ~1ms.
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)|| defined (ARDUINO_AVR_PRO) //AVR chips use the ISR for this
ISR(TIMER2_OVF_vect, ISR_NOBLOCK)
#elif defined (CORE_TEENSY) || defined(CORE_STM32)
void oneMSInterval() //Most ARM chips can simply call a function
#endif
{

  //Increment Loop Counters
  loop100ms++;
  loop250ms++;
  loopSec++;

  //Loop executed every 100ms loop
  //Anything inside this if statement will run every 100ms.
  if (loop100ms == 100)
  {
    loop100ms = 0; //Reset counter
  }

  //Loop executed every 250ms loop (1ms x 250 = 250ms)
  //Anything inside this if statement will run every 250ms.
  if (loop250ms == 250)
  {
    loop250ms = 0; //Reset Counter.
  }

  //Loop executed every 1 second (1ms x 1000 = 1000ms)
  if (loopSec == 1000)
  {
    loopSec = 0; //Reset counter.
  
    //**************************************************************************************************************************************************
    //This records the number of main loops the system has completed in the last second
    currentStatus.loopsPerSecond = mainLoopCount;
    mainLoopCount = 0;
    //**************************************************************************************************************************************************
    //increament secl (secl is simply a counter that increments every second and is used to track whether the system has unexpectedly reset
    currentStatus.secl++;
    //**************************************************************************************************************************************************

  }
//#if defined(CORE_AVR)
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)|| defined (ARDUINO_AVR_PRO)  //AVR chips use the ISR for this
    //Reset Timer2 to trigger in another ~1ms
    TCNT2 = 130;            //Preload timer2 with 100 cycles, leaving 156 till overflow.
    TIFR2  = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
#endif
}

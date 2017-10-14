void driveOutputs()
{
  
  for (byte DoutCH = 1; DoutCH < 17 ; DoutCH++)     //loop through outputs 1 to 16
   
    {
      
     portX = DoutCH-1;
     if (pinOut[portX+1] < 255)    //if pin is not set to 255 then is in use by selected board so drive the digital outputs according to current active channel
       { 
        if(BIT_CHECK(currentStatus.testIO_hardware, 0) == 0)   //if testenable unset then check outputs according to port editor
          {
        //    currentStatus.dev1 = gear_port_Enabled[2];
        //    currentStatus.dev2 = boxOutput[2];//configPage1.shiftsolenoid_1 & B00011111;//gear_port_Enabled[0];
        //   if(gear_port_Enabled[portX+1] == 1)   //is output in use by gearbox solenoids or valve etc ?
        //     {
              //if (configPage2.port_Enabled[portX] == 1){ configPage2.port_Enabled[portX] = 0;}   //block/disable the outputs use by porteditor(is not burned so is not saved to eeprom)
       //       gearOutput();      
       //      }
           if(configPage2.port_Enabled[portX] == 1)   // is output enabled in porteditor ?
              {
               porteditorOutput();
              } //ends the check is output is enabled
           else   //else if output is not enabled
              {
               currentStatus.condition_pass[portX] = 0;      //set pin state to off 
              }
          
          outputaction(portX); //set output high/lows 
            
          }  //ends the else (not a test)
        
        else      //this is a test so do direct port ops according to the bitread  
          {   
           state = bitRead(currentStatus.digOut,(portX));
           digitalWrite(pinOut[portX+1],state);
          }
             
       }    // ends is the pin used by board
   else
      {
        configPage2.port_Enabled[portX] = 0; //disable output as pin is not configured
        state = 0;    //pin is unused(255 in utils) so set pin state off
      }        
    }     // ends the for next 1-17 count loop
    
   if(BIT_CHECK(currentStatus.testIO_hardware, 0)) //if testenabled is set 
     {   
      if(BIT_CHECK(currentStatus.testIO_hardware, 1) == 0) //and if testactive is clear 
        {
          BIT_CLEAR(currentStatus.testIO_hardware, 0);    //clear testenabled flag now all outputs have been forced
        }
     }   
}

void outputaction(uint8_t chanX)
{
  
  //the output stage is checked to set flags then the actual outputs are fed off those flags. this is for future proof when pwm outputs  and external outputs are added
 //bit0 == output on/off
 //bit1 == active high/low
 //bit2
 //bit3
 //bit4
 //bit5
 //bit6
 //bit7
 
  // now if the relationship conditions have passed(if required) then do the output action
          if (currentStatus.condition_pass[chanX] == 1)
              {  
              //activate the output according to setup/config
              if (configPage2.port_PortValue[chanX] == 1 ) // if the output is active high
                  {
                   currentStatus.OutputPort[chanX] = B00000011;  // output1 is [0]
                   BIT_SET(currentStatus.digOut, (chanX) );
                  }  
              else if (configPage2.port_PortValue[chanX] == 0 )  // if the output is active low
                  {
                    currentStatus.OutputPort[chanX] = B00000001;
                    BIT_CLEAR(currentStatus.digOut, (chanX) );
                  }  
              }
              
 
          else if (currentStatus.condition_pass[chanX] == 0)  //means it met the conditions to turn off
                  {
                  //deactivate the output according to setup/config
                  if (configPage2.port_PortValue[chanX] == 1 ) // if the output is active high
                      {
                       currentStatus.OutputPort[chanX] = B00000010;             
                       BIT_CLEAR(currentStatus.digOut, (chanX) );
                      }    
                  else if (configPage2.port_PortValue[chanX] == 0 ) 
                      {
                       currentStatus.OutputPort[chanX] = B00000000;
                       BIT_SET(currentStatus.digOut, (chanX) );
                      }   
                  }   

           state = bitRead(currentStatus.digOut,(chanX));
           digitalWrite(pinOut[chanX+1],state);     
}

void initialOutputs()
{
  
  for (byte DoutCH = 1; DoutCH < 17 ; DoutCH++)     //loop through outputs 1 to 16
    {
      
     portX = DoutCH-1;
     if (pinOut[portX+1] != 255 )    //if pin is not set to 255 then is in use by selected board so drive the digital outputs according to current active channel
       { 
        state = configPage2.port_InitValue[portX];
        if (!configPage2.port_Enabled[portX])
          {
            state = 0;    //if output is disabled then set pin off
          } 
       }
   else
      {
        configPage2.port_Enabled[portX] = 0; //disable output as pin is not configured
        state = 0;    //pin is unused(0 in utils) so set pin state off
      }      
        currentStatus.condition_pass[portX] = state;
   }

}    

void porteditorOutput()
{
  //currentStatus.dev1 = configPage2.port_OutOffset[portX];
 uint8_t checkcondloop = 0;
              uint16_t offValue1;
              uint16_t offValue2;
              //first get the conditon checks into flag array for current io output channel (0 is first condition, 1 is second)
            if (!((configPage2.port_OutOffset[portX] == 13) ||(configPage2.port_OutOffset[portX] == 14)))
              {  
              getAnalogBYoffset(configPage2.port_OutOffset[portX],checkcondloop); //lookup analog value based on selected outchannel per current output
              }
              switch (configPage2.port_Condition[portX])
                    {
                    case 38:
                    //& condition
                  // currentStatus.dev1 = configPage2.port_OutOffset[0];
            if ((configPage2.port_OutOffset[portX] == 13) ||(configPage2.port_OutOffset[portX] == 14))
              {
               if (currentStatus.digIn & configPage2.port_Threshold[portX])
                  {
                   currentStatus.condition_pass[portX] = 1;  //set status to pass

                  }
              else
                  {
                   currentStatus.condition_pass[portX] = 0;  //set status to pass
                   
                  }
              }                    
                    break;
                    
                    case 60:
                    //< condition
                     offValue2 = (configPage2.port_Threshold[portX] + configPage2.port_Hysteresis[portX]); 
                     if (offValue2 >= 1024 ){ offValue2 = 1024;}

                     if (currentStatus.currentInputvalue[checkcondloop] < configPage2.port_Threshold[portX])
                        {
                         currentStatus.condition_pass[portX] = 1;  //set status to pass
                        }
 
                     if (currentStatus.currentInputvalue[checkcondloop] >= offValue2) 
                        {                         
                         currentStatus.condition_pass[portX] = 0;  //set status to off (threshold+hysteresis)
                        }
       
                    break;

                    case 61:
                    //= condition
                    offValue1 = (configPage2.port_Threshold[portX] - configPage2.port_Hysteresis[portX]);
                    if (offValue1 <= 0 ){offValue1 = 0;}
                    offValue2 = (configPage2.port_Threshold[portX] + configPage2.port_Hysteresis[portX]);
                    if (offValue2 >= 1024 ){ offValue2 = 1024;}
                    
                    if (currentStatus.currentInputvalue[checkcondloop] == configPage2.port_Threshold[portX])
                        {
                             currentStatus.condition_pass[portX] = 1;  //set status to pass
                        }
               else if (currentStatus.condition_pass[portX] == 1)
                        {
                         if (currentStatus.currentInputvalue[checkcondloop] < offValue1)  
                            {
                             currentStatus.condition_pass[portX] = 0; // set status to fail (threshold-hysteresis)
                            }
                    else if (currentStatus.currentInputvalue[checkcondloop] > offValue2)  
                            {
                             currentStatus.condition_pass[portX] = 0; // set status to fail (threshold+hysteresis)
                            }
                         }
  
                    break;

                    case 62:
                    //> condition      
                    offValue1 = (configPage2.port_Threshold[portX] - configPage2.port_Hysteresis[portX]);
                    if (offValue1 <= 0 ){offValue1 = 0;}
      
                    if (currentStatus.currentInputvalue[checkcondloop] > configPage2.port_Threshold[portX])
                        {
                         currentStatus.condition_pass[portX] = 1;  //set status to pass
                        }   
                    if (currentStatus.currentInputvalue[checkcondloop] <= offValue1) // has it dropped below the hys
                        {
                         currentStatus.condition_pass[portX] = 0;  //set status to off (threshold+hysteresis)
                        }
                    break;                    
                    
                    } //ends the switch case 
}

void selectorInput()
{
//  currentStatus.current_gear_Selected = 0;
    //first check lever position
  if (currentStatus.digIn == 0)
      {
        currentStatus.current_gear_Selected = 80;     //no selector switch is active so error
      }
  else
      {      
        if (configPage1.park_in != 0)
            {
             if (BIT_CHECK(currentStatus.digIn,(((configPage1.park_in)-1) & B00011111)) ==  inpin2binary[(((configPage1.park_in)-1) & B00011111)])
                {
                  currentStatus.current_gear_Selected = 11;
                  currentStatus.first_Run = 0;
                }    
            }
            
        if (BIT_CHECK(currentStatus.digIn,(configPage1.neutral_in & B00011111)) ==  inpin2binary[(configPage1.neutral_in & B00011111)])
          {
            //if((currentStatus.current_gear_Selected != 0) && ((configPage1.park_in & B00011111) != (configPage1.neutral_in & B00011111)))
            //  { 
            //    currentStatus.current_gear_Selected = 80;                
            //  }
            //else
            //  {  
                currentStatus.current_gear_Selected = 10;
                currentStatus.first_Run = 0;
            //  }  
          }
      
       if  (BIT_CHECK(currentStatus.digIn,(configPage1.drive_in & B00011111)) ==  inpin2binary[(configPage1.drive_in & B00011111)])      //
          {
           // if(currentStatus.current_gear_Selected != 0)
           //   { 
           //     currentStatus.current_gear_Selected = 80;                
           //   }
           // else
           //   { 
                currentStatus.current_gear_Selected = 20;
           //   }
          }
      
       if (BIT_CHECK(currentStatus.digIn, (configPage1.reverse_in & B00011111)) == inpin2binary[(configPage1.reverse_in & B00011111)])
          {
          //  if(currentStatus.current_gear_Selected != 0)
          //    { 
          //      currentStatus.current_gear_Selected = 80;                
          //    }
          //  else
          //    {             
                currentStatus.current_gear_Selected = 30;
          //    }
          }

      if ((configPage1.gear1_in != 0)||(gear[1] != 255))       //if selection is NOT unused by selector or NOT unused by gearbox  
               {
                if ((gear[1] < 255)&&(BIT_CHECK(currentStatus.digIn,(((configPage1.gear1_in)-1)  & B00011111)) ==  inpin2binary[((configPage1.gear1_in)-1) & B00011111]))
                  {
            //      if(currentStatus.current_gear_Selected != 0)
            //          { 
            //            currentStatus.current_gear_Selected = 80;               
            //          }
            //      else
             //         {   
                          currentStatus.current_gear_Selected = 1;
            //          }
                  }
               }     
          
       if ((configPage1.gear2_in != 0)||(gear[2] != 255))       //if selection is NOT unused by selector or NOT unused by gearbox  
                  {
                    if ((gear[2] < 255) && (BIT_CHECK(currentStatus.digIn, (((configPage1.gear2_in)-1)  & B00011111)) ==  inpin2binary[((configPage1.gear2_in)-1) & B00011111]))
                        {
            //            if(currentStatus.current_gear_Selected != 0)
            //              { 
            //               currentStatus.current_gear_Selected = 80;                
            //              }
             //           else
            //              {                     
                              currentStatus.current_gear_Selected = 2;
            //              }
                        }
                  }        
                
       if ((configPage1.gear3_in != 0)||(gear[3] != 255))       //if selection is NOT unused by selector or NOT unused by gearbox  
               {
                if ((gear[3] < 255)&&(BIT_CHECK(currentStatus.digIn, (((configPage1.gear3_in)-1)  & B00011111)) ==  inpin2binary[((configPage1.gear3_in)-1) & B00011111]))
                    {
              //      if(currentStatus.current_gear_Selected != 0)
              //        { 
              //          currentStatus.current_gear_Selected = 80;                
              //        }
              //      else                  
              //        {
                          currentStatus.current_gear_Selected = 3;
              //        }
                    }    
               }
               
       if ((configPage1.gear4_in != 0)||(gear[4] != 255))       //if selection is NOT unused by selector or NOT unused by gearbox  
                {
                 if ((gear[4] < 255)&& (BIT_CHECK(currentStatus.digIn, (((configPage1.gear4_in)-1)  & B00011111)) ==  inpin2binary[((configPage1.gear4_in)-1) & B00011111]))
                    {
             //       if(currentStatus.current_gear_Selected != 0)
              //        { 
              //          currentStatus.current_gear_Selected = 80;                
              //        }
             //       else
              //        {
                          currentStatus.current_gear_Selected = 4;
             //         }
                    }   
                }
                
       if ((configPage1.gear5_in != 0)||(gear[5] != 255))       //if selection is NOT unused by selector or NOT unused by gearbox  
                {
                 if ((gear[5] < 255) && (BIT_CHECK(currentStatus.digIn, (((configPage1.gear5_in)-1)  & B00011111)) ==  inpin2binary[((configPage1.gear5_in)-1) & B00011111]))
                    {
             //       if(currentStatus.current_gear_Selected != 0)
             //           { 
             //             currentStatus.current_gear_Selected = 80;                
             //           }
             //       else
              //          {
                            currentStatus.current_gear_Selected = 5;
              //          }
                    }    
                }
                
        if ((configPage1.gear6_in != 0)||(gear[6] != 255))       //if selection is NOT unused by selector or NOT unused by gearbox  
                {
                  if ((gear[6] < 255) && (BIT_CHECK(currentStatus.digIn, (((configPage1.gear6_in)-1)  & B00011111)) ==  inpin2binary[((configPage1.gear6_in)-1) & B00011111]))
                      {
                        currentStatus.current_gear_Selected = 6;
                      }     
                }      
       
       if ((configPage1.gear7_in != 0)||(gear[7] != 255))       //if selection is NOT unused by selector or NOT unused by gearbox  
                {
                  if ((gear[7] < 255) && (BIT_CHECK(currentStatus.digIn, (((configPage1.gear7_in)-1)  & B00011111)) ==  inpin2binary[((configPage1.gear7_in)-1) & B00011111]))
                    {
                      currentStatus.current_gear_Selected = 7;
                    }
                  
                }
                
       if ((configPage1.gear8_in != 0)||(gear[8] != 255))       //if selection is NOT unused by selector or NOT unused by gearbox  
                {
                 if ((gear[8] < 255) && (BIT_CHECK(currentStatus.digIn, (((configPage1.gear8_in)-1)  & B00011111)) ==  inpin2binary[((configPage1.gear8_in)-1) & B00011111]))
                    {
                      currentStatus.current_gear_Selected = 8;
                    }    
                }
                
       if (currentStatus.current_gear_Selected == 0)                                                //input is in error code80
            {     //selection error
               currentStatus.current_gear_Selected = 80;
            }   
      }
      
            currentStatus.dev1 =  currentStatus.current_gear_Selected; 
}

void gearOutput()
{
  switch (currentStatus.current_gear_Status)
    {
         case 1: // pattern is defined in gear[1]
         case 2:
         case 3:
         case 4:     
         case 5:        
         case 6:          
         case 7:          
         case 8:

              BIT_CLEAR(currentStatus.digOut,((boxOutput[10])-1));   //disable starter
              for (byte sO = 1; sO < 9 ; sO++)
                {                     
                  if (shiftsolenoid[sO] < 255)          //if shiftsolenoid[x] is in use
                  {
                   if ((bitRead(gear[currentStatus.current_gear_Status],(sO-1))) == 1)  // 
                     {
                       BIT_SET(currentStatus.digOut,((boxOutput[(sO-1)])-1));
                     }
                  else
                     {
                       BIT_CLEAR(currentStatus.digOut,((boxOutput[(sO-1)])-1));                                
                     }
                  }        
                }    
           
          break;
          
          case 10:    //neutral
                 currentStatus.digOut = 0;                             //reset all outputs
                 BIT_SET(currentStatus.digOut,((boxOutput[10])-1));   //enable starter
          break;

          case 11:    // park
                 currentStatus.digOut = 0;
                 BIT_SET(currentStatus.digOut,((boxOutput[10])-1));   //enable starter
          break;

          case 20:    //drive
                 BIT_CLEAR(currentStatus.digOut,((boxOutput[10])-1));   //disable starter
          break;

          case 30:    //reverse
                 BIT_CLEAR(currentStatus.digOut,((boxOutput[10])-1));   //disable starter
                 for (byte sO = 1; sO < 9 ; sO++)
                    {                     
                      if (shiftsolenoid[sO] < 255)          //if shiftsolenoid[x] is in use
                        {
                          if ((bitRead(rev_gear,(sO-1))) == 1)  //((gear[1] & B00000001) == 1)  
                            {
                              BIT_SET(currentStatus.digOut,((boxOutput[(sO-1)])-1));
                            }
                          else
                            {
                              BIT_CLEAR(currentStatus.digOut,((boxOutput[(sO-1)])-1));                                
                            }
                        }        
                    }
          break;
    }

   // currentStatus.dev1 = activestate[0]; //(32 is high 0 is low)
   // currentStatus.dev2 = boxOutput[0];
   // currentStatus.dev3 = activestate[10];
   // currentStatus.dev4 = boxOutput[10];

  //drive the boxoutputs according to active high/low and on/off          
    for (byte oloop = 0; oloop <16; oloop++)
       { 
          if  (bitRead(configPage1.FunctionActive, oloop) == 1)   //if the function is active
          //(pinOut[(boxOutput[oloop])] < 255)    //if the pinout pointed to by boxOutput[x] is in use
              {
                state = bitRead(currentStatus.digOut,((boxOutput[oloop])-1));  //get state from digOut array (1 is on 0 is off) pointed to the pin by the boxOutput[x]
               // digitalWrite(pinOut[(boxOutput[(oloop)])],state);
               if (activestate[oloop] == 32)    //if the active level of the function is high
                  {
                  digitalWrite(pinOut[(boxOutput[(oloop)])],state);   
                  }
          else if (activestate[oloop] == 0)
                  {
                    invstate = !state;
                   //if (state == 1){invstate = 0;}
                   // else if(state == 0){invstate = 1;}
                  digitalWrite(pinOut[(boxOutput[(oloop)])],invstate);                
                  }
              }
       } 

}
void gearStatus()
{  

  //set active gear according to selector position
  switch (currentStatus.current_gear_Selected)
        {

          case 1:
              if ((currentStatus.current_gear_Status == 2) || (currentStatus.current_gear_Status == 20))
                  {
                    currentStatus.current_gear_Status = 1;
                  }  
          break;

          case 2:
              currentStatus.current_gear_Status = 2;
          break;
          
          case 3:
              currentStatus.current_gear_Status = 3;
          break;
          
          case 4:
              currentStatus.current_gear_Status = 4;
          break;
          
          case 5:
              currentStatus.current_gear_Status = 5;
          break;
          
          case 6:
              currentStatus.current_gear_Status = 6;
          break;
          
          case 7:
              currentStatus.current_gear_Status = 7;
          break;
          
          case 8:
              currentStatus.current_gear_Status = 8;
          break;
          
          case 10:    //neutral
              currentStatus.current_gear_Status = 10;
              currentStatus.paddleshift_used = 0;
              currentStatus.GearNow = 0;
          break;

          case 11:    // park
              currentStatus.current_gear_Status = 11;
              currentStatus.paddleshift_used = 0;
              currentStatus.GearNow = 0;
          break;

          case 20:  //drive
               if ((configPage1.manual_auto_status & B00000001) == 0)      //if set to manual 
                  {
                    if ((currentStatus.current_gear_Status == 10) || ((currentStatus.current_gear_Status >= 1)&&(currentStatus.current_gear_Status <=8)) || (currentStatus.current_gear_Status == 80) )
                      {
                          if (currentStatus.paddleshift_used == 0)   // if a manual change has NOT occurred
                              {
                                currentStatus.current_gear_Status = 20;
                              }   
                      }
               else if (currentStatus.first_Run == 1)
                      {
                        currentStatus.current_gear_Status = 81;
                      } 
                       
          //     else if ((configPage1.manual_auto_status & B00000001) == 1)      //if set to auto      
          //              {
          //                if (currentStatus.auto_changed == 0)   // if a auto change has NOT occurred
          //                    {
          //                      currentStatus.current_gear_Status = 20;
          //                    }   
         //               }    
                  } 
                  
           if ((configPage1.manual_auto_status & B00000001) == 0) {currentStatus.dev3 = 88;}
           if ((configPage1.manual_auto_status & B00000001) == 1) {currentStatus.dev3 = 77;}
           currentStatus.dev4 = (configPage1.manual_auto_status & B00000001);  
                
          break;

          case 30:    //reverse
               if ((currentStatus.current_gear_Status == 11) || (currentStatus.current_gear_Status == 10))
                {
                  currentStatus.current_gear_Status = 30;
                }
         else if (currentStatus.first_Run == 1)
                {
                  currentStatus.current_gear_Status = 81;
                }
          break;
        }
        
     //   return;
  //now if in manual and in drive do up down inputs if enabled
 // if ((configPage1.manual_auto_status & B00000001) == 0)      //if set to manual check up down inputs
 //   {
    if (currentStatus.current_gear_Selected == 20)
        {
          if (configPage1.change_up != 0)           //if up paddle is activated
            {
              if (BIT_CHECK(currentStatus.digIn, ((configPage1.change_up -1)  & B00011111)) == inpin2binary[(configPage1.change_up -1) & B00011111])
                {
                  if (currentStatus.change_up_current == 1)
                    {
                      //pin already high 
                    }
                  else
                    {
                      //pin was low and is now high!
                      currentStatus.change_up_last = currentStatus.change_up_current;
                      currentStatus.change_up_current = 1;   
                    }
                }    
              else if (BIT_CHECK(currentStatus.digIn, ((configPage1.change_up -1)  & B00011111)) != inpin2binary[(configPage1.change_up -1) & B00011111])
                {
                  if (currentStatus.change_up_current == 1)
                    {
                      //pin was high and is now low!
                      currentStatus.change_up_last = currentStatus.change_up_current;
                      currentStatus.change_up_current = 0; 
                      stepgear(1);            
                    }
                  else
                    {
                      //pin was alrady low    
                    }
                }
            }      

          if (configPage1.change_down != 0)       // if down paddle is activated
            {      
              if (BIT_CHECK(currentStatus.digIn, ((configPage1.change_down -1)  & B00011111)) == inpin2binary[(configPage1.change_down -1) & B00011111])
                {
                  if (currentStatus.change_down_current == 1)
                    {
                      //pin already high 
                    }
                  else
                    {
                      //pin was low and is now high!
                      currentStatus.change_down_last = currentStatus.change_down_current;
                      currentStatus.change_down_current = 1;   
                    }
                }
              else if (BIT_CHECK(currentStatus.digIn, ((configPage1.change_down -1)  & B00011111)) != inpin2binary[(configPage1.change_down -1) & B00011111])
                {
                  if (currentStatus.change_down_current == 1)
                    {
                      //pin was high and is now low!
                      currentStatus.change_down_last = currentStatus.change_down_current;
                      currentStatus.change_down_current = 0;
                      stepgear(0);            
                    }
                  else
                    {
                      //pin was alrady low    
                    }
                }
            }   //ends change_down != 0      
        } //ends current_gear_Status == 10
   // }       // manual_auto_status == 0
    
    //now do lockup switching
 //   currentStatus.dev1 = configPage1.lockup_overide;//the pinin from switch
 //   currentStatus.dev2 = boxOutput[8];// the pinout the lockup1 solenoid is on
   
    if (configPage1.lockup_overide != 0)    //if lockup override is enabled
      {
        if (BIT_CHECK(currentStatus.digIn, ((configPage1.lockup_overide -1)  & B00011111)) == inpin2binary[(configPage1.lockup_overide -1) & B00011111])  //if the input pin is active
          {
 //   currentStatus.dev3 = 33; 
            BIT_SET(currentStatus.digOut,((boxOutput[8])-1));
          }
        else
          {
 //   currentStatus.dev3 = 44;
            BIT_CLEAR(currentStatus.digOut,((boxOutput[8])-1));                                
          }
      }  

      currentStatus.dev2 =  currentStatus.current_gear_Status;
}

void stepgear(uint8_t updwn)
{
   switch (updwn)
      {
        case 0:   //step down a gear
            if (currentStatus.current_gear_Selected == 20)    //if selector lever is in drive
                 {
                  if ((currentStatus.current_gear_Status > 1) &  (currentStatus.current_gear_Status < 9))
                      {
                        currentStatus.current_gear_Status --;
                        currentStatus.GearNow --;
                      }
                 }     
        break;

        case 1:   //step up a gear
              if (currentStatus.current_gear_Status == 20)  // if current gear status is D
                 {
                  currentStatus.current_gear_Status = 1;    //make gear status "1"
                  currentStatus.GearNow = 1;
                  currentStatus.paddleshift_used = 1;       //set flag to show gear has changed
                 }
              else if (currentStatus.current_gear_Status < 8)
                      {
                        if (gear[currentStatus.current_gear_Status + 1] < 255)
                            {
                              currentStatus.current_gear_Status ++;
                              currentStatus.GearNow ++;
                              //currentStatus.paddleshift_used = 1;
                            }
                      }      
        break;
        
      }
}



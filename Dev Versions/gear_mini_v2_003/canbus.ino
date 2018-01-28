void CAN0_INT_routine()
{
  #if defined OBD_ACTIVE
        #if defined OBD_CANPORT == 0
        
        #endif
  #else
    //receive_CAN0_message()
  #endif
}

void CAN1_INT_routine()
{
  #if defined OBD_ACTIVE
        #if defined OBD_CANPORT == 1    //use can1 for obd data stream
            obd_command(1);
        #endif
  #else
  //receive_CAN1_message()      
  #endif
}

void initialiseCAN0()
{
  uint8_t tryInit0 = 0;
  START_INIT0: 
    if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK)//(CAN_OK == CAN1.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
    {
       CAN0.setMode(MCP_NORMAL);
       BIT_SET(currentStatus.canstatus, BIT_CANSTATUS_CAN0ACTIVATED);
       BIT_CLEAR(currentStatus.canstatus, BIT_CANSTATUS_CAN0FAILED);
        //CONSOLE_SERIALLink.println("CAN BUS Shield init ok!");
    }
    else
    {
      BIT_CLEAR(currentStatus.canstatus, BIT_CANSTATUS_CAN0ACTIVATED);
      BIT_SET(currentStatus.canstatus, BIT_CANSTATUS_CAN0FAILED);
        //CONSOLE_SERIALLink.println("CAN BUS Shield init fail");
        //CONSOLE_SERIALLink.println("Init CAN BUS Shield again");
        delay(100);
        tryInit0++;
        if (tryInit0<50){goto START_INIT0;} //retry 50 times before quiting init
    }
 
}
//----------------------------------------------------------------------------------------
void initialiseCAN1()
{
  uint8_t tryInit1 = 0;
  START_INIT1:

    if(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)//  if(CAN_OK == CAN2.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
    {
       CAN1.setMode(MCP_NORMAL);
       BIT_SET(currentStatus.canstatus, BIT_CANSTATUS_CAN1ACTIVATED);
       BIT_CLEAR(currentStatus.canstatus, BIT_CANSTATUS_CAN1FAILED);

       // Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
       BIT_CLEAR(currentStatus.canstatus, BIT_CANSTATUS_CAN1ACTIVATED);
       BIT_SET(currentStatus.canstatus, BIT_CANSTATUS_CAN1FAILED);
      //  Serial.println("CAN BUS Shield init fail");
      //  Serial.println("Init CAN BUS Shield again");
        delay(100);       //this is ok as we havent started the actual firmware yet
        tryInit1++;
        if (tryInit1<50){goto START_INIT1;} //retry 50 times before quiting init
    }
}
//----------------------------------------------------------------------------------------
void Send_CAN0_message(byte bcChan, uint16_t theaddress, byte *thedata)
{
  byte sndStat = CAN0.sendMsgBuf(theaddress, 0, 8, thedata);    
  if(sndStat == CAN_OK)
    {
      //Serial.println("Message Sent Successfully!");
     BIT_CLEAR(currentStatus.canstatus, BIT_CANSTATUS_CAN0MSGFAIL);
    } 
 else
    {
     //Serial.println("Error Sending Message...");
     BIT_SET(currentStatus.canstatus, BIT_CANSTATUS_CAN0MSGFAIL);
    }   
}

//----------------------------------------------------------------------------------------
void Send_CAN1_message(byte bcChan, uint16_t theaddress, uint8_t *thedata)
{
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN1.sendMsgBuf(theaddress, 0, 8, thedata);
  if(sndStat == CAN_OK)
    {
      //Serial.println("Message Sent Successfully!");
      BIT_CLEAR(currentStatus.canstatus, BIT_CANSTATUS_CAN1MSGFAIL);
    }
  else
    {
      //Serial.println("Error Sending Message...");
      BIT_SET(currentStatus.canstatus, BIT_CANSTATUS_CAN1MSGFAIL);
    }

}


//---------------------------------------------------------------------------------------------

void receive_CAN0_message()
{

}

//---------------------------------------------------------------------------------------------

void receive_CAN1_message()
{

}
//----------------------------------------------------------------------------------------------

void canbroadcastperfreq(byte freqcheck)
{
     bool cpActive = 0;
  if(BIT_sCHECK(configPage4.canbroadcast_config, 0) == 1)         //generic can broadcasts
    {
     for(byte Bchan = 0; Bchan <16 ; Bchan++)
        { 
         if((configPage4.canbroadcast_freq[Bchan] & 7) == freqcheck)     // if the freq rate for chanX is set to freqcheck  (the &7 masks bit 0-2)
           {
            if(BIT_sCHECK(configPage4.canbroadcast_sel, Bchan) == 1)    //if the current channel is enabled
              {              
               direct_read_realtime();     //re-read the realtime data into array
               senddata[0] = fullStatus[(configPage4.canbroadcast_chan_offset[(Bchan)])];
               senddata[1] = 0;
               byte tempD3 = configPage4.canbroadcast_chan_offset[(Bchan)];                  
               //theaddress = (configPage4.canbroadcast_source_can_address[(bcChan-64)]+0x100);      
               if (tempD3 != 0 | tempD3 !=1 | tempD3!=2 | tempD3!= 13 | tempD3!=14 | tempD3!=15 |tempD3!=16 |tempD3!=17 )    //check if value is two bytes long
                  {
                   senddata[1] = fullStatus[(configPage4.canbroadcast_chan_offset[(Bchan)]+1)];                                   //get the second byte
                  }
               Send_CAN0_message((Bchan+64),(configPage4.canbroadcast_source_can_address[(Bchan)]+0x100),senddata);       // 
              }  
          }
        }   
    }

//  if(BIT_sCHECK(configPage4.canbroadcast_config, 3) == 1)       //remote inputs 0-15
//    { 
//     //byte RIchan = 1;
//     for(byte RIchan = 0; RIchan <16 ; RIchan++)
//        { 
//         if((configPage4.remoteinput_freq[RIchan] & 7) == freqcheck)     //  if the freq rate for chanX is set to freqcheck  (the &7 masks bit 0-2)
//           {
//            if(BIT_sCHECK(configPage4.remoteinput_sel_0_16, RIchan) == 1)            //if remoteinput channel X is enabled                 
//              {
//               if ((((configPage4.remoteinput_port[RIchan]&63)-1))<= 16)
//                  {
//                    cpActive =BIT_sCHECK(configPage1.DinchanActive, ((configPage4.remoteinput_port[RIchan]&63)-1));      // if chanX input pin port is valid(ie enabled in utils)  &63 is a 6bit mask               
//                  }
//               else if ((((configPage4.remoteinput_port[RIchan]&63)-1))<= 32)
//                  {   
//                    cpActive =BIT_sCHECK(configPage1.DinchanActive_2, ((configPage4.remoteinput_port[RIchan]&63)-17));
//                  }  
                 
//                  if (cpActive ==1)
//                    {
//                      senddata[0] = (BIT_sCHECK(currentStatus.digIn, (RIchan)));   //set the first byte to the value of the input state
//                      Send_CAN0_message(RIchan,(configPage4.remoteinput_can_address[RIchan]+0x100),senddata);                    
//                    }
//              }
//           }
//        }
//    }          
  
//  if(BIT_sCHECK(configPage4.canbroadcast_config, 4) == 1)     // remote inputs 16-31
//    {
//     for(byte RIchan = 16; RIchan <32 ; RIchan++)
//        { 
//         if((configPage4.remoteinput_freq[RIchan] & 7) == freqcheck)     //  if the freq rate for chanX is set to freqcheck  (the &7 masks bit 0-2)
//           {
//            if(BIT_sCHECK(configPage4.remoteinput_sel_17_31, (RIchan-16)) == 1)            //if remoteinput channel X is enabled                 
//              {
//               if ((((configPage4.remoteinput_port[RIchan]&63)-1))<= 16)
//                  {
//                    cpActive =BIT_sCHECK(configPage1.DinchanActive, ((configPage4.remoteinput_port[RIchan]&63)-1));      // if chanX input pin port is valid(ie enabled in utils)  &63 is a 6bit mask               
//                  }
//               else if ((((configPage4.remoteinput_port[RIchan]&63)-1))<= 32)
//                  {   
//                    cpActive =BIT_sCHECK(configPage1.DinchanActive_2, ((configPage4.remoteinput_port[RIchan]&63)-17));
//                  }  
                 
//                  if (cpActive ==1)
//                    {
//                      senddata[0] = (BIT_sCHECK(currentStatus.digIn_2, (RIchan-16)));   //set the first byte to the value of the input state
//                      Send_CAN0_message(RIchan,(configPage4.remoteinput_can_address[RIchan]+0x100),senddata);                    
//                    }
//              }
//           }
//        }
//    }
  
 //currentStatus.dev2 = freqcheck;
 // currentStatus.dev1 = BIT_sCHECK(configPage4.remoteAninput_sel, 1);//configPage3.canbroadcast_out; 
 // currentStatus.EXin[13] =  (configPage4.remoteAninput_port[1]&63)-1;
 // currentStatus.EXin[14] = BIT_sCHECK(configPage1.AinchanActive,  ((configPage4.remoteAninput_port[1]&63)-1));
  

//  if(BIT_sCHECK(configPage4.canbroadcast_config, 5) == 1)       //remote Ainputs 0-15
//    { 
//     //byte RIchan = 1;
//     for(byte RIchan = 0; RIchan <16 ; RIchan++)
//        { 
//         if((configPage4.remoteAninput_freq[RIchan] & 7) == freqcheck)     //  if the freq rate for chanX is set to freqcheck  (the &7 masks bit 0-2)
//           {
//            if(BIT_sCHECK(configPage4.remoteAninput_sel, RIchan) == 1)            //if remoteinput channel X is enabled                 
//              {
//               if ((((configPage4.remoteAninput_port[RIchan]&63)-1))<= 16)
//                  {
//                    cpActive =BIT_sCHECK(configPage1.AinchanActive, ((configPage4.remoteAninput_port[RIchan]&63)-1));      // if chanX input pin port is valid(ie enabled in utils)  &63 is a 6bit mask               
//                  }
//               //else if ((((configPage4.remoteAinput_port[RIchan]&63)-1))<= 32)
//               //   {   
//               //     cpActive =BIT_sCHECK(configPage1.AinchanActive_2, ((configPage4.remoteAinput_port[RIchan]&63)-17));
//               //   }  
                 
//                  if (cpActive ==1)
//                    {
//                      senddata[0] = lowByte(currentStatus.Analog[RIchan]);    //set the first byte to the low byte value of the analog
//                      senddata[1] = highByte(currentStatus.Analog[RIchan]);   //set the second byte to the high byte value of the analog
//                      Send_CAN0_message(RIchan,(configPage4.remoteAninput_can_address[RIchan]+0x100),senddata);                    
//                    }
//              }
//           }
//        }
//    }          
              
}

void obd_command(byte usecan)
{
      switch(usecan)
        { 
         case 0:
             CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
         break;

         case 1:
             CAN1.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
         break;
        }
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
    {
      // id is extended 29bit address
      //sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    }
    else  
    {
      // id is std 11 bit
      //sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
    } 
     
    obd_response(rxBuf[2]);
    
    #if defined OBD_CANPORT == 0    //use can0 to send obd data stream
            Send_CAN0_message(0xFF,configPage4.gpio_obd_address,obddata);
    #endif
    
    #if defined OBD_CANPORT == 1    //use can1 to send obd data stream
            Send_CAN1_message(0xFF,configPage4.gpio_obd_address,obddata);
    #endif

}

void obd_response(byte therequest)
{
  switch (therequest)
  {
  case 0:  
  obddata[0] =  0x06;    // sending 6 bytes
  obddata[1] =  0x41;    // Same as query, except that 40h is added to the mode value. So:41h = show current data ,42h = freeze frame ,etc.
  obddata[2] =  0x00;    // pid code
  obddata[3] =  0x08;   //B0000 1000
  obddata[4] =  0x7E;   //B0111 1110
  obddata[5] =  0xB8;   //B1011 1000
  obddata[6] =  0x11;   //B0001 0001
  obddata[7] = 0x00;
  break;

  case 5:      //engine coolant temperature A-40 , range is -40 to 215 deg C
  int16_t enginecoolant;
  enginecoolant = (realtimebufferA[(7)]-40);
  obdcalcA = enginecoolant + 40;
  obddata[0] =  0x03;    // sending 3 bytes
  obddata[1] =  0x41;    // Same as query, except that 40h is added to the mode value. So:41h = show current data ,42h = freeze frame ,etc.
  obddata[2] =  0x05;    // pid code
  obddata[3] =  obdcalcA;   //the data value A
  obddata[4] =  0x00;   //the data value B
  obddata[5] =  0x00; 
  obddata[6] =  0x00; 
  obddata[7] =  0x00;
  break;

  
  case 11:        //0B or MAP , A ,range is 0 to 255 kPa
  uint16_t engineMap;
  engineMap = (realtimebufferA[(5)]<<8) | realtimebufferA[4];
  obddata[0] =  0x03;    // sending 3 byte
  obddata[1] =  0x41;    // 
  obddata[2] =  0x0B;    // pid code
  obddata[3] =  engineMap;    // the map value
  obddata[4] =  0x00;
  obddata[5] =  0x00; 
  obddata[6] =  0x00; 
  obddata[7] =  0x00;
  break;

  case 12:        //0C or rpm , 256A+B / 4 , range is 0 to 16383.75 rpm
  uint16_t revs; 
  revs = (realtimebufferA[(15)]<<8) | realtimebufferA[14];
  obdcalcA = revs*4;
  obdcalcB = obdcalcA/256;              //the rmainder will be lost for the next calculation which we dont want! it is the first byte for sending
  obdcalcC = obdcalcB*256;              //
  obdcalcD = obdcalcA - obdcalcC;       //calcs the second byte for sending
  obddata[0] =  0x04;    // sending 4 byte
  obddata[1] =  0x41;    // 
  obddata[2] =  0x0C;    // pid code
  obddata[3] =  obdcalcB;
  obddata[4] =  obdcalcD;
  obddata[5] =  0x00; //B0000
  obddata[6] =  0x00; //B0000
  obddata[7] = 0x00;
  break;

  case 13:        // 0D or vehicle speed , A , range is 0 to 255 km/h
  obddata[0] =  0x03;    // sending 3 bytes
  obddata[1] =  0x41;    // Same as query, except that 40h is added to the mode value. So:41h = show current data ,42h = freeze frame ,etc.
  obddata[2] =  0x0D;    // pid code
  obddata[3] =  0x04;   //the data value A
  obddata[4] =  0x00;   //the data value B
  obddata[5] =  0x00; 
  obddata[6] =  0x00; 
  obddata[7] =  0x00;
  break;

  case 15:      //inlet air temperature, A-40 , range is -40 to 215 deg C
  uint16_t engineIAT;
  engineIAT = (realtimebufferA[(6)]-40);
  obdcalcA = engineIAT + 40;
  obddata[0] =  0x03;    // sending 3 bytes
  obddata[1] =  0x41;    // Same as query, except that 40h is added to the mode value. So:41h = show current data ,42h = freeze frame ,etc.
  obddata[2] =  0x0F;    // pid code
  obddata[3] =  obdcalcA;   //the data value A
  obddata[4] =  0x00;   //the data value B
  obddata[5] =  0x00; 
  obddata[6] =  0x00; 
  obddata[7] =  0x00;
  break;

  case 17:      //tps percentage, 100/256 A , range is 0 to 100 percent
  uint16_t tpsPC;
  tpsPC = realtimebufferA[24];
  obdcalcA = (tpsPC *256) /100;
  obddata[0] =  0x03;    // sending 3 bytes
  obddata[1] =  0x41;    // Same as query, except that 40h is added to the mode value. So:41h = show current data ,42h = freeze frame ,etc.
  obddata[2] =  0x11;    // pid code
  obddata[3] =  obdcalcA;   //the data value A
  obddata[4] =  0x00;   //the data value B
  obddata[5] =  0x00; 
  obddata[6] =  0x00; 
  obddata[7] =  0x00;
  break;

  
  case 28:      //obd standard
  uint16_t obdstandard;
  obdstandard = 7;
  obddata[0] =  0x03;    // sending 3 bytes
  obddata[1] =  0x41;    // Same as query, except that 40h is added to the mode value. So:41h = show current data ,42h = freeze frame ,etc.
  obddata[2] =  0x1C;    // pid code
  obddata[3] =  obdstandard;   //the data value A
  obddata[4] =  0x00;   //the data value B
  obddata[5] =  0x00; 
  obddata[6] =  0x00; 
  obddata[7] =  0x00;
  break;
  
  case 32:      //pids 21-40
  obddata[0] =  0x06;    // sending 4 bytes
  obddata[1] =  0x41;    // Same as query, except that 40h is added to the mode value. So:41h = show current data ,42h = freeze frame ,etc.
  obddata[2] =  0x20;    // pid code
  obddata[3] =  0x18; //B0001 1000
  obddata[4] =  0x00; //B0000 0000
  obddata[5] =  0x18; //B0001 1000
  obddata[6] =  0x01; //B0000 0001
  obddata[7] = 0x00;
  break;

    case 64:       //pids 41-60  
  obddata[0] =  0x06;    // sending 4 bytes
  obddata[1] =  0x41;    // Same as query, except that 40h is added to the mode value. So:41h = show current data ,42h = freeze frame ,etc.
  obddata[2] =  0x40;    // pid code
  obddata[3] =  0x40; //B0100 0000
  obddata[4] =  0x00; //B0000 0000
  obddata[5] =  0x40; //B0100 0000
  obddata[6] =  0x10; //B0001 0000
  obddata[7] = 0x00;
  break;

  case 66:      //control module voltage, 256A+B / 1000 , range is 0 to 65.535v
  uint16_t ecuBatt;
  ecuBatt = realtimebufferA[9];
  obdcalcA = ecuBatt*100;               //should be *1000 but ecuBatt needs a /10
  obdcalcB = obdcalcA/256;              //the rmainder will be lost for the next calculation which we dont want! it is the first byte for sending
  obdcalcC = obdcalcB*256;              //
  obdcalcD = obdcalcA - obdcalcC;       //calcs the second byte for sending
  obddata[0] =  0x04;    // sending 4 bytes
  obddata[1] =  0x41;    // Same as query, except that 40h is added to the mode value. So:41h = show current data ,42h = freeze frame ,etc.
  obddata[2] =  0x42;    // pid code
  obddata[3] =  obdcalcB;   //the data value A
  obddata[4] =  obdcalcD;   //the data value B
  obddata[5] =  0x00; 
  obddata[6] =  0x00; 
  obddata[7] =  0x00;
  break;

  default:
  return;
  break;
  
  }  
}


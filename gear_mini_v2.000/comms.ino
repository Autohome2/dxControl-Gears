#include <Arduino.h>

/*
Speeduino Can interface
Copyright (C) Darren Siepka
A full copy of the license may be found in the projects root directory

Based on code by Josh Stewart for the Speeduino project , see www.Speeduino.com for more info
*/

/*
This is called when a command is received over serial from TunerStudio / Megatune
It parses the command and calls the relevant function
*/
#include "comms.h"
#include "globals.h"
//#include "storage.h"


void command()
{
    switch (SERIALLink.read())
          {
          case 'A':
                  while (SERIALLink.available() == 0) {}
                  replylength = (SERIALLink.read());              //read in reply length
                  for (byte rdata = 0; rdata < replylength; rdata++) //read x bytes of data according to replylength
                      {
                       realtimebuffer[rdata] = (SERIALLink.read());
                      }
          break;  

          case 'r': 
                byte cmd;
                byte tsCanId_sent;         
                uint16_t offset, length;
                byte tmp;
                while (SERIALLink.available() == 0) {}
                tsCanId_sent = SERIALLink.read(); //Read the $tsCanId
                while (SERIALLink.available() == 0) {}                
                cmd = SERIALLink.read();
                    tmp = SERIALLink.read();
                    offset = word(SERIALLink.read(), tmp);
                    tmp = SERIALLink.read();
                    if (cmd != 207)          //if is "W"(dec87)+120 only 1 more byte is sent
                      {
                      length = word(SERIALLink.read(), tmp);     
                      }
                    else{length = tmp;}
                    if (tsCanId_sent ==  thistsCanId)
                      {
                        do_rCommands(cmd,tsCanId_sent,offset,length);  
                      }
          
          break;
    }

return;
 
}

void do_rCommands(uint8_t commandletter, uint8_t canid, uint16_t cmdoffset, uint16_t cmdlength)
{  
 
    switch (commandletter)
           {
           
           case 48:    //previously 0x30:
                                 Serial.print("got to 3d");
                                 // direct_sendValues(offset, length, cmd);
           break; 
           
           case 134:  //
                    SERIALLink.print("r");
                    SERIALLink.write(thistsCanId);              //canId of the device you are requesting data from
                    SERIALLink.write(134);                     //0x0E+128dec = 0x8E
                    SERIALLink.write(zero);                       // dummy offset lsb
                    SERIALLink.write(zero);                       // dummy offset msb
                    SERIALLink.write(sizeof(simple_remote_RevNum));
                    SERIALLink.write(zero);                       //dummy length msb
                    for (unsigned int sg = 0; sg < sizeof(simple_remote_RevNum) - 1; sg++)
                        {
                        SERIALLink.write(simple_remote_RevNum[sg]);
                        currentStatus.secl = 0; //This is required in TS3 due to its stricter timings
                        }     
           break; 
           
           case 135:    //
           Serial.print("reply135");
                    SERIALLink.print("r");
                    SERIALLink.write(thistsCanId);              //canId of the device you are requesting data from
                    SERIALLink.write(135);                     //0x0F+128dec = 0x8F
                    SERIALLink.write(zero);                       // dummy offset lsb
                    SERIALLink.write(zero);                       // dummy offset msb
                    SERIALLink.write(sizeof(simple_remote_signature));
                    SERIALLink.write(zero);                       //dummy length msb
                    for (unsigned int sg = 0; sg < sizeof(simple_remote_signature) - 1; sg++)
                        {
                        SERIALLink.write(simple_remote_signature[sg]);  
                        // Serial.write(simple_remote_signature[sg]);
                        }  
           break;
                        
           case 180:  //(0x3c+120 == 0xB4(180dec)):
                   direct_sendValues(cmdoffset, cmdlength, 180);
           break;

           case 186: // r version of B(66dec)+120 == 186
                    // Burn current values to eeprom
                    writeConfig();
           break;
           
           case 189: // r version of E(69dec)+120 ==  189
                  commandButtons(cmdoffset);
           break;
                               
           case 200:  //r version of P == dec80+120 == 200
                  currentStatus.currentPage = byte(cmdoffset);
           break;
          
           case 206:  //r version of V == dec86+120 == 206
                  direct_sendPage(cmdlength,thistsCanId,206);
           break;
                    
           case 207:  //r version of W(87dec)+120 == 207
                  //int valueOffset; //cannot use offset as a variable name, it is a reserved word for several teensy libraries
                  direct_receiveValue(cmdoffset, cmdlength);  //Serial.read());                    
           break;
           
       } //closes the switch/case 
}

void sendTheCommand(uint8_t commandletter, uint8_t canid, uint16_t cmdoffset, uint16_t cmdlength)
{
  SERIALLink.write(commandletter);          // send command letter to the Speeduino
   if (commandletter == 'r')                  //the next 5 bytes ned only be sent if it is an "r" command
    {
      SERIALLink.write(canid);              //canId of the device you are requesting data from
      SERIALLink.write(0x3C);               // the "r" command type(0x30 requests specific realtime data)
      SERIALLink.write(lowByte(cmdoffset));
      SERIALLink.write(highByte(cmdoffset));
      SERIALLink.write(lowByte(cmdlength));
      SERIALLink.write(highByte(cmdlength));  
    }  
}

void receiveValue(uint16_t rvOffset, uint8_t newValue)
{      
        
  void* pnt_configPage;//This only stores the address of the value that it's pointing to and not the max size

  switch (currentStatus.currentPage)
  {

    case 1: //simple_remote_setupPage:
      pnt_configPage = &configPage1; //Setup a pointer to the relevant config page
     //For some reason, TunerStudio is sending offsets greater than the maximum page size. I'm not sure if it's their bug or mine, but the fix is to only update the config page if the offset is less than the maximum size
      if ( rvOffset < page_1_size)
      {
        *((uint8_t *)pnt_configPage + (uint8_t)rvOffset) = newValue; //Need to subtract 80 because the map and bins (Which make up 80 bytes) aren't part of the config pages
      }
      break;
  }
}

 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
sendPage() packs the data within the current page (As set with the 'P' command)
into a buffer and sends it.
Note that some translation of the data is required to lay it out in the way Megasqurit / TunerStudio expect it
useChar - If true, all values are send as chars, this is for the serial command line interface. TunerStudio expects data as raw values, so this must be set false in that case
*/
void sendPage(uint16_t send_page_Length,bool useChar, byte can_id)
{

 //currentPage = pagenum;
 
        void* pnt_configPage;

        switch (currentStatus.currentPage)
          {

            case simple_remote_setupPage:  //veSetPage:
                {
                // currentTitleIndex = 27;

                pnt_configPage = &configPage1; //Create a pointer to Page 1 in memory  
                  send_page_Length = page_1_size; 
                }
            break;  
          }
    
          //All other bytes can simply be copied from the config table
          SERIALLink.print("r");
          SERIALLink.write(thistsCanId);                //canId of the device you are requesting data from
          SERIALLink.write(0xCE);                       //0x56+120dec = 0xCE  
          SERIALLink.write(zero);                       // dummy offset lsb
          SERIALLink.write(zero);                       // dummy offset msb
          SERIALLink.write(lowByte(send_page_Length));  // length lsb
          SERIALLink.write(highByte(send_page_Length)); // length msb
          
          uint8_t response[send_page_Length];
          for ( uint16_t x = 0; x < send_page_Length; x++)
            {
              response[x] = *((uint8_t *)pnt_configPage + (uint16_t)(x)); //Each byte is simply the location in memory of the configPage + the offset(not used) + the variable number (x)
            }
          SERIALLink.write((uint8_t *)&response, sizeof(response));
       
      
}
/*
This function is used to store calibration data sent by Tuner Studio.
*/
void receiveCalibration(byte tableID)
{

}

void sendValues(uint16_t offset, uint16_t packetLength, uint8_t cmd)
{
  byte fullStatus[packetSize];
  byte response[packetLength];

      if (cmd == 0x41)  //(offset == 0)
        {
          SERIALLink.write("A");         //confirm cmd letter
        }
      else if (cmd == 0xB4)
        {
          Serial.print("r was sent");
          SERIALLink.write("r");         //confirm cmd letter
        } 
      SERIALLink.write(zero);           //canid
      SERIALLink.write(cmd);          //confirm cmd
      SERIALLink.write(lowByte(offset));                       // offset lsb
      SERIALLink.write(highByte(offset));                      // offset msb
      SERIALLink.write(lowByte(packetLength));      //confirm no of byte to be sent
      SERIALLink.write(highByte(packetLength));      //confirm no of byte to be sent

    if(requestCount == 0) { currentStatus.secl = 0; }
    requestCount++;

  fullStatus[0] = currentStatus.secl; //secl is simply a counter that increments each second. Used to track unexpected resets (Which will reset this count to 0)
  fullStatus[1] = currentStatus.systembits; //Squirt Bitfield
  fullStatus[2] = lowByte(currentStatus.loopsPerSecond);
  fullStatus[3] = highByte(currentStatus.loopsPerSecond);
    //The following can be used to show the amount of free memory
  currentStatus.freeRAM = freeRam();
  fullStatus[4] = lowByte(currentStatus.freeRAM); //(byte)((currentStatus.loopsPerSecond >> 8) & 0xFF);
  fullStatus[5] = highByte(currentStatus.freeRAM);
  fullStatus[6] = lowByte(mainLoopCount);
  fullStatus[7] = highByte(mainLoopCount);
  fullStatus[8] = 108;
  fullStatus[9] = 109;
  fullStatus[10] = 210;
  fullStatus[11] = 211;
    for(byte x=0; x<packetLength; x++)
  {
    response[x] = fullStatus[offset+x];
  }

   SERIALLink.write(response, (size_t)packetLength); 
   Serial.write(response, (size_t)packetLength);

  return;
}
  

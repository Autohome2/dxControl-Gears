#ifndef COMMS_H
#define COMMS_H

//These are the page numbers that the Tuner Studio serial protocol uses to transverse the different map and config pages.

#define simple_remote_setupPage    1 //Config Page 1
#define port_editor_config         2 // port editor config page

uint8_t cmdreply;
uint8_t realtimebuffer[60];   //buffer array to store the realtime data read
volatile uint8_t replylength;          // the length of the reply variable
byte zero = 0;
byte packetSize = 6;
byte requestCount = 0;

void command();
void do_rCommands(uint8_t commandletter, uint8_t canid, uint16_t cmdoffset, uint16_t cmdlength);
void receiveValue(uint16_t rvOffset, uint8_t newValue);
void sendPage(uint16_t send_page_Length,bool useChar, byte can_id);
void sendValues(uint16_t offset, uint16_t packetLength, byte cmd);
void sendTheCommand(uint8_t commandletter, uint8_t canid, uint16_t cmdoffset, uint16_t cmdlength);

#endif // COMMS_H

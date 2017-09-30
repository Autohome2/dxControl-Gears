#ifndef DIRECTCOMMS_H
#define DIRECTCOMMS_H

//These are the page numbers that the Tuner Studio serial protocol uses to transverse the different map and config pages.

//uint8_t realtimebuffer[60];   //buffer array to store the realtime data read
//volatile uint8_t replylength;          // the length of the reply variable

byte direct_packetSize = 54;
unsigned long direct_requestCount = 0; //The number of times the A command has been issued

void direct_serial_command();
void dodirect_rCommands(uint8_t commandletter, uint8_t canid, uint16_t cmdoffset, uint16_t cmdlength);
void direct_receiveValue(uint16_t rvOffset, uint8_t newValue);
void direct_sendPage(uint16_t send_page_Length, byte can_id, byte cmd);
void direct_sendValues(uint16_t offset, uint16_t packetLength, byte cmd);
void direct_sendTheCommand(uint8_t commandletter, uint8_t canid, uint16_t cmdoffset, uint16_t cmdlength);
void commandButtons(uint16_t cmdCombined);

#endif // DIRECTCOMMS_H

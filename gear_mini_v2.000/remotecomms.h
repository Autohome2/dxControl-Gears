#ifndef REMOTE_COMMS_H
#define REMOTE_COMMS_H

uint16_t Gdata[9];
uint8_t cmdreply;
uint8_t realtimebuffer[3];   //buffer array to store the realtime data read
volatile uint8_t replylength;          // the length of the reply variable
uint16_t roffset, rlength;

void remote_serial_command();
void getExternalInput(uint8_t Xchan);

#endif // COMMS_H

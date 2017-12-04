#ifndef CANBUS_H
#define CANBUS_H

void CAN0_INT_routine();
void CAN1_INT_routine();
void initialiseCAN0();
void initialiseCAN1();
void Send_CAN0_message(byte bcChan, uint16_t theaddress, byte *thedata);
void Send_CAN1_message(byte bcChan, uint16_t theaddress, byte *thedata);
void receive_CAN0_message();
void receive_CAN1_message();
void obd_command(byte usecan);
void obd_response(byte therequest);


  long unsigned int rxId;
  unsigned char len = 0;
  unsigned char rxBuf[8];

  uint8_t obddata[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  //arrray for canbus odb data to be sent
  uint8_t senddata[8] = {0x00, 0x00, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
  uint16_t obdcalcA;    //used in calcs and sending data values
  uint16_t obdcalcB;    //used in calcs and sending data values
  uint16_t obdcalcC;    //used in calcs and sending data values
  uint16_t obdcalcD;    //used in calcs and sending data values
  uint32_t obdcalcE32;    //used in calcs 
  uint32_t obdcalcF32;    //used in calcs 
  uint16_t obdcalcG16;    //used in calcs
  uint16_t obdcalcH16;    //used in calcs  
  
 #define OBD_ACTIVE   1           //set to 0 or 1 to disable/enable
 #define OBD_CANPORT  1           //set to the can port to be used(0 or 1)
 
#endif

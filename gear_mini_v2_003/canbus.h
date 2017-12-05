#ifndef CANBUS_H
#define CANBUS_H

/*--------------- canbus config options -------------- */
/* CAN 0 */
#define CAN0_ACTIVE     0         // 0 == DISABLED , 1 == ENABLED
#define CAN0_RATE       500       // eg 500 == 500kbps
#define CAN0_STD_EXT    0         // 0 == STD 11bit , 1 == EXT 29bit
#define CAN0_INT        2         // Set INT to pin 2
#define CAN0_CS         11        // Set CS to pin 11

/* CAN 1 */
#define CAN1_ACTIVE     0         // 0 == DISABLED , 1 == ENABLED
#define CAN1_RATE       500       // eg 500 == 500kbps
#define CAN1_STD_EXT    0         // 0 == STD 11bit , 1 == EXT 29bit
#define CAN1_INT        3         // Set INT to pin 3
#define CAN1_CS         12        // Set CS to pin 12

/* OBD2 */
 #define OBD_ACTIVE   1           // 0 == DISABLED , 1 == ENABLED
 #define OBD_RATE     500         // eg 500 == 500kbps
 #define OBD_CANPORT  1           // 0 == use CAN0 , 1 == use CAN1

/*-----------------------------------------------------*/

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
 
#endif

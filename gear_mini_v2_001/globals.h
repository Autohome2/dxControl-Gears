
#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>

// set core processor compile flags
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) ||defined(ARDUINO_AVR_PRO)
//  #define BOARD_NR_GPIO_PINS 54
  #define LED_BUILTIN 13
  #define CORE_AVR
  
#elif defined(CORE_TEENSY)
//  #define BOARD_NR_GPIO_PINS 34
  
#elif defined(STM32_MCU_SERIES) || defined(_VARIANT_ARDUINO_STM32_)
  #define CORE_STM32
  #define LED_BUILTIN PC13
  #define USE_FRAM 1
  #define USE_EXT_FLASH 0
  
  inline unsigned char  digitalPinToInterrupt(unsigned char Interrupt_pin) { return Interrupt_pin; } //This isn't included in the stm32duino libs (yet)
  #define portOutputRegister(port) (volatile byte *)( &(port->regs->ODR) ) //These are defined in STM32F1/variants/generic_stm32f103c/variant.h but return a non byte* value
  #define portInputRegister(port) (volatile byte *)( &(port->regs->IDR) ) //These are defined in STM32F1/variants/generic_stm32f103c/variant.h but return a non byte* value
  
#else
  #error Incorrect board selected. Please select the correct board (Usually Mega 2560) and upload again
#endif

// now set specific processor compile flags
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
  #define MEGA_AVR

#elif defined(ARDUINO_AVR_PRO)
  #define 328_AVR

#elif defined(ARDUINO_Nucleo_64)
  #define NUCLEO_64_STM32

#elif defined(MCU_STM32F103C8)
  #define F108C8_STM32

#elif defined(MCU_STM32F407VGT6)
  #define F407_STM32  

#endif

/*
 The "A" command allows you to specify where in the reltime data list you want the Speeduino to start sending you data from , and how many bytes you want to be sent.
 The format is "A" where
 A == the main command

 Speeduino replies with 
 "A" == confirming the command sent
 reply length == a single byte with a value of the number of bytes it will be sending
 xx bytes == the number of data bytes according to the replylength.
*/
 
/*
 The "r" command allows you to specify where in the reltime data list you want the Speeduino to start sending you data from , and how many bytes you want to be sent.
 The format is "r\$tsCanId\x30%2o%2c" where
 r == the main command
 tsCanId == the canid of the device being called
 x30 == dec48 the optimised output command
 2o == 2 bytes of offset
 2c == 2 bytes of length

 Speeduino replies with 
 "r" == confirming the command sent
 reply length == a single byte with a value of the number of bytes it will be sending
 xx bytes == the number of data bytes according to the replylength.
  
 */
#define maincommand  'r'    // the command to send to the Speeduino
#define commandletterr  'r'
#define tsCanId  0          // this is the tunerstudio canID for the device you are requesting data from , this is 0 for the main ecu in the system which is usually the speeduino ecu . 
                              // this value is set in Tunerstudio when configuring your Speeduino
uint16_t realtime_offset = 4;  // the offset of the realtime data to start reading from
uint16_t realtime_bytes = 1;   // the number of bytes requested

/*
change the realtime_offset to what offset position you wish to read the realtime data from
eg : realtime_offset = 4 , this relates to the currentStatus.MAP
This is 1 byte long so if this is the only byte you want to read set realtime_bytes to 1

eg : realtime_offset = 8 , this relates to the  currentStatus.battery10 (battery voltage)
this is 1 byte long.
if you set realtime_bytes = 2
then you will also be sent offset 9 too which is currentStatus.O2
 */
const unsigned char simple_remote_signature[]    = "speeduino_GearControl 201710"; 
const unsigned char simple_remote_RevNum[] = "speeduino 201710-GearControl V2.001";
uint8_t thistsCanId = 4;    // this is the tunerstudio canId of this device
const uint8_t data_structure_version = 2; //This identifies the data structure when reading / writing.
const uint8_t page_1_size = 128;
const uint16_t page_2_size = 256;
const uint8_t page_3_size = 128;

//Handy bitsetting macros
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_CHECK(var,pos) ((var) & (1<<(pos)))

#define BIT_TIMER_1HZ             0
#define BIT_TIMER_4HZ             1
#define BIT_TIMER_10HZ            2
#define BIT_TIMER_15HZ            3
#define BIT_TIMER_25HZ            4
#define BIT_TIMER_30HZ            5

volatile byte TIMER_mask;
volatile byte LOOP_TIMER;

//The status struct contains the current values for all 'live' variables
//In current version this is x bytes
struct statuses {
  volatile byte secl; //Continous
  volatile byte systembits ;
  volatile unsigned int loopsPerSecond ;
 volatile  uint16_t freeRAM ;
 volatile uint8_t currentPage;
 volatile uint8_t testIO_hardware;
 volatile bool first_Run = 1; // 1 if first loop cycle 0 if not 
// uint8_t digInState[16];             //0 is off else on(set by direct pin read or interupts)
 uint16_t currentInputvalue[2];      //holds the analog input value for each conditional input , [0] first condition and [1] holds the second
 uint16_t currentInputvalueCond[3];  //holds the input test condition flags for each test condition , [0] holds first, [1] holds the second and [2] holds the total pass
 uint8_t condition_pass[48];          // array stores pass/fail flags for the one or two(if selected) condition checks
 uint8_t condition_pass_last[48];     // array stores pass/fail flags for the one or two(if selected) condition checks
 uint8_t OutputPort[16];             //output port condition status flags
 volatile uint16_t digOut;         //bits of outputs active == 1
 volatile uint16_t digOut_Active; // bits show if channel is used by board selected when pin value is < 255
 volatile uint16_t digOut_2;
 volatile uint16_t digOut_2_Active; // bits show if channel is used by board selected when pin value is < 255
 volatile uint16_t digIn;         // bits store Raw input pin senseing
 volatile uint16_t digIn_Active;
 volatile uint16_t Analog[16];      //stores analog value read
 volatile uint16_t Analog_Active;   //bits show if analog channel is used by board selected
  volatile uint16_t EXin[16];      // 16bit value data array for external analog(0-15)
  
 volatile uint8_t change_up_current;          //used for paddle shift debounce
 volatile uint8_t change_up_last;             // as above
 volatile uint8_t change_down_current;        // as above
 volatile uint8_t change_down_last;           // as above
 volatile uint8_t paddleshift_used = 0;       // set once paddle shift has been used, cleared once put into a gear or P or N or R
 volatile uint8_t auto_changed;
 volatile uint8_t GearNow;                  // the numeric gear when forward ie 1-8
 volatile uint8_t current_gear_Status = 0;      //the actual gear the box is in
 volatile uint8_t old_gear_Status = 1;          //the last actual gear the box was in
 volatile uint8_t current_gear_Selected = 0;    //the gear selected by the lever
 volatile uint8_t old_gear_Selected = 1;       //the last gear selected by the lever
 volatile uint16_t speedo;
 volatile uint32_t odometer = 121345; 
 volatile uint8_t *odometer_byte = (uint8_t*)&odometer;    //odometer_byte[3];           //odometer reading broken into byte sized chunks , [0] is lsb ,[4] is msb 
 volatile uint16_t dev1;
 volatile uint16_t dev2;
 volatile uint16_t dev3;
 volatile uint16_t dev4;
};
//struct statuses currentStatus; //The global status object

//Page 1 of the config - See the ini file for further reference
//this is laid out as first the byte size data then the words

struct __attribute__ ( ( packed ) ) config1 {
uint16_t master_controller_address:10;
byte pinLayout;
byte speeduinoConnection ;
uint16_t speeduinoBaseCan ;       //speeduino base can address
//byte unused4;
//byte unused5;
byte unused6;
byte unused7;
byte unused8;
//byte unused9;
uint8_t GearActive;       // flags for gears in use by gearbox selected
uint16_t DoutchanActive; // digital outputchannels 1-16 active flags
uint16_t DoutchanActive_2; // digital output channels 17-32 active flags
uint16_t DinchanActive; // digital input channels 1-16 active flags
uint16_t DinchanActive_2; // digital input channels 17-32 active flags
uint16_t AinchanActive; // analog input channels 1-16 active flags
uint16_t AinchanActive_2; // analog  input channels 17-32 active flags
uint8_t shiftsolenoid_1  ;              //pinout assigned to the solenoid (bits 0-4)
//uint8_t shiftsolenoid_1_activestate;    //is solenoid output active low/high (bit 5 of shiftsolenoid_1)
uint8_t shiftsolenoid_2  ;//       
uint8_t shiftsolenoid_3  ;//      
uint8_t shiftsolenoid_4  ;//      
uint8_t shiftsolenoid_5  ;//       
uint8_t shiftsolenoid_6  ;//        
uint8_t shiftsolenoid_7  ;//        
uint8_t shiftsolenoid_8  ;//        
uint8_t lockup_solenoid_1  ;//  
uint8_t lockup_solenoid_2  ;// 
uint8_t starter_inhibit_out  ;
uint8_t boxOutput_11  ;
uint8_t boxOutput_12 ;
uint8_t boxOutput_13 ;
uint8_t boxOutput_14 ;
uint8_t boxOutput_15 ;
uint8_t boxOutput_16 ;
uint8_t boxOutput_17 ;
uint8_t boxOutput_18 ;
uint8_t boxOutput_19 ;
uint8_t boxOutput_20 ;
uint8_t boxOutput_21 ;
uint8_t boxOutput_22 ;
uint8_t boxOutput_23 ;
uint8_t boxOutput_24 ;
uint8_t boxOutput_25 ;
uint8_t boxOutput_26 ;
uint8_t boxOutput_27 ;
uint8_t boxOutput_28 ;
uint8_t boxOutput_39 ;
uint8_t boxOutput_30 ;
uint8_t boxOutput_31 ;
uint8_t change_up       ;//   = bits,   U08,    30,     [0:3] ,   $IN_PORT_NUMBERS
uint8_t change_down      ;//  = bits,   U08,    31,     [0:3] ,   $IN_PORT_NUMBERS
uint8_t park_in         ;//   = bits,   U08,    32,     [0:3] ,   $IN_PORT_NUMBERS
uint8_t reverse_in      ;//   = bits,   U08,    33,     [0:3] ,   $IN_PORT_NUMBERS
uint8_t neutral_in     ;//    = bits,   U08,    34,     [0:3] ,   $IN_PORT_NUMBERS
uint8_t drive_in       ;//    
uint8_t gear1_in       ;//    
uint8_t gear2_in       ;//  
uint8_t gear3_in       ;//   
uint8_t gear4_in       ;//    
uint8_t gear5_in       ;//   
uint8_t gear6_in       ;//   
uint8_t gear7_in       ;//   
uint8_t gear8_in       ;//   
uint8_t lockup_overide   ;//   = bits,      [0:3] ,   $IN_PORT_NUMBERS
uint8_t gearbox_type  ;

uint16_t FunctionActive; // function active flags 1-16
uint16_t FunctionActive_2; // function active flags 17-32

//byte unused72;
//byte unused73;
byte unused74;
byte unused75;
byte unused76;
byte unused77;
byte unused78;
byte unused79;
byte unused80;
byte unused81;
byte unused82;
byte unused83;
byte unused84;
byte unused85;
byte unused86;
byte unused87;
byte unused88;
byte unused89;
byte unused90;
byte unused91;
byte unused92;
byte unused93;
byte unused94;
byte unused95;
byte unused96;
byte unused97;
byte unused98;
byte unused99;
uint8_t display_type;
uint8_t manual_auto_status = 0;
uint8_t unused102;
uint8_t unused103;
byte unused104;
byte unused105;
byte unused106;
byte unused107;
byte unused108;
byte unused109;
uint16_t pulse_per_mile;
uint16_t final_drive_ratio;         //ratio * 1000
uint16_t tyre_diameter;             //diamter *100
uint16_t number_wheel_impulses;     //Vss pulse/rev (teeth/rev)
//byte unused110;
//byte unused111;
//byte unused112;
//byte unused113;
//byte unused114;
//byte unused115;
//byte unused116;
//byte unused117;
byte unused118;
byte unused119;
byte unused120 = 200;
byte unused121;
byte unused122;
byte unused123;
byte unused124;
byte unused125;
byte unused126 = 226;
byte unused127 = 227;
};

//Page 2 of the config - See the ini file for further reference
//this is laid out as first the byte size data then the words

struct __attribute__ ( ( packed ) ) config2 {
  uint8_t    port_Enabled[16];                // 1 if enabled 0 if not
  uint8_t    port_Condition[16];              // < is 60, = is 61, > is 62, & is 38
  uint8_t    port_Condition_relationship[16]; // none is 32 , OR is 124 , AND is 38 , NOT(!) is 33  
  uint8_t    port_InitValue[16];              // 1 on 0 off
  uint8_t    port_PortValue[16];              // 1 if active high 0 if active low
  uint8_t    port_OutSize[16];                // unsure of purpose but must be present
  uint16_t   port_OutOffset[16];              // port offset refers to the offset value from the output channels
  uint16_t    port_Threshold[16];              // threshhold value for on/off
  uint16_t    port_Hysteresis[16];             // hysteresis value for on/off
  uint8_t    port_CanId[16];                  // TScanid of the device the output channel is from  
  uint8_t    gear_port_Enabled2[16];                // 1 if enabled 0 if not
  //byte unused2_208;
//byte unused2_209;
//byte unused2_210;
//byte unused2_211;
//byte unused2_212;
//byte unused2_213;
//byte unused2_214;
//byte unused2_215;
//byte unused2_216;
//byte unused2_217;
//byte unused2_218;
//byte unused2_219;
//byte unused2_220;
//byte unused2_221;
//byte unused2_222;
//byte unused2_223;
byte unused2_224;
byte unused2_225;
byte unused2_226;
byte unused2_227;
byte unused2_228;
byte unused2_229;
byte unused2_230;
byte unused2_231;
byte unused2_232;
byte unused2_233;
byte unused2_234;
byte unused2_235;
byte unused2_236;
byte unused2_237;
byte unused2_238;
byte unused2_239;
byte unused2_240;
byte unused2_241;
byte unused2_242;
byte unused2_243;
byte unused2_244;
byte unused2_245;
byte unused2_246;
byte unused2_247;
byte unused2_248;
byte unused2_249;
byte unused2_250;
byte unused2_251;
byte unused2_252;
byte unused2_253; 
byte unused2_254;
byte unused2_255; 
};

//Page 3 of the config - See the ini file for further reference
//this is laid out as first the byte size data then the words

struct __attribute__ ( ( packed ) ) config3 {
uint16_t exinsel;                 // External input channel enabled bit flags
uint16_t INdata_from_Can[16];     // can address of source of data 0x100(256dec) to 0x7FF(2047dec) as 0 dec - 535 dec
byte data_from_offset[16];        // offset of data source 0 - 255
byte num_bytes[16];               // number of bytes length of data source 0,1,or 2
byte unused3_66;
byte unused3_67;
byte unused3_68;
byte unused3_69;
byte unused3_70;
byte unused3_71; 
byte unused3_72;
byte unused3_73;
byte unused3_74;
byte unused3_75;
byte unused3_76;
byte unused3_77;
byte unused3_78;
byte unused3_79;
byte unused3_80;
byte unused3_81;
byte unused3_82;
byte unused3_83;
byte unused3_84;
byte unused3_85;
byte unused3_86;
byte unused3_87;
byte unused3_88;
byte unused3_89;
byte unused3_90;
byte unused3_91;
byte unused3_92;
byte unused3_93;
byte unused3_94;
byte unused3_95;
byte unused3_96;
byte unused3_97;
byte unused3_98;
byte unused3_99;
uint8_t unused3_100;
uint8_t unused3_101;
uint8_t unused3_102;
uint8_t unused3_103;
byte unused3_104;
byte unused3_105;
byte unused3_106;
byte unused3_107;
byte unused3_108;
byte unused3_109;
byte unused3_110;
byte unused3_111;
byte unused3_112;
byte unused3_113;
byte unused3_114;
byte unused3_115;
byte unused3_116;
byte unused3_117;
byte unused3_118;
byte unused3_119;
byte unused3_120 = 200;
byte unused3_121;
byte unused3_122;
byte unused3_123;
byte unused3_124;
byte unused3_125;
byte unused3_126 = 226;
byte unused3_127 = 227;  
};

 //declare io pins
byte pinOut[33]; //digital outputs array is +1 as pins start at 1 not 0

byte pinIn[33];  //digital inputs

byte pinAin[17]; //analog inputs

// declare gearbox config parameters

volatile uint8_t shiftsolenoid[9];     //8 shiftsolenoid (0 is not used) , set to 1 if used, 100 for pwm ,255 if unused
volatile byte lockup_solenoid[3];      // 2 lockup solenoid (0 not used) , set to 1 if used, 100 for pwm ,255 if unused
volatile uint8_t starter_inhibit;       //used or not by gearbox(1 or 255)
volatile uint8_t boxOutput[33] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};        // holds board output port values aligned to gearbox outputs , eg boxOutput[1] == configPage1.shiftsolenoid_1
volatile uint8_t spareOut[33];         // 33 spare outputs(making 32 outputs in total as 0 is not used)
volatile uint8_t gear_port_Enabled[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ;  // range is [1] to [16] , 0 is not used
volatile uint16_t inpin2binary[17] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};  //this converts a pin bit location to the binary value of the bit in the input array
volatile uint8_t gear[9];                 // 8 gears (0 not used)
volatile uint8_t rev_gear;

volatile uint8_t activestate[32] ;      //set via bit 5 on bootup, 0 == active low pin 1 == active high pin


// global variables // from passthrough_example.ino
extern struct statuses currentStatus; // from passthrough.ino
extern struct config1 configPage1;  
extern struct config2 configPage2;
extern struct config3 configPage3;

//extern unsigned long currentLoopTime; //The time the current loop started (uS)
//extern unsigned long previousLoopTime; //The time the previous loop started (uS)

#endif // GLOBALS_H                              

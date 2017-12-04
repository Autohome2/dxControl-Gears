
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

  //only choose one of the following three USExxxx defines for config memory  , comment out the unused ones
  // #define USE_EXT_FLASH
              #define EXT_FLASH_SIZE 8192
              #define FLASH_OFFSET  EXT_FLASH_SIZE / 2
  #define USE_EXT_EEPROM
  //#define USE_EXT_FRAM
  
  extern "C" char* sbrk(int incr); //Used to freeRam
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

//define masks for system status'
#define BIT_SYSTEM_READY                    0
#define BIT_SYSTEM_1                        1
#define BIT_SYSTEM_BURN_GOOD                7


//Define masks for CanStatus
#define BIT_CANSTATUS_CAN0ACTIVATED         0  //can0 has enabled
#define BIT_CANSTATUS_CAN0FAILED            1  //can0 failed to configure
#define BIT_CANSTATUS_CAN1ACTIVATED         2  //can1 has enabled
#define BIT_CANSTATUS_CAN1FAILED            3  //can1 failed o configure
#define BIT_CANSTATUS_CAN0MSGFAIL           4 //
#define BIT_CANSTATUS_CAN1MSGFAIL           5  //
#define BIT_CANSTATUS_7                     6       //
#define BIT_CANSTATUS_8                     7  //

//define maskes for generalConfig1
#define USE_REALTIME_BROADCAST              0
//#define unused1_7b                          1
  
#define maincommand  'r'    // the command to send to the Speeduino
#define commandletterr  'r'
#define tsCanId  0          // this is the tunerstudio canID for the device you are requesting data from , this is 0 for the main ecu in the system which is usually the speeduino ecu . 
                              // this value is set in Tunerstudio when configuring your Speeduino
uint16_t realtime_offset = 4;  // the offset of the realtime data to start reading from
uint16_t realtime_bytes = 1;   // the number of bytes requested


const unsigned char simple_remote_signature[]    = "speeduino_dxControl-Gears 201712"; 
const unsigned char simple_remote_RevNum[] = "speeduino 201712_dxControl-Gears V2.003";

uint8_t thistsCanId = 4;    // this is the tunerstudio canId of this device
const uint8_t data_structure_version = 2; //This identifies the data structure when reading / writing.
const uint8_t page_1_size = 128;
const uint16_t page_2_size = 256;
const uint8_t page_3_size = 128;
const uint16_t page_4_size = 256;

volatile byte tmp;         //used in assembling incoming serial data ints
volatile uint16_t theoffset, thelength;  //used with serial data

//Handy bitsetting macros
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_CHECK(var,pos) ((var) & (1<<(pos)))
#define BIT_sCHECK(var,pos) (((var) & (1<<(pos)))>>pos)       // gives a 1 or 0 answer according to if the bit at pos was 1 or 0

#define BIT_TIMER_1HZ             0
#define BIT_TIMER_4HZ             1
#define BIT_TIMER_5HZ             2
#define BIT_TIMER_10HZ            3
#define BIT_TIMER_15HZ            4
#define BIT_TIMER_20HZ            5
#define BIT_TIMER_25HZ            6
#define BIT_TIMER_30HZ            7

volatile byte TIMER_mask;
volatile byte LOOP_TIMER;

//The status struct contains the current values for all 'live' variables
//In current version this is x bytes
struct statuses {
  volatile byte secl; //Continous
  volatile byte systembits ;
  volatile byte canstatus;    //canstatus bitfield
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
  volatile uint16_t oil_Temp;                    //raw a/d value of oil temp
  volatile uint16_t GPSspeedo;
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
uint16_t master_controller_address:11;
byte pinLayout;
byte speeduinoConnection ;
uint16_t speeduinoBaseCan ;       //speeduino base can address
uint8_t canModuleConfig;                //bit flags for canmodule configuration
uint8_t generalConfig1;
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
uint8_t paddle_change_up       ;//   = bits,   U08,    30,     [0:3] ,   $IN_PORT_NUMBERS
uint8_t paddle_change_down      ;//  = bits,   U08,    31,     [0:3] ,   $IN_PORT_NUMBERS
uint8_t park_in         ;//   = bits,   U08,    32,     [0:3] ,   $IN_PORT_NUMBERS
uint8_t reverse_in      ;//   = bits,   U08,    33,     [0:3] ,   $IN_PORT_NUMBERS
uint8_t neutral_in     ;//    = bits,   U08,    34,     [0:3] ,   $IN_PORT_NUMBERS
uint8_t drive_in       ;//    
uint8_t sport_in       ;//    
uint8_t sport_change_up   ;
uint8_t sport_change_down ;
uint8_t unused63;
uint8_t unused64;
uint8_t unused65;
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
//byte unused74;
//byte unused75;
//byte unused76;
//byte unused77;
//byte unused78;
//byte unused79;
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

//Page 4 of the config -canbus broadcast,obd and config- See the ini file for further reference
//this is laid out as first the byte size data then the words

struct config4 {
uint8_t canbroadcast_config;  
uint16_t canbroadcast_sel;
uint16_t canbroadcast_source_can_address[16];
uint16_t canbroadcast_chan_offset[16];
uint8_t canbroadcast_chan_size[16];
uint8_t canbroadcast_chan_canid[16];
byte unused4_99 = 99;

uint16_t gpio_obd_address;             //GPIO OBD diagnostic address 
uint8_t canbroadcast_freq[16]; 

byte unused4_118;
byte unused4_119;
byte unused4_120 = 200;
byte unused4_121;
byte unused4_122;
byte unused4_123;
byte unused4_124;
byte unused4_125;
byte unused4_126 = 226;
byte unused4_127 = 227;
byte unused4_128;
byte unused4_129;
byte unused4_130 = 200;
byte unused4_131;
byte unused4_132;
byte unused4_133;
byte unused4_134;
byte unused4_135;
byte unused4_136 = 236;
byte unused4_137 = 237;
byte unused4_138;
byte unused4_139;
byte unused4_140;
byte unused4_141;
byte unused4_142;
byte unused4_143;
byte unused4_144;
byte unused4_145;
byte unused4_146;
byte unused4_147;
byte unused4_148;
byte unused4_149;
byte unused4_150;
byte unused4_151;
byte unused4_152;
byte unused4_153;
byte unused4_154;
byte unused4_155;
byte unused4_156;
byte unused4_157;
byte unused4_158;
byte unused4_159;
byte unused4_160;
byte unused4_161;
byte unused4_162;
byte unused4_163;
byte unused4_164;
byte unused4_165;
byte unused4_166;
byte unused4_167;
byte unused4_168;
byte unused4_169;
byte unused4_170;
byte unused4_171;
byte unused4_172;
byte unused4_173;
byte unused4_174;
byte unused4_175;
byte unused4_176;
byte unused4_177;
byte unused4_178;
byte unused4_179;
byte unused4_180;
byte unused4_181;
byte unused4_182;
byte unused4_183;
byte unused4_184;
byte unused4_185;
byte unused4_186;
byte unused4_187;
byte unused4_188;
byte unused4_189;
byte unused4_190;
byte unused4_191;
byte unused4_192;
byte unused4_193;
byte unused4_194;
byte unused4_195;
byte unused4_196;
byte unused4_197;
byte unused4_198;
byte unused4_199;
byte unused4_200 = 200;
byte unused4_201;
byte unused4_202;
byte unused4_203;
byte unused4_204;
byte unused4_205;
byte unused4_206 = 206;
byte unused4_207 = 207;  
byte unused4_208;
byte unused4_209;
byte unused4_210;
byte unused4_211;
byte unused4_212;
byte unused4_213;
byte unused4_214;
byte unused4_215;
byte unused4_216;
byte unused4_217;
byte unused4_218;
byte unused4_219;
byte unused4_220;
byte unused4_221;
byte unused4_222;
byte unused4_223;
byte unused4_224;
byte unused4_225;
byte unused4_226;
byte unused4_227;
byte unused4_228;
byte unused4_229;
byte unused4_230;
byte unused4_231;
byte unused4_232;
byte unused4_233;
byte unused4_234;
byte unused4_235;
byte unused4_236;
byte unused4_237;
byte unused4_238;
byte unused4_239;
byte unused4_240;
byte unused4_241;
byte unused4_242;
byte unused4_243;
byte unused4_244;
byte unused4_245;
byte unused4_246;
byte unused4_247;
byte unused4_248;
byte unused4_249;
byte unused4_250;
byte unused4_251;
byte unused4_252;
byte unused4_253; 
byte unused4_254;
byte unused4_255; 

#if defined(CORE_AVR)
  };
#else
  } __attribute__((__packed__)); //The 32 bit systems require all structs to be fully packed
#endif

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
extern struct config4 configPage4;

//extern unsigned long currentLoopTime; //The time the current loop started (uS)
//extern unsigned long previousLoopTime; //The time the previous loop started (uS)

#endif // GLOBALS_H                              

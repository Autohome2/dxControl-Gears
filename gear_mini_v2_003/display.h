#ifndef DISPLAY_H
#define DISPLAY_H

//#define USE_SSD1106
#define USE_SSH1106
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//display module options . only choose one of the following defines for display selection  , comment out the unused ones 
//set display 1 type
#define DISP1_ACTIVE 1              // 1 if in use or comment out
#define DISP1_I2C_ROUTE  0               // direct i2c == 0 , specified address == screen address, remote == mux address
#define DISP1_USE_SSD1106_I2C
//#define DISP1_USE_SSD1306_I2C
//#define DISP1_USE_SSD1106_SPI
//#define DISP1_USE_SSH1106_SPI

//set display 2 type
//#define DISP2_ACTIVE 1 
#define DISP2_I2C_ROUTE  0               // direct i2c == 0 , specified address == screen address, remote == mux address
#define DISP2_USE_SSD1106_I2C
//#define DISP2_USE_SSD1306_I2C
//#define DISP2_USE_SSD1106_SPI
//#define DISP2_USE_SSH1106_SPI

//set display size
#define USE_128x64
//#define USE_128x128

//set pins for screens in spi mode
//display 1
#define DISPLAY1_CS 7                //pin to use for CS
#define DISPLAY1_DC 6                //pin to use for DC
#define DISPLAY1_RESET 8 

//display 2
#define DISPLAY2_CS 7                //pin to use for CS
#define DISPLAY2_DC 6                //pin to use for DC
#define DISPLAY2_RESET 8


//SPI NOTES:
//spi on mega2560 mosi == 51, clk(sck) == 52
//I2C NOTES:
// i2c on the mega2560 sda == 20, scl == 21

//void configure_display_type();
void initialise_display();

void update_display();
void manual_auto();
void gear_letter();
void gps_line();
void oiltemp_line();
void selector_line();

uint8_t disp_blink = 0;

#endif

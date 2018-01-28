#ifndef DISPLAY_H
#define DISPLAY_H


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//display module options .  
//setup display 1
#define DISP1_ACTIVE     1             // 0 == DISABLED , 1 == ENABLED
#define DISP1_SPI_I2C    1             // 0 == SPI , 1 == I2C

//if I2C set these next three values
#define DISP1_ADDRESS    0             //direct i2c == 0 , specified address == screen address
#define DISP1_I2C_ROUTE  0             // direct i2c == 0 , via mux == mux address
#define DISP1_ROUTE_OUT  0             // the output port on the tca9548 screen is connected to if mux in use

//only choose one of the following defines for display type selection  , comment out the unused ones
#define DISP1_USE_SSD1106_I2C
//#define DISP1_USE_SSD1306_I2C
//#define DISP1_USE_SSD1106_SPI
//#define DISP1_USE_SSH1106_SPI
//#define DISP1_USE_4x20_I2C_EXP

//set pins for screens in spi mode
//display 1
#define DISPLAY1_CS      7             //pin to use for CS
#define DISPLAY1_DC      6             //pin to use for DC
#define DISPLAY1_RESET   8 

//---------------------------------------------------------------------------------------------------------

//setup display 2
#define DISP2_ACTIVE     0             // 0 == DISABLED , 1 == ENABLED
#define DISP2_SPI_I2C    1             // 0 == SPI , 1 == I2C

//if I2C set these next three values
#define DISP2_ADDRESS    0             //direct i2c == 0 , specified address == screen address
#define DISP2_I2C_ROUTE  0             // direct i2c == 0 , via mux == mux address
#define DISP2_ROUTE_OUT  1             // the output port on the tca9548 screen is connected to if mux in use

//only choose one of the following defines for display type selection  , comment out the unused ones
#define DISP2_USE_SSD1106_I2C
//#define DISP2_USE_SSD1306_I2C
//#define DISP2_USE_SSD1106_SPI
//#define DISP2_USE_SSH1106_SPI
//#define DISP2_USE_4x20_I2C_EXP

//set pins for screens in spi mode
//display 2
#define DISPLAY2_CS      7             //pin to use for CS
#define DISPLAY2_DC      6             //pin to use for DC
#define DISPLAY2_RESET   8

//-----------------------------------------------------------------------------------------------------

//set display size
#define USE_128x64
//#define USE_128x128

//SPI NOTES:
//spi on mega2560 mosi == 51, clk(sck) == 52
//I2C NOTES:
// i2c on the mega2560 sda == 20, scl == 21

void initialise_display();
void tcaselect(uint8_t i, uint8_t dispnum);
void update_display();
void manual_auto();
void gear_letter();
void gps_line();
void oiltemp_line();
void selector_line();

uint8_t disp_blink = 0;

#endif

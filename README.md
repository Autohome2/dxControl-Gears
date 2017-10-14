# Speeduino-GearControl
The Speeduino Gearbox controller module is based on the Gpio from the same family.
Currently on Mega2560 MCU support is provided but STM32 and Teensy3.X will follow with some mcu specific features added due to the extra processing power those mcu offer. 
The firmware supports up to 8 forward gears controlled by up to 8 selection solenoids, two torque-convertor lockup solenoids and a further 21 outputs spare capacity for future expansion.

Initially a manual change only standalone unit , later improvements will be link to speeduino , Vss sensors, GPS input ,pwm outputs ,fully automatic operation and CANBUS connectivity etc.

IO pins are selected via board configurations in the same way as GPIO does. 
The gearbox selection is also made in a similar way with the gearbox config being defined in the utils file.Pre defined gearboxes are selected via TS menu. Curent support is for the A340E with 5r55 and A4ODE support planned to follow ,although any gearbox could be included if defined.

The unit is fully programmable via Tunerstudio.

Version History

V1.000

Paddle shift with up to eight gears and 8 solenoids.Torque convertor Lockup by manual switch only 

V2.000(Still Beta)

Serial3 link to Speeduino to rear up to 16 data channels.External data channel can be used as input to universal io.
Placeholders for Vss input for Speed

V2.001(beta test only)

Add in gear flash display when mcu is booted with selector in gear.

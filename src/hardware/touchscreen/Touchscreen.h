/*
*
* Touchscreen - FT6206 or XPT2046 controller
*
* DIY-Thermocam Firmware
*
* GNU General Public License v3.0
*
* Copyright by Max Ritter
*
* http://www.diy-thermocam.net
* https://github.com/maxritter/DIY-Thermocam
*
*/

#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

/*################################# INCLUDES ##################################*/

#include <point.h>

/*################# PUBLIC CONSTANTS, VARIABLES & DATA TYPES ##################*/

extern volatile bool touch_capacitive;

/*########################## PUBLIC PROCEDURES ################################*/

TS_Point touch_getPoint();
void touch_init();
void touch_setRotation(bool rotated);
volatile bool touch_touched();

#ifdef __cplusplus
}
#endif

#endif /* TOUCHSCREEN_H */

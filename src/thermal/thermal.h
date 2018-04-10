/*
*
* THERMAL - Main functions in the live mode
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

#ifndef _THERMAL_H
#define _THERMAL_H

#ifdef __cplusplus
extern "C" {
#endif

void touchIRQ();
void buttonIRQ();
void longTouchHandler();
void showColorBar();
void changeDisplayOptions(byte* pos);
void selectColorScheme();
void changeColorScheme(byte* pos);
void showImage();
void liveModeInit();
void liveMode();

#ifdef __cplusplus
}
#endif

#endif /* _THERMAL_H */

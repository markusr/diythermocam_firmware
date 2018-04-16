/*
*
* MASS STORAGE -  Mass storage mode to connect the internal storage to the PC
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

#ifndef MASSSTORAGE_H
#define MASSSTORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/*########################## PUBLIC PROCEDURES ################################*/

void jumpToApplicationAt0x38980();
void massStorage();
void resetPeripherals();
void restartAndJumpToApp(void);
void startup_late_hook(void);

#ifdef __cplusplus
}
#endif

#endif /* MASSSTORAGE_H */

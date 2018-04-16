/*
*
* COLOR SCHEMES - Contains 19 different color schemes to display the thermal image
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

#ifndef COLORSCHEMES_H
#define COLORSCHEMES_H

#ifdef __cplusplus
extern "C" {
#endif

/*################# PUBLIC CONSTANTS, VARIABLES & DATA TYPES ##################*/

extern const byte colorMap_arctic[240];
extern const byte colorMap_blackHot[224];
extern const byte colorMap_blueRed[192];
extern const byte colorMap_coldest[224];
extern const byte colorMap_contrast[224];
extern const byte colorMap_doubleRainbow[256];
extern const byte colorMap_grayRed[224];
extern const byte colorMap_glowBow[224];
extern const byte colorMap_grayscale[256];
extern const byte colorMap_hottest[224];
extern const byte colorMap_ironblack[256];
extern const byte colorMap_lava[240];
extern const byte colorMap_medical[224];
extern const byte colorMap_rainbow[256];
extern const byte colorMap_wheel1[256];
extern const byte colorMap_wheel2[256];
extern const byte colorMap_wheel3[256];
extern const byte colorMap_whiteHot[224];
extern const byte colorMap_yellow[224];

#ifdef __cplusplus
}
#endif

#endif /* COLORSCHEMES_H */

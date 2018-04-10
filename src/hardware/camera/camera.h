/*
*
* Camera - Visual camera module
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

#ifndef CAMERA_H
#define CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

void camera_capture(void);
void camera_changeRes(byte camRes);
unsigned int camera_decompIn(JDEC * jd, byte* buff, unsigned int ndata);
unsigned int camera_decompOutCombined(JDEC * jd, void * bitmap, JRECT * rect);
unsigned int camera_decompOutNormal(JDEC * jd, void * bitmap, JRECT * rect);
void camera_get(byte mode, char* dirname = NULL);
void camera_init(void);
void camera_setDisplayRes();
void camera_setSaveRes();
void camera_setStreamRes();

#ifdef __cplusplus
}
#endif

#endif /* CAMERA_H */

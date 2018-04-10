/*
*
* CREATE - Functions to create and display the thermal frameBuffer
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

#ifndef _CREATE_H
#define _CREATE_H

#ifdef __cplusplus
extern "C" {
#endif

void gaussianFilter();
void boxFilter();
void resizePixels(unsigned short* pixels, int w1, int h1, int w2, int h2);
void resizeImage();
void calcFillPixel(uint16_t x, uint16_t y);
void fillEdges();
void smallToBigBuffer(bool trans);
void clearTempPoints();
void showTemperatures();
void getTouchPos(uint16_t* x, uint16_t* y);
void tempPointFunction(bool remove);
void limitValues();
void getHotColdColors(byte* red, byte* green, byte* blue);
void convertColors(bool small);
void refreshMinMax();
void refreshTempPoints();
void calculatePointPos(int16_t* xpos, int16_t* ypos, uint16_t pixelIndex);
void createThermalImg(bool small);
void createVisCombImg();

#ifdef __cplusplus
}
#endif

#endif /* _CREATE_H */

/*
*
* CALIBRATION - Functions to convert Lepton raw values to absolute values
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

#ifndef _CALIBRATION_H
#define _CALIBRATION_H

#ifdef __cplusplus
extern "C" {
#endif

float celciusToFahrenheit(float Tc);
float fahrenheitToCelcius(float Tf);
float calFunction(uint16_t rawValue);
uint16_t tempToRaw(float temp);
uint16_t calcAverage();
void compensateCalib();
void checkWarmup();
void calibrationProcess(bool serial, bool firstStart);
bool calibration();

#ifdef __cplusplus
}
#endif

#endif /* _CALIBRATION_H */

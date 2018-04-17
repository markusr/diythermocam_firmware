/*
*
* LEPTON - Access the FLIR Lepton LWIR module
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

#ifndef LEPTON_H
#define LEPTON_H

/*################# PUBLIC CONSTANTS, VARIABLES & DATA TYPES ##################*/

//Lepton frame error return
enum LeptonReadError {
	NONE, DISCARD, SEGMENT_ERROR, ROW_ERROR, SEGMENT_INVALID
};

/*########################## PUBLIC PROCEDURES ################################*/

void lepton_begin();
void lepton_end();
bool lepton_ffc(bool message = false);
void lepton_ffcMode(bool automatic);
LeptonReadError lepton_getPackage(byte line, byte seg);
void lepton_getRawValues();
void lepton_init();
int lepton_readReg(byte reg);
void lepton_reset();
void lepton_setReg(byte reg);
float lepton_spotTemp();
void lepton_version();
bool savePackage(byte line, byte segment = 0);

#endif /* LEPTON_H */

/*
*
* LOAD - Load images and videos from the internal storage
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

#ifndef _LOAD_H
#define _LOAD_H

#ifdef __cplusplus
extern "C" {
#endif

void clearData();
void displayRawData();
void loadBMPImage(char* filename);
bool isImage(char* filename);
void readTempPoints();
void loadRawData(char* filename, char* dirname);
bool yearChoose(char* filename);
bool monthChoose(bool* months, char* filename);
bool dayChoose(bool* days, char* filename);
bool hourChoose(bool* hours, char* filename) ;
bool minuteChoose(bool* minutes, char* filename);
bool secondChoose(bool* seconds, char* filename);
void copyIntoBuffers(char* filename);
bool checkFileValidity();
void checkFileStructure(bool* check);
void checkFileEnding(bool* check, char* filename);
bool findFile(char* filename, bool next, bool restart, int* position = 0, char* compare = NULL);
void searchFiles();
void chooseFile(char* filename);
bool loadDelete(char* filename, int* pos);
void loadFind(char* filename, int* pos);
void loadAlloc();
void loadSettings();
void loadDeAlloc();
void loadTouchIRQ();

#ifdef __cplusplus
}
#endif

#endif /* _LOAD_H */

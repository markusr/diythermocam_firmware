/*
*
* SAVE - Save images and videos to the internal storage
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

#ifndef _SAVE_H
#define _SAVE_H

#ifdef __cplusplus
extern "C" {
#endif

void createSDName(char* filename, boolean folder);
void createBMPFile(char* filename);
void createJPEGFile(char* dirname);
void createVideoFolder(char* dirname);
void imgSaveStart();
void frameFilename(char* filename, uint16_t count);
void saveVideoFrame(char* filename, char* dirname);
void processVideoFrames(int framesCaptured, char* dirname);
void saveRawData(bool isImage, char* name, uint16_t framesCaptured);
void imgSaveEnd();
void saveBuffer(char* filename);

#ifdef __cplusplus
}
#endif

#endif /* _SAVE_H */

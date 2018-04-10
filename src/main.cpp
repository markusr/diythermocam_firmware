// Do not remove the include below
#include "Arduino.h"

/*
*
* MAIN SKETCH - Main entry point for the firmware
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

/* Current firmware version */
#define Version "Firmware 2.45 from 06.04.2018"
#define fwVersion 245

/* External Libraries */

#include <SPI.h>
#include <i2c_t3.h>
#include <EEPROM.h>
#include <ADC.h>
#include <Metro.h>
#include <Time.h>
#include <Bounce.h>
#include <SdFat.h>

/* General Includes */

#include <GlobalDefines.h>
#include <GlobalVariables.h>
#include <GlobalMethods.h>
#include <ColorSchemes.h>

/* Modules */

#include <Hardware.h>
#include <GUI.h>
#include <Thermal.h>

/* Main Entry point */

void setup()
{
	//Init the hardware components
	initHardware();

	//Check for firmware upgrade done
	checkFWUpgrade();

	//Enter USB connection if no display attached
	if (checkNoDisplay())
		serialInit();

	//Check for hardware issues
	checkDiagnostic();

	//Do the first start setup if required
	if (checkFirstStart())
		firstStart();

	//Read all settings from EEPROM
	readEEPROM();

	//Show the live mode helper if required
	if (checkLiveModeHelper())
		liveModeHelper();

	//Go to the live Mode
	liveMode();
}

/* Loop not used */

void loop()
{
}

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

/*################################# INCLUDES ##################################*/

#include <Arduino.h>
#include <globaldefines.h>
#include <globalvariables.h>
#include <hardware.h>
#include <SPI.h>
#include <i2c_t3.h>
#include <EEPROM.h>
#include <gui.h>
#include <lepton.h>

/*######################### STATIC DATA DECLARATIONS ##########################*/

//Array to store one Lepton frame
static byte leptonFrame[164];

/*######################## PUBLIC FUNCTION BODIES #############################*/

/* Start Lepton SPI Transmission */
void lepton_begin() {
	//Start alternative clock line, except for old HW
	if (mlx90614Version == mlx90614Version_new)
		startAltClockline();

	//For Teensy  3.1 / 3.2 and Lepton3 use this one
	if ((teensyVersion == teensyVersion_old) && ((leptonVersion == leptonVersion_3_0_shutter) || (leptonVersion == leptonVersion_3_5_shutter)))
		SPI.beginTransaction(SPISettings(30000000, MSBFIRST, SPI_MODE0));

	//Otherwise use 20 Mhz maximum and SPI mode 1
	else
		SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE1));

	//Start transfer  - CS LOW
	digitalWrite(pin_lepton_cs, LOW);
}

/* End Lepton SPI Transmission */
void lepton_end() {
	//End transfer - CS HIGH
	digitalWriteFast(pin_lepton_cs, HIGH);

	//End SPI Transaction
	SPI.endTransaction();

	//End alternative clock line, except for old HW
	if (mlx90614Version == mlx90614Version_new)
		endAltClockline();
}

/* Reset the SPI bus to re-initiate Lepton communication */
void lepton_reset()
{
	lepton_end();
	delay(186);
	lepton_begin();
}

/* Store one package of 80 columns into RAM */
bool savePackage(byte line, byte segment) {
	//Go through the video pixels for one video line
	for (int column = 0; column < 80; column++) {
		//Apply horizontal mirroring
		if (rotationHorizont)
			column = 79 - column;

		//Make a 16-bit rawvalue from the lepton frame
		uint16_t result = (uint16_t)(leptonFrame[2 * column + 4] << 8
			| leptonFrame[2 * column + 5]);

		//Discard horizontal mirroring
		if (rotationHorizont)
			column = 79 - column;

		//Invalid value, return
		if (result == 0) {
			return 0;
		}

		//Lepton2.x
		if ((leptonVersion != leptonVersion_3_0_shutter) && (leptonVersion != leptonVersion_3_5_shutter)) {
			//Rotated or old hardware version
			if (((mlx90614Version == mlx90614Version_old) && (!rotationVert)) ||
				((mlx90614Version == mlx90614Version_new) && (rotationVert))) {
				smallBuffer[(line * 2 * 160) + (column * 2)] = result;
				smallBuffer[(line * 2 * 160) + (column * 2) + 1] = result;
				smallBuffer[(line * 2 * 160) + 160 + (column * 2)] = result;
				smallBuffer[(line * 2 * 160) + 160 + (column * 2) + 1] = result;
			}
			//Non-rotated
			else {
				smallBuffer[19199 - ((line * 2 * 160) + (column * 2))] = result;
				smallBuffer[19199 - ((line * 2 * 160) + (column * 2) + 1)] = result;
				smallBuffer[19199 - ((line * 2 * 160) + 160 + (column * 2))] = result;
				smallBuffer[19199 - ((line * 2 * 160) + 160 + (column * 2) + 1)] = result;
			}
		}

		//Lepton3
		else {
			//Non-rotated
			if (!rotationVert) {
				switch (segment) {
				case 1:
					if (rotationHorizont)
						smallBuffer[19199 - (((line / 2) * 160) + ((1 - (line % 2)) * 80) + (column))] = result;
					else
						smallBuffer[19199 - (((line / 2) * 160) + ((line % 2) * 80) + (column))] = result;
					break;
				case 2:
					if (rotationHorizont)
						smallBuffer[14399 - (((line / 2) * 160) + ((1 - (line % 2)) * 80) + (column))] = result;
					else
						smallBuffer[14399 - (((line / 2) * 160) + ((line % 2) * 80) + (column))] = result;
					break;
				case 3:
					if (rotationHorizont)
						smallBuffer[9599 - (((line / 2) * 160) + ((1 - (line % 2)) * 80) + (column))] = result;
					else
						smallBuffer[9599 - (((line / 2) * 160) + ((line % 2) * 80) + (column))] = result;
					break;
				case 4:
					if (rotationHorizont)
						smallBuffer[4799 - (((line / 2) * 160) + ((1 - (line % 2)) * 80) + (column))] = result;
					else
						smallBuffer[4799 - (((line / 2) * 160) + ((line % 2) * 80) + (column))] = result;
					break;
				}
			}
			//Rotated
			else {
				switch (segment) {
				case 1:
					smallBuffer[((line / 2) * 160) + ((line % 2) * 80) + (column)] = result;
					break;
				case 2:
					smallBuffer[4800 + (((line / 2) * 160) + ((line % 2) * 80) + (column))] = result;
					break;
				case 3:
					smallBuffer[9600 + (((line / 2) * 160) + ((line % 2) * 80) + (column))] = result;
					break;
				case 4:
					smallBuffer[14400 + (((line / 2) * 160) + ((line % 2) * 80) + (column))] = result;
					break;
				}
			}
		}

	}

	//Everything worked
	return 1;
}

/* Get one line package from the Lepton */
LeptonReadError lepton_getPackage(byte line, byte seg) {
	//Receive one frame over SPI
	SPI.transfer(leptonFrame, 164);

	//Repeat as long as the frame is not valid, equals sync
	if ((leptonFrame[0] & 0x0F) == 0x0F)
		return DISCARD;

	//Check if the line number matches the expected line
	if (leptonFrame[1] != line)
		return ROW_ERROR;

	//For the Lepton3.x, check if the segment number matches
	if ((line == 20) && ((leptonVersion == leptonVersion_3_0_shutter) || (leptonVersion == leptonVersion_3_5_shutter))) {
		byte segment = (leptonFrame[0] >> 4);
		if (segment == 0)
			return SEGMENT_INVALID;
		if (segment != seg)
			return SEGMENT_ERROR;
	}

	//Everything worked
	return NONE;
}

/* Get one frame of raw values from the lepton */
void lepton_getRawValues()
{
	byte line, error, segmentNumbers;

	//Determine number of segments
	if ((leptonVersion == leptonVersion_3_0_shutter) || (leptonVersion == leptonVersion_3_5_shutter))
		segmentNumbers = 4;
	else
		segmentNumbers = 1;

	//Begin SPI Transmission
	lepton_begin();

	//Go through the segments
	for (byte segment = 1; segment <= segmentNumbers; segment++) {
		//Reset error counter for each segment
		error = 0;

		//Go through one segment, equals 60 lines of 80 values
		do {
			for (line = 0; line < 60; line++) {
				//Maximum error count
				if (error == 255) {
					//If main menu should be entreed
					if (showMenu == showMenu_desired) {
						lepton_end();
						return;
					}

					//Reset segment
					segment = 1;
					//Reset error
					error = 0;
					//Reset Lepton SPI
					lepton_reset();
					//Restart at line 0
					break;
				}

				//Get a package from the lepton
				LeptonReadError retVal = lepton_getPackage(line, segment);

				//If everythin worked, continue
				if (retVal == NONE)
					if (savePackage(line, segment))
						continue;

				//Raise lepton error
				error++;

				//Stabilize framerate
				uint32_t time = micros();
				while ((micros() - time) < 800)
					__asm__ volatile ("nop");

				//Restart at line 0
				break;
			}
		} while (line != 60);
	}

	//End SPI Transmission
	lepton_end();
}

/* Trigger a flat-field-correction on the Lepton */
bool lepton_ffc(bool message) {
	//Show a message for main menu
	if (message)
	{
		//When in manual temperature mode, a FFC is not possible
		if (!autoMode)
		{
			showFullMessage((char*) "No FFC in manual mode", true);
			delay(1000);
			return false;
		}
		showFullMessage((char*) "Performing FFC..", true);
	}

	//Send FFC run command
	Wire.beginTransmission(0x2A);
	Wire.write(0x00);
	Wire.write(0x04);

	//For radiometric Lepton, send RAD FFC command
	if ((leptonVersion == leptonVersion_2_5_shutter) || (leptonVersion == leptonVersion_3_5_shutter)){
		Wire.write(0x4E);
		Wire.write(0x2E);
	}
	//For all others, send normal FFC command
	else {
		Wire.write(0x02);
		Wire.write(0x42);
	}

	//Get error byte
	byte error = Wire.endTransmission();

	//Wait some time when in main menu
	if (message)
		delay(2000);

	return error;
}

/* Select I2C Register on the Lepton */
void lepton_setReg(byte reg) {
	Wire.beginTransmission(0x2A);
	Wire.write(reg >> 8 & 0xff);
	Wire.write(reg & 0xff);
	Wire.endTransmission();
}

/* Read I2C Register on the lepton */
int lepton_readReg(byte reg) {
	uint16_t reading;
	lepton_setReg(reg);
	Wire.requestFrom(0x2A, 2);
	reading = Wire.read();
	reading = reading << 8;
	reading |= Wire.read();
	return reading;
}

/* Get the spotmeter value on a radiometric lepton */
float lepton_spotTemp() {
	//Get RAD spotmeter value
	Wire.beginTransmission(0x2A);
	Wire.write(0x00);
	Wire.write(0x04);
	Wire.write(0x4E);
	Wire.write(0xD0);
	byte error = Wire.endTransmission();

	//Lepton I2C error, set diagnostic
	if (error != 0) {
		setDiagnostic(diag_spot);
		return 0;
	}

	//Transfer the new package
	Wire.beginTransmission(0x2A);
	while (lepton_readReg(0x2) & 0x01);
	Wire.requestFrom(0x2A, lepton_readReg(0x6));
	byte response[8];
	Wire.readBytes(response, 8);
	Wire.endTransmission();

	//Calculate spot temperature in Kelvin
	float spotTemp = (response[0] * 256.0) + response[1];
	//Multiply by correction factor
	spotTemp *= 0.01;
	//Convert to celsius
	spotTemp -= 273.15;

	return spotTemp;
}

/* Set the shutter operation to manual/auto */
void lepton_ffcMode(bool automatic)
{
	//If there is no shutter, return
	if (leptonShutter == leptonShutter_none)
		return;

	//When enabling auto FFC, check for some factors
	if ((automatic) && ((hotColdMode != hotColdMode_disabled) || (autoMode == false) || (limitsLocked == true)))
		return;

	//Contains the standard values for the FFC mode
	byte package[] = { automatic, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 224, 147, 4, 0, 0, 0, 0, 0, 44, 1, 52, 0 };

	//Data length
	Wire.beginTransmission(0x2A);
	Wire.write(0x00);
	Wire.write(0x06);
	Wire.write(0x00);
	Wire.write(sizeof(package));
	Wire.endTransmission();

	//Send package
	Wire.beginTransmission(0x2A);
	Wire.write(0x00);
	Wire.write(0x08);
	for (byte i = 0; i < sizeof(package); i++)
		Wire.write(package[i]);
	Wire.endTransmission();

	//SYS module with FFC Mode Set
	Wire.beginTransmission(0x2A);
	Wire.write(0x00);
	Wire.write(0x04);
	Wire.write(0x02);
	Wire.write(0x3D);
	Wire.endTransmission();

	//Set shutter mode
	if (automatic)
		leptonShutter = leptonShutter_auto;
	else
		leptonShutter = leptonShutter_manual;
}

/* Checks the Lepton hardware revision */
void lepton_version() {
	//Get AGC Command
	Wire.beginTransmission(0x2A);
	Wire.write(0x00);
	Wire.write(0x04);
	Wire.write(0x48);
	Wire.write(0x1C);
	byte error = Wire.endTransmission();

	//Lepton I2C error, set diagnostic
	if (error != 0) {
		setDiagnostic(diag_lep_conf);
		leptonVersion = leptonVersion_2_0_noShutter;
		return;
	}

	//Transfer the new package
	Wire.beginTransmission(0x2A);
	while (lepton_readReg(0x2) & 0x01);
	Wire.requestFrom(0x2A, lepton_readReg(0x6));
	char leptonhw[33];
	Wire.readBytes(leptonhw, 32);
	Wire.endTransmission();

	//Detected Lepton2 Non-Shuttered
	if (strstr(leptonhw, "05-060340") != NULL) {
		leptonVersion = leptonVersion_2_0_noShutter;
		leptonShutter = leptonShutter_none;
	}

	//Detected Lepton2 Shuttered
	else if (strstr(leptonhw, "05-060950") != NULL) {
		leptonVersion = leptonVersion_2_0_shutter;
		leptonShutter = leptonShutter_auto;
	}

	//Detected Lepton2.5 Shuttered (Radiometric)
	else if (strstr(leptonhw, "05-070360") != NULL) {
		leptonVersion = leptonVersion_2_5_shutter;
		leptonShutter = leptonShutter_autoRAD;
	}

	//Detected Lepton3 Shuttered
	else if (strstr(leptonhw, "05-070620") != NULL) {
		leptonVersion = leptonVersion_3_0_shutter;
		leptonShutter = leptonShutter_auto;
	}

	//Detected Lepton3.5 Shuttered (Radiometric)
	else if (strstr(leptonhw, "05-070170") != NULL) {
		leptonVersion = leptonVersion_3_5_shutter;
		leptonShutter = leptonShutter_autoRAD;
	}

	//Detected unknown Lepton2 No-Shutter
	else {
		leptonVersion = leptonVersion_2_0_noShutter;
		leptonShutter = leptonShutter_none;
	}

	//Write to EEPROM
	EEPROM.write(eeprom_leptonVersion, leptonVersion);
}

void lepton_set_sys_gain_mode(byte mode){
	if (mode > 2){
		return;
	}
	Wire.beginTransmission(0x2A);
	// CCI/TWI Data Registers is at 0x0008
	Wire.write(0x00);
	Wire.write(0x08);
	// size is 32 bits --> so we have 4 bytes
	// Lower 16 bits go to DATA0, hiher 16 bits go to DATA1
	Wire.write(0x00); //MSB of DATA0
	Wire.write(mode); //LSB of DATA0
	Wire.write(0x00); //MSB of DATA1
	Wire.write(0x00); //LSB of DATA1
	Wire.write(0x00);
	Wire.endTransmission();

	//Set data length to 4
	Wire.beginTransmission(0x2A);
	// CCI/TWI Data Length Register is at 0x0006
	Wire.write(0x00);
	Wire.write(0x06);
	//Data length bytes
	Wire.write(0x00);
	Wire.write(0x04);
	Wire.endTransmission();

	// Execute the SYS Gain Mode Set Command, so that the module applies the values
	Wire.beginTransmission(0x2A);
	Wire.write(0x00);
	Wire.write(0x04); //COMMANDID_REG
	// 0x0200 (SDK Module ID) + 0x48 (SDK Command ID) + 0x1 (SET operation) + 0x0000 (Protection Bit) = 0x0249.
	Wire.write(0x02); 
	Wire.write(0x49);
	Wire.endTransmission();
}

void lepton_set_sys_gain_high(){
	lepton_set_sys_gain_mode(0x00);
}

void lepton_set_sys_gain_low(){
	lepton_set_sys_gain_mode(0x01);
}

void lepton_set_sys_gain_auto(){
	lepton_set_sys_gain_mode(0x02);
}

/*
 * Returns the SYS Gain Mode
 * 0: high mode (hardware default),
 * 1: low mode,
 * 2: auto mode
 * The measurement range for the Lepton 3.5 is (see datasheet for details):
 * High Gain Mode: 	-10 to +140 deg C
 * Low Gain Mode: 	-10 to +450 deg C
 * 
 * Returns -1 if the gain mode could not be read
 */
int lepton_get_sys_gain_mode(){

	byte data[4];
	int data_length;

	//SYS Gain Mode Get Command
	Wire.beginTransmission(0x2A);
	Wire.write(0x00);
	Wire.write(0x04); //COMMANDID_REG
	// 0x0200 (SDK Module ID) + 0x48 (SDK Command ID) + 0x0 (GET operation) + 0x0000 (Protection Bit) = 0x0248.
	Wire.write(0x02);
	Wire.write(0x48);
	Wire.endTransmission();

	// Wait for execution of the command
	while (lepton_readReg(0x2) & 0x01);

	// Read the data length (should be 4)
	data_length = lepton_readReg(0x6);

	if (data_length != 4){
		return -1;
	}

	Wire.requestFrom(0x2A, data_length);
	Wire.readBytes(data, data_length);
	Wire.endTransmission();

	//The enum value is the LSB of DATA0, see set_gain_mode function
	return data[1];
}

/* Init the FLIR Lepton LWIR sensor */
void lepton_init() {
	//Check the Lepton HW Revision
	lepton_version();

	//For Lepton 3.5 set to low gain mode which allows to measure up to 450 deg C.
	if (leptonVersion == leptonVersion_3_5_shutter)
		lepton_set_sys_gain_low();

	//For radiometric Lepton, set calibration to done
	if ((leptonVersion == leptonVersion_2_5_shutter) ||(leptonVersion == leptonVersion_3_5_shutter))
		calStatus = cal_standard;

	//Otherwise init warmup timer
	else {
		//Set the calibration timer
		calTimer = millis();
		//Set calibration status to warmup if not coming from mass storage
		calStatus = cal_warmup;
	}

	//Set the compensation value to zero
	calComp = 0;

	//Activate auto mode
	autoMode = true;
	//Deactivate limits Locked
	limitsLocked = false;

	//Check if SPI works
	lepton_begin();
	do {
		SPI.transfer(leptonFrame, 164);
	}
	//Repeat as long as the frame is not valid, equals sync
	while (((leptonFrame[0] & 0x0F) == 0x0F) && ((millis() - calTimer) < 1000));
	lepton_end();

	//If sync not received after a second, set diagnostic
	if ((leptonFrame[0] & 0x0F) == 0x0F)
		setDiagnostic(diag_lep_data);
}

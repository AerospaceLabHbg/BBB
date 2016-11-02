/*
 * ADXL345.h  Created on: 17 May 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone" 
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European 
 * Union Public License (the "EUPL") version 1.1 as published by the 
 * European Union. Any use of this Software, other than as authorized 
 * under this License is strictly prohibited (to the extent such use 
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and 
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#ifndef BMP180_H_
#define BMP180_H_
#include"I2CDevice.h"

/// The BMP180 has 0x100 registers
#define BUFFER_SIZE 0x40

namespace exploringBB {

/**
 * @class ADXL345
 * @brief Specific class for the ADXL345 Accelerometer that is a child of the I2CDevice class
 * Protected inheritance means that the public I2CDevice methods are not publicly accessible
 * by an object of the ADXL345 class.
 */
class BMP180:protected I2CDevice{

public:
	/// An enumeration to define the oversampling of the sensor
	enum OVERSAMPLING_SETTING {
		OSS_0 			= 4500,// delay 4,5 ms
		OSS_1	 		= 7500,//delay 7,5 ms
		OSS_2	 		= 13500,//delay 13,5 ms
		OSS_3	 		= 25500 //delay 25,5 ms
	};


private:
	unsigned int I2CBus, I2CAddress;
	unsigned char *registers;
	BMP180::OVERSAMPLING_SETTING oversampling_setting;
	short ac1, ac2, ac3, b1, b2, mb, mc, md; // raw 2's complement values
	unsigned short ac4, ac5, ac6; // raw 2's complement values
	long ut, up, x1, x2, b5, t, b6, x3, b3, p;
	unsigned long b4, b7;
	long combineRegisters16(unsigned char msb, unsigned char lsb);
	long combineRegisters24(unsigned char msb, unsigned char lsb, unsigned char xlsb);
	void calculateTemperature();
	void calculatePressure();
	virtual int updateRegisters();

public:
	BMP180(unsigned int I2CBus, unsigned int I2CAddress=0x77);
	virtual int readSensorState();

	/*virtual void setRange(ADXL345::RANGE range);
	virtual BMP180::RANGE getRange() { return this->range; }
	virtual void setResolution(ADXL345::RESOLUTION resolution);
	virtual BMP180::RESOLUTION getResolution() { return this->resolution; }*/
	virtual void display(int iterations = 5);
	virtual long getUT() { return ut; }
	virtual long getUP() { return up; }
	virtual long getT() { return t; }
	virtual long getP() { return p; }
	
	virtual ~BMP180();

};

} /* namespace exploringBB */

#endif /* BMP180_H_ */

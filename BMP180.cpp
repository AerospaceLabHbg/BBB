#include "BMP180.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

using namespace std;

namespace exploringBB {


#define	DEVID		0x00   //Device ID
#define DATA_FORMAT	0x31   //Data format control
#define OUTREG		0xF4   //Output control register

#define AC11		0xAA
#define AC12		0xAB


long BMP180::combineRegisters16(unsigned char msb, unsigned char lsb){
   //shift the MSB left by 8 bits and OR with LSB
   return ((short)msb<<8)|(short)lsb;
}

long BMP180::combineRegisters24(unsigned char msb, unsigned char lsb, unsigned char xlsb){
   //shift the MSB left by 16 bits and shifts the LSB left by 8 bits and OR with XLSB
   return ((short)msb<<16)|(short)lsb<<8|(short)xlsb;
}

int BMP180::updateRegisters(){

   return 0;
}
/*
 * The constructor for the BMP180 object. It passes the bus number and the
 * device address (with is 0x53 by default) to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the ADXL345 device is on - typically 0 or 1
 * @param I2CAddress The address of the ADLX345 device (default 0x53, but can be altered)
 */
BMP180::BMP180(unsigned int I2CBus, unsigned int I2CAddress):
	I2CDevice(I2CBus, I2CAddress){   // this member initialisation list calls the parent constructor
	this->I2CAddress = I2CAddress;
	this->I2CBus = I2CBus;
	this->registers = NULL;
	this->writeRegister(OUTREG, 0x2E);
}

/*
 * Read the sensor state. This method checks that the device is being correctly
 * read by using the device ID of the ADXL345 sensor. It will read in the accelerometer registers
 * and pass them to the combineRegisters() method to be processed.
 * @return 0 if the registers are successfully read and -1 if the device ID is incorrect.
 */
	int BMP180::readSensorState(){
	this->registers = this->readRegisters(BUFFER_SIZE, 0x00);
	if(*this->registers!=0x55){
		perror("BMP180: Failure Condition - Sensor ID not Verified");
		return -1;
	}
	this->ac1 = this->combineRegisters16(*(registers+AC11), *(registers+AC12));
	return 0;
}
	
BMP180::~BMP180() {}

} /* namespace exploringBB */

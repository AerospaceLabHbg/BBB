#include "BMP180.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <iomanip>

using namespace std;

namespace exploringBB {


#define	DEVID		0xD0   //Device ID
#define SOFTRESET	0xE0   //Output control register
	
//Calibration registers 16bit MSB first
#define AC11		0xAA
#define AC12		0xAB
#define AC21		0xAC
#define AC22		0xAD
#define AC31		0xAE
#define AC32		0xAF
#define AC41		0xB0
#define AC42		0xB1
#define AC51		0xB2
#define AC52		0xB3
#define AC61		0xB4
#define AC62		0xB5
#define B11		0xB6
#define B12		0xB7
#define B21		0xB8
#define B22		0xB9
#define MB1		0xBA
#define MB2		0xBB
#define MC1		0xBC
#define MC2		0xBD
#define MD1		0xBE
#define MD2		0xBF
//Output Manipulation
#define CONREG		0xF4
#define OUTTemp		0x2E
#define OUTPres		0x34
#define OSS		0

#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)
	
long BMP180::combineRegisters16(unsigned char msb, unsigned char lsb){
   //shift the MSB left by 8 bits and OR with LSB
   return ((short)msb<<8)|(short)lsb;
}

long BMP180::combineRegisters24(unsigned char msb, unsigned char lsb, unsigned char xlsb){
   //shift the MSB left by 16 bits and shifts the LSB left by 8 bits and OR with XLSB
   return ((short)msb<<16)|(short)lsb<<8|(short)xlsb;
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
	this->writeRegister(SOFTRESET, 0xB6);
}

/*
 * Read the sensor state. This method checks that the device is being correctly
 * read by using the device ID of the ADXL345 sensor. It will read in the accelerometer registers
 * and pass them to the combineRegisters() method to be processed.
 * @return 0 if the registers are successfully read and -1 if the device ID is incorrect.
 */
int BMP180::readSensorState(){
	this->registers = this->readRegisters(BUFFER_SIZE, 0x00);
	if(readRegister(DEVID)!=0x55){
		perror("BMP180: Failure Condition - Sensor ID not Verified");
		return -1;
	}
	this->ac1 = this->combineRegisters16(*(registers+AC11), *(registers+AC12));
	this->ac2 = this->combineRegisters16(*(registers+AC21), *(registers+AC22));
	this->ac3 = this->combineRegisters16(*(registers+AC31), *(registers+AC32));
	this->ac4 = this->combineRegisters16(*(registers+AC41), *(registers+AC42));
	this->ac5 = this->combineRegisters16(*(registers+AC51), *(registers+AC52));
	this->ac6 = this->combineRegisters16(*(registers+AC61), *(registers+AC62));
	return 0;
}

void BMP180::displayCalibrationData(){
	cout << "AC1:"<< ac1 << "AC2:" << (this->getAC2()) << endl;

}
	
BMP180::~BMP180() {}

} /* namespace exploringBB */

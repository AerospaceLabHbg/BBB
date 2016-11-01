/*
 * ADXL345.cpp  Created on: 17 May 2014
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

#include "BMP180.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

using namespace std;

namespace exploringBB {

//From Table 19. of the ADXL345 Data sheet
// The BMP180 Registers required for this example
#define DEVID  0xD0
// Calibration Registers
#define AC11   0xAA
#define AC12   0xAB
#define AC21   0xAC
#define AC22   0xAD
#define AC31   0xAE
#define AC32   0xAF
#define AC41   0xB0
#define AC42   0xB1
#define AC51   0xB2
#define AC52   0xB3
#define AC61   0xB4
#define AC62   0xB5
#define B11    0xB6
#define B12    0xB7
#define B21    0xB8
#define B22    0xB9
#define MB1    0xBA
#define MB2    0xBB
#define MC1    0xBC
#define MC2    0xBD
#define MD1    0xBE
#define MD2    0xBF
// Data Registers
#define WT2     0x2E
#define WT1     0xF4
#define WT3     0x34
#define MSB     0xF6
#define LSB     0xF7
#define XLSB    0xF8

//Variables
#define P0   101325
#define WAIT 4500
#define OSS     0
#define OSSWAIT 4500

/**
 * Method to combine two 8-bit registers into a single short, which is 16-bits on the BBB. It shifts
 * the MSB 8-bits to the left and then ORs the result with the LSB.
 * @param msb an unsigned character that contains the most significant byte
 * @param lsb an unsigned character that contains the least significant byte
 */
long BMP180::combineRegisters16(unsigned char msb, unsigned char lsb){
   //shift the MSB left by 8 bits and OR with LSB
   return ((short)msb<<8)|(short)lsb;
}

long BMP180::combineRegisters24(unsigned char msb, unsigned char lsb){
   //shift the MSB left by 8 bits and OR with LSB
   return ((short)msb<<16)|(short)lsb<<8|(short)xlsb;
}

/**
 * Method to calculate the pitch and roll state values. This calculation takes account of the scaling
 * factors due to the resolution and gravity range to determine gravity weighted values that are used
 * to calculate the angular pitch and roll values in degrees.

void ADXL345::calculatePitchAndRoll(){
	float gravity_range;
	switch(ADXL345::range){
		case ADXL345::PLUSMINUS_16_G: gravity_range=32.0f; break;
		case ADXL345::PLUSMINUS_8_G: gravity_range=16.0f; break;
		case ADXL345::PLUSMINUS_4_G: gravity_range=8.0f; break;
		default: gravity_range=4.0f; break;
	}
    float resolution = 1024.0f;
    if (this->resolution==ADXL345::HIGH) resolution = 8192.0f; //13-bit resolution
    float factor = gravity_range/resolution;

    float accXg = this->accelerationX * factor;
    float accYg = this->accelerationY * factor;
    float accZg = this->accelerationZ * factor;
	float accXSquared = accXg * accXg ;
	float accYSquared = accYg * accYg ;
	float accZSquared = accZg * accZg ;
	this->pitch = 180 * atan(accXg/sqrt(accYSquared + accZSquared))/M_PI;
	this->roll = 180 * atan(accYg/sqrt(accXSquared + accZSquared))/M_PI;
}
 */
/**
 * Method used to update the DATA_FORMAT register and any other registers that might be added
 * in the future.
 * @return 0 if the register is updated successfully

int BMP180::updateRegisters(){
   //update the DATA_FORMAT register
   char data_format = 0x00;  //+/- 2g with normal resolution
   //Full_resolution is the 3rd LSB
   data_format = data_format|((this->resolution)<<3);
   data_format = data_format|this->range; // 1st and 2nd LSB therefore no shift
   return this->writeRegister(DATA_FORMAT, data_format);
}
 */
/**
 * The constructor for the ADXL345 accelerometer object. It passes the bus number and the
 * device address (with is 0x53 by default) to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the ADXL345 device is on - typically 0 or 1
 * @param I2CAddress The address of the ADLX345 device (default 0x53, but can be altered)
 */
BMP180::BMP180(unsigned int I2CBus, unsigned int I2CAddress):
	I2CDevice(I2CBus, I2CAddress){   // this member initialisation list calls the parent constructor
	this->I2CAddress = I2CAddress;
	this->I2CBus = I2CBus;
	this->ut = 0;
	this->up = 0;
	this->registers = NULL;
	//this->updateRegisters();
}

/**
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
	this->ac2 = this->combineRegisters16(*(registers+AC21), *(registers+AC22));
	this->ac3 = this->combineRegisters16(*(registers+AC31), *(registers+AC32));
	this->ac4 = this->combineRegisters16(*(registers+AC41), *(registers+AC42));
	this->ac5 = this->combineRegisters16(*(registers+AC51), *(registers+AC52));
	this->ac6 = this->combineRegisters16(*(registers+AC61), *(registers+AC62));
	this->b1 = this->combineRegisters16(*(registers+B11), *(registers+B12));
	this->b2 = this->combineRegisters16(*(registers+B21), *(registers+B22));
	this->mb = this->combineRegisters16(*(registers+MB1), *(registers+MB2));
	this->mc = this->combineRegisters16(*(registers+MC1), *(registers+MC2));
	this->md = this->combineRegisters16(*(registers+MD1), *(registers+MD2));
	
	this->writeRegister(WT1, WT2)
	usleep(WAIT);
	this->registers = this->readRegisters(BUFFER_SIZE, 0x00);
	this->ut = this->combineRegisters16(*(registers+MSB), *(registers+LSB));
	
	this->writeRegister(WT1, (WT3+(OSS<<6)))
	usleep(OSSWAIT);
	this->registers = this->readRegisters(BUFFER_SIZE, 0x00);
	this->up = this->combineRegisters24(*(registers+MSB), *(registers+LSB),*(registers+XLSB));
	return 0;
}


/**
 * Useful debug method to display the pitch and roll values in degrees on a single standard output line
 * @param iterations The number of 0.1s iterations to take place.
 */
void BMP180::display(int iterations){
	int count = 0;
	cout << "AC1:"<< this->ac1 << endl;
	cout << "AC2:"<< this->ac2 << endl;
	while(count < iterations){
	      usleep(100000);
	      this->readSensorState();
	cout << "ut:"<< this->ut << endl;
	cout << "up:"<< this->up << endl;
	      count++;
	}
}

BMP180::~BMP180() {}

} /* namespace exploringBB */

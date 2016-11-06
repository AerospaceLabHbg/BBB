
#include "DS1307.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <iomanip>

using namespace std;

namespace exploringBB {


#define	DEVIDREGDS1307	0xD1   //Device ID Register
#define DEVIDDS1307	    0x97   //Device ID
#define OSCI_START	    0x00   //Output control register
#define OSCI_STOP	      0x01   //Output control register
	
//Calibration registers 16bit MSB first
#define SECONDS		0x00
#define MINUTES		0x01
#define HOURS		  0x02
#define DAY		    0x03
#define DATE	    0x04
#define MONTH		  0x05
#define YEAR		  0x06

short DS1307::getData(unsigned char msb){
   return msb;
}
/*
 * The constructor for the BMP180 object. It passes the bus number and the
 * device address (with is 0x53 by default) to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the ADXL345 device is on - typically 0 or 1
 * @param I2CAddress The address of the ADLX345 device (default 0x53, but can be altered)
 */
DS1307::DS1307(unsigned int I2CBus, unsigned int I2CAddress):
	I2CDevice(I2CBus, I2CAddress){   // this member initialisation list calls the parent constructor
	this->I2CAddress = I2CAddress;
	this->I2CBus = I2CBus;
	this->registers = NULL;
	this->writeRegister(OSCI_START, 0x00);
}

/*
 * Read the sensor state. This method checks that the device is being correctly
 * read by using the device ID of the ADXL345 sensor. It will read in the accelerometer registers
 * and pass them to the combineRegisters() method to be processed.
 * @return 0 if the registers are successfully read and -1 if the device ID is incorrect.
 */
int DS1307::readSensorState(){
	this->registers = this->readRegisters(BUFFER_SIZE, 0x00);
	if(readRegister(DEVIDREGDS1307	)!=DEVIDDS1307){
		perror("BMP180: Failure Condition - Sensor ID not Verified");
		return -1;
	}
	this->seconds = this->getData(*(registers+SECONDS));
	this->minutes = this->getData(*(registers+MINUTES));
	this->hours = this->getData(*(registers+HOURS));
	this->day = this->getData(*(registers+DAY));
	this->date = this->getData(*(registers+DATE));
	this->month = this->getData(*(registers+MONTH));
	this->year = this->getData(*(registers+YEAR));
	return 0;
}

void DS1307::updateRTC(){
	/*struct tm {
   int tm_sec;          seconds,  range 0 to 59          
   int tm_min;          minutes, range 0 to 59           
   int tm_hour;         hours, range 0 to 23             
   int tm_mday;         day of the month, range 1 to 31  
   int tm_mon;          month, range 0 to 11             
   int tm_year;         The number of years since 1900   
   int tm_wday;        day of the week, range 0 to 6    
   int tm_yday;         day in the year, range 0 to 365  
   int tm_isdst;        daylight saving time             
}; */
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int tmhour, tmmon, tmyear, tmwday;
	tmhour = tm.tm_hour+1;
	tmmon = tm.tm_mon+1;
	tmyear = tm.tm_year+1900;
	cout << dec <<endl;
	tmwday = tm.tm_wday+1;
	cout << "BBB Time->  "<< "seconds: "<< tm.tm_sec <<"   minutes: "<< tm.tm_min <<"   hours: "<< tmhour <<endl;
	cout << "wday: "<< tmwday <<"   date: "<< tmday <<"   month: "<< tmmon<<"   year: "<< tmyear <<endl;


}
	
void DS1307::displayTime(){
	cout << hex <<endl;
	cout << "seconds: "<< seconds <<"   minutes: "<< minutes <<"   hours: "<< hours <<endl;


}

	
void DS1307::readData(){
	this->registers = this->readRegisters(BUFFER_SIZE, 0x00);
	this->seconds = this->getData(*(registers+SECONDS));
	this->minutes = this->getData(*(registers+MINUTES));
	this->hours = this->getData(*(registers+HOURS));
	this->day = this->getData(*(registers+DAY));
	this->date = this->getData(*(registers+DATE));
	this->month = this->getData(*(registers+MONTH));
	this->year = this->getData(*(registers+YEAR));
}
	


void DS1307::displayDate(){
	
}
	
DS1307::~DS1307() {}

} /* namespace exploringBB */

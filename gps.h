/*
 * I2C.h  Created on: 17 May 2014
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

#ifndef GPS_H_
#define GPS_H_

 //This sets up variables for useful commands.

                //This set is used to set the rate the GPS reports
                #define UPDATE_10_sec  "$PMTK220,10000*2F\r\n" //Update Every 10 Seconds
                #define UPDATE_5_sec  "$PMTK220,5000*1B\r\n"   //Update Every 5 Seconds  
                #define UPDATE_1_sec  "$PMTK220,1000*1F\r\n"   //Update Every One Second
                #define UPDATE_200_msec  "$PMTK220,200*2C\r\n" //Update Every 200 Milliseconds
                //This set is used to set the rate the GPS takes measurements
                #define MEAS_10_sec  "$PMTK300,10000,0,0,0,0*2C\r\n" //Measure every 10 seconds
                #define MEAS_5_sec  "$PMTK300,5000,0,0,0,0*18\r\n"   //Measure every 5 seconds
                #define MEAS_1_sec  "$PMTK300,1000,0,0,0,0*1C\r\n"   //Measure once a second
                #define MEAS_200_msec "$PMTK300,200,0,0,0,0*2F\r\n"  //Meaure 5 times a second
                //Set the Baud Rate of GPS
                #define BAUD_57600 "$PMTK251,57600*2C\r\n"          //Set Baud Rate at 57600
               #define BAUD_9600 "$PMTK251,9600*17\r\n"             //Set 9600 Baud Rate
                //Commands for which NMEA Sentences are sent
                #define GPRMC_ONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n" //Send only the GPRMC Sentence
                #define GPRMC_GPGGA"$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"//Send GPRMC AND GPGGA Sentences
                #define SEND_ALL "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n" //Send All Sentences
                #define SEND_NOTHING"$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n" //Send Nothing

/* baudrate settings are defined in <asm/termbits.h>, which is
   included by <termios.h> */
#define BAUDRATE B9600   // Change as needed, keep B
#define BAUDRATEH B9600   // Change as needed, keep B

/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyO4" //Beaglebone Black serial port

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

namespace GPS {

class GPSDevice{
private:
	int fd, c, res;
   	struct termios oldtio, newtio;
    	char buf[255];
public:
	GPSDevice();
	virtual int open();
	virtual int write(unsigned char value);
	virtual unsigned char readRegister(unsigned int registerAddress);
	virtual unsigned char* readRegisters(unsigned int number, unsigned int fromAddress=0);
	virtual int writeRegister(unsigned int registerAddress, unsigned char value);
	virtual void debugDumpRegisters(unsigned int number = 0xff);
	virtual void close();
	virtual ~GPSDevice();
};
	
               


} /* namespace exploringBB */

#endif /* I2C_H_ */

/*
 * EBBGPIO.cpp  Created on: 29 Apr 2014
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

#include <iostream>
#include "BMP180.h"
#include "DS1307.h"
#include "I2CDevice.h"
#include <unistd.h>
#include <pthread.h>
#include "GPS.h"


using namespace std;
using namespace exploringBB;

int main() {

	BMP180 BMP180(1,0x77);
	DS1307 DS1307(1,0x68);
	DS1307.readSensorState();
	DS1307.updateRTC();
	GPSDevice gps;
	BMP180.readSensorState();
	BMP180.displayCalibrationData();
	
	int i = 0;
		while(i<60){
			cout <<dec<<"i="<< i <<endl;
	BMP180.readTemperature();
	BMP180.readPressure();
	DS1307.readData();
	DS1307.displayTime();
	BMP180.displayResults();
	gps.readnmea();
	usleep(1000000);
	i++;	}
	
	gps.closeGPS();
	return 0;
}









#include <iostream>
#include "GPS.h"
#include <unistd.h>
#include <pthread.h>

using namespace std;


int main() {

	GPSDevice gps;
	int i = 0;
		while(i<20){

	cout <<dec<<"i="<< i <<endl;
	gps.readnmea();
	
	i++;	}
	gps.closeGPS();
	return 0;
}

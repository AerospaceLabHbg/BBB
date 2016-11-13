

#include <iostream>
#include "GPS.h"
#include <unistd.h>
#include <pthread.h>

using namespace std;
using namespace GPS;

int main() {

	GPSDevice gps;
	int i = 0;
		while(i<4){

	cout <<dec<<"i="<< i <<endl;
	gps.readnmea();
	
	i++;	}
	gps.~GPSDevice()
	return 0;
}

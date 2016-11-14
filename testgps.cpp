

#include <iostream>
#include "GPS.h"
#include <unistd.h>
#include <pthread.h>

using namespace std;
using namespace exploringBB;


int main() {

	GPSDevice gps;
	int i = 0;
	
	while(i<20){
	gps.readnmea();	
	i++;	
	}
	
	gps.closeGPS();
	return 0;
}

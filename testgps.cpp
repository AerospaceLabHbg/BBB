

#include <iostream>
#include "GPS.h"
#include <unistd.h>
#include <pthread.h>

using namespace std;
using namespace GPS;

int main() {

	GPSDevice();
	int i = 0;
		while(i<5){
gps.readnmea();
	cout <<dec<<"i="<< i <<endl;
	i++;	}
	return 0;
}

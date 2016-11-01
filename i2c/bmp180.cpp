/** Sample I2C ADXL345 Code that outputs the x,y and z accelerometer values
*   on a single line for sixty seconds.
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<iomanip>
#include<math.h>
#include"i2cdevice.h"
using namespace std;
using namespace exploringBB;

// Small macro to display value in hexadecimal with 2 places
#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)

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
#define WT1     0x2E
#define WT2     0xF4
#define WT3     0x34
#define OSS     2
#define MSB     0xF6
#define LSB     0xF7
#define XLSB    0xF8
//Buffer
#define BUFFER_SIZE 256
//Variables
#define P0   101325
#define WAIT 13500
unsigned char dataBuffer[BUFFER_SIZE];

// short is 16-bits in size on the BBB
long combineValues(unsigned char msb, unsigned char lsb){
   //shift the msb right by 8 bits and OR with lsb
   return ((long)msb<<8)|(long)lsb;
}

// long is 32-bits in size on the BBB
long combineValueslong(unsigned char msb, unsigned char lsb, unsigned char xlsb){
   return ((long)msb<<16)|((long)lsb<<8)|((long)xlsb);
}

int I2CDevice::readSensorState(){
	this->file = this->readRegisters(BUFFER_SIZE, 0x00);
	if(*this->file!=0x55){
		perror("ADXL345: Failure Condition - Sensor ID not Verified");
		return -1;
	}
	return 0;
}

int main(){
   int file;
   cout << "Starting the BMP180 sensor application" << endl;
	
	I2CDevice sensor(1,0x77);
	sensor.open();
	
   if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      cout << "failed to open the bus" << endl;
      return 1;
   }
   if(ioctl(file, I2C_SLAVE, 0x77) < 0){
      cout << "Failed to connect to the sensor" << endl;
      return 1;
   }
  // writeRegister(file, POWER_CTL, 0x08);
   //Setting mode to 00000000=0x00 for +/-2g 10-bit
   //Setting mode to 00001011=0x0B for +/-16g 13-bit
  // writeRegister(file, DATA_FORMAT, 0x00);
   sensor.readRegisters(BUFFER_SIZE,0x00);
   cout << "The Device ID is: " << HEX(dataBuffer[DEVID]) << endl;
   //cout << "The POWER_CTL mode is: " << HEX(dataBuffer[POWER_CTL]) << endl;
   //cout << "The DATA_FORMAT is: " << HEX(dataBuffer[DATA_FORMAT]) << endl;
   //cout << dec << endl;   //reset back to decimal
     
      short ac1 = combineValues((dataBuffer[AC11]),(dataBuffer[AC12]));
      short ac2 = combineValues((dataBuffer[AC21]),(dataBuffer[AC22]));
      short ac3 = combineValues((dataBuffer[AC31]),(dataBuffer[AC32]));
     unsigned short ac4 = combineValues((dataBuffer[AC41]),(dataBuffer[AC42]));
     unsigned short ac5 = combineValues((dataBuffer[AC51]),(dataBuffer[AC52]));
     unsigned short ac6 = combineValues((dataBuffer[AC61]),(dataBuffer[AC62]));
      short b1 = combineValues((dataBuffer[B11]),(dataBuffer[B12]));
      short b2 = combineValues((dataBuffer[B21]),(dataBuffer[B22]));
      short mb = combineValues((dataBuffer[MB1]),(dataBuffer[MB2]));
      short mc = combineValues((dataBuffer[MC1]),(dataBuffer[MC2]));
      short md = combineValues((dataBuffer[MD1]),(dataBuffer[MD2]));
/*
      cout << "ac1="<<setbase(10)<<ac1<< endl  ;
      cout << "ac2="<<setbase(10)<<ac2<< endl  ;
      cout << "ac3="<<setbase(10)<<ac3<< endl  ;
      cout << "ac4="<<setbase(10)<<ac4<< endl  ;
      cout << "ac5="<<setbase(10)<<ac5<< endl  ;
      cout << "ac6="<<setbase(10)<<ac6<< endl  ;
      cout << "b1="<<setbase(10)<<b1<< endl  ;
      cout << "b2="<<setbase(10)<<b2<< endl  ;
      cout << "mb="<<setbase(10)<<mb<< endl  ;
      cout << "mc="<<setbase(10)<<mc<< endl  ;
      cout << "md="<<setbase(10)<<md<< endl  ;
*/
int count=0;
while(count <10){
 sensor.writeRegister(WT2,WT1);
 usleep (WAIT);
   sensor.readRegisters(BUFFER_SIZE,0x00);
    long ut = combineValues((dataBuffer[MSB]),(dataBuffer[LSB]));

 sensor.writeRegister(WT2,(WT3+(OSS<<6)));
 usleep (WAIT);
   sensor.readRegisters(BUFFER_SIZE,0x00);
    long up =  (combineValueslong(dataBuffer[MSB],dataBuffer[LSB],dataBuffer[XLSB]))>>(8-OSS);

cout << dec << endl;
/*
      cout << "UT="<<ut<< endl  ;
      cout << "UP="<<up<< endl  ;
*/
// Berechnung T
long x1; 
long x2;
long b5;
long t;
x1=(ut-ac6)*ac5/(pow(2,15));
x2=mc*(pow(2,11))/(x1+md);
b5=x1+x2;
t=(b5+8)/16;
	cout << "T="<<t<< endl  ;
//Berechnung p
long b6;
long x3;
long b3;
unsigned long b4;
unsigned long b7;
long p;

b6 = b5-4000;
x1 = (b2*(b6*b6/(pow(2,12))))/(pow(2,11));
x2 = ac2*b6/(pow(2,11));
x3 = x1+x2;
b3 = (((ac1*4+x3)<<OSS)+2)/4;
x1 = ac3*b6/(pow(2,13));
x2 = (b1*(b6*b6/(pow(2,12))))/(pow(2,16));
x3 = ((x1+x2)+2)/4;
b4 = ac4*(unsigned long)(x3+32768)/(pow(2,15));
b7 = ((unsigned long)up-b3)*(50000>>OSS);
if (b7<0x80000000)
	{
	p = (b7*2)/b4;
	}
	else
	{
	p = (b7/b4)*2;
}

x1 = (p/256)*(p/256);
x1 = (x1*3038)/(pow(2,16));
x2 = (-7357*p)/(pow(2,16));
p = p+(x1+x2+3791)/16;

	cout << "p=" <<p<<endl;
//Hoehe
float h;
h = 44330*(1-(pow(((float) p/P0),(1/5.255))));

cout << "h=" <<h<<endl;
count++;  
}

   // usleep(1000000);
     // readRegisters(file);  //read the sensor again
     // count++;
   close(file);
   return 0;
}

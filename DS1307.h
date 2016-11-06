
#ifndef DS1307_H_
#define DS1307_H_
#include"I2CDevice.h"

namespace exploringBB {

class DS1307:protected I2CDevice{

private:
	unsigned int I2CBus, I2CAddress; //initialiserung für BMP180(I2CBus,I2CAddress)
	unsigned char *registers;
	short ac1, ac2, ac3, b1, b2, mb, mc, md;
	unsigned short ac4, ac5, ac6;
	long x1, x2, x3, b3, b5, b6;
	unsigned long b4, b7;
	long combineRegisters16(unsigned char msb, unsigned char lsb);
	long combineRegisters24(unsigned char msb, unsigned char lsb, unsigned char xlsb);
	long ut,up,pressure,temperature;


public:
	DS1307(unsigned int I2CBus, unsigned int I2CAddress=0x53);
	virtual void displayResults();
	virtual void readTemperature();
	virtual void readPressure();
	virtual int readSensorState();
	virtual void displayCalibrationData();
	virtual ~DS1307();
};

} /* namespace exploringBB */

#endif /* BMP180_H_ */

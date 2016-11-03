
#ifndef BMP180_H_
#define BMP180_H_
#include"I2CDevice.h"

/// An i2c Device has 0x100 registers
#define BUFFER_SIZE 0x100

namespace exploringBB {

class BMP180:protected I2CDevice{

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
	void readTemperature();
	void readPressure();

public:
	BMP180(unsigned int I2CBus, unsigned int I2CAddress=0x53);
	virtual void displayResults();
	virtual int readSensorState();
	virtual void displayCalibrationData();
	virtual ~BMP180();
};

} /* namespace exploringBB */

#endif /* BMP180_H_ */


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
	short ac1, ac2 ,ac3;
	long combineRegisters16(unsigned char msb, unsigned char lsb);
	long combineRegisters24(unsigned char msb, unsigned char lsb, unsigned char xlsb);
	long ut,up,pressure,temperature;
	virtual int updateRegisters();

public:
	BMP180(unsigned int I2CBus, unsigned int I2CAddress=0x53);
	virtual int readSensorState();
	virtual short getUT() { return ut; }
	virtual short getUP() { return up; }
	virtual short getTemperature() { return temperature; }
	virtual float getPressure() { return pressure; }
	virtual ~BMP180();
};

} /* namespace exploringBB */

#endif /* BMP180_H_ */


#ifndef DS1307_H_
#define DS1307_H_
#include"I2CDevice.h"

namespace exploringBB {

class DS1307:protected I2CDevice{

private:
	unsigned int I2CBus, I2CAddress; //initialiserung für BMP180(I2CBus,I2CAddress)
	unsigned char *registers;
	short seconds, minutes, hours, day, date, month, year;
	int getData(unsigned char msb);


public:
	DS1307(unsigned int I2CBus, unsigned int I2CAddress=0x68);
	virtual void readData();
	virtual int readSensorState();
	virtual void displayDate();
	virtual void displayTime();
	virtual ~DS1307();
};

} /* namespace exploringBB */

#endif /* DS1307_H_ */

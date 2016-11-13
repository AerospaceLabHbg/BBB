

#include"GPS.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>




using namespace std;



GPSDevice::GPSDevice() {
                    

    /* Open modem device for reading and writing and not as controlling tty
       because we don't want to get killed if linenoise sends CTRL-C. */
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
    if (fd < 0) { perror(MODEMDEVICE); exit(-1); }

    bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

    /* BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
       CRTSCTS : output hardware flow control (only used if the cable has
                 all necessary lines. See sect. 7 of Serial-HOWTO)
       CS8     : 8n1 (8bit,no parity,1 stopbit)
       CLOCAL  : local connection, no modem contol
       CREAD   : enable receiving characters */
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

    /* IGNPAR  : ignore bytes with parity errors
       otherwise make device raw (no other input processing) */
    newtio.c_iflag = IGNPAR;

    /*  Raw output  */
    newtio.c_oflag = 0;

    /* ICANON  : enable canonical input
       disable all echo functionality, and don't send signals to calling program */
    newtio.c_lflag = ICANON;
    /* now clean the modem line and activate the settings for the port */
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
    // NMEA command to ouput all sentences
    // Note that this code & format values in manual are hexadecimal
	//------- Setting Baudrate to 57600
	write(fd, BAUD_57600, sizeof(BAUD_57600));
        usleep(1000);
	close(fd);
	  fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
    if (fd < 0) { perror(MODEMDEVICE); exit(-1); }

    bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

    /* BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
       CRTSCTS : output hardware flow control (only used if the cable has
                 all necessary lines. See sect. 7 of Serial-HOWTO)
       CS8     : 8n1 (8bit,no parity,1 stopbit)
       CLOCAL  : local connection, no modem contol
       CREAD   : enable receiving characters */
    newtio.c_cflag = BAUDRATEH | CRTSCTS | CS8 | CLOCAL | CREAD;

    /* IGNPAR  : ignore bytes with parity errors
       otherwise make device raw (no other input processing) */
    newtio.c_iflag = IGNPAR;

    /*  Raw output  */
    newtio.c_oflag = 0;

    /* ICANON  : enable canonical input
       disable all echo functionality, and don't send signals to calling program */
    newtio.c_lflag = ICANON;
    /* now clean the modem line and activate the settings for the port */
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
	//---------
	write(fd, UPDATE_200_msec, sizeof(UPDATE_200_msec));
        usleep(1000);
	write(fd, MEAS_200_msec,sizeof(MEAS_200_msec));
        usleep(1000);
	write(fd, GPRMC_ONLY,sizeof(GPRMC_GPGGA  ));
        usleep(1000);

printf("GPS is Initialized");
}
int GPSDevice::readnmea(){
	
        res = read(fd, buf, 255);
	buf[res] = 0;             // set end of string, so we can printf 
	string str(buf);	//copy char to string for splitting
	//cout << str <<endl;
/*eg4. $GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
1    = UTC of position fix
2    = Data status (V=navigation receiver warning)
3    = Latitude of fix
4    = N or S
5    = Longitude of fix
6    = E or W
7    = Speed over ground in knots
8    = Track made good in degrees True
9    = UT date
10   = Magnetic variation degrees (Easterly var. subtracts from true course)
11   = E or W
12   = Checksum*/
string nmea[13];
int j=0;
	
istringstream ss(str);	//splitting string with comma
string token;

while(getline(ss, token, ',')) {
    
	nmea[j]=token;
	j++;
}
	if (nmea[2]="A"){
cout <<"UTC(hhmmss.ss): "<< nmea[1] << '\n';
cout <<"Latitude: "<< nmea[3] << nmea[4] << '\n';
cout <<"Longitude: "<< nmea[5] << nmea[6] << '\n';
	}
}
	



int GPSDevice::closeGPS(){
	close(fd);
}
	
GPSDevice::~GPSDevice() {
	  tcsetattr(fd, TCSANOW, &oldtio);

}



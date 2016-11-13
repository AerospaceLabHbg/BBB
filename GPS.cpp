

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

#include "nmea.h"
#include "nmea/gpgll.h"
#include "nmea/gpgga.h"

using namespace std;


namespace GPS {


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
	write(fd, GPRMC_GPGGA  ,sizeof(GPRMC_GPGGA  ));
        usleep(1000);

printf("GPS is Initialized");
}


int GPSDevice::readnmea(){
        res = read(fd, buf, 255);
	 buf[res] = 0;             // set end of string, so we can printf 
	string str(buf);
	//printf("%s", buf, res);
	cout << str <<endl;

	printf("Parsing NMEA sentence: %s", buf);

	// Pointer to struct containing the parsed data. Should be freed manually.
	nmea_s *data;

	// Parse...
	data = nmea_parse(&buf, strlen(buf), 0);
	if (NMEA_GPGGA == data->type) {
    nmea_gpgga_s *gpgga = (nmea_gpgga_s *) data;

    printf("GPGGA Sentence\n");
    printf("Number of satellites: %d\n", gpgga->n_satellites);
    printf("Altitude: %d %c\n", gpgga->altitude, gpgga->altitude_unit);
	}

	if (NMEA_GPGLL == data->type) {
    nmea_gpgll_s *gpgll = (nmea_gpgll_s *) data;

    printf("GPGLL Sentence\n");
    printf("Longitude:\n");
    printf("  Degrees: %d\n", gpgll->longitude.degrees);
    printf("  Minutes: %f\n", gpgll->longitude.minutes);
    printf("  Cardinal: %c\n", (char) gpgll->longitude.cardinal);
    printf("Latitude:\n");
    printf("  Degrees: %d\n", gpgll->latitude.degrees);
    printf("  Minutes: %f\n", gpgll->latitude.minutes);
    printf("  Cardinal: %c\n", (char) gpgll->latitude.cardinal);
	}
	

	nmea_free(data);
	free(buf);
	
}
int GPSDevice::closeGPS(){
	close(fd);
}
	
GPSDevice::~GPSDevice() {
	  tcsetattr(fd, TCSANOW, &oldtio);

}

} /* namespace GPS */

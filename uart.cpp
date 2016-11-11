#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
//NMEA Parser Lib
#include <include/nmealib/info.h>
#include <include/nmealib/nmath.h>
#include <include/nmealib/parser.h>


/* baudrate settings are defined in <asm/termbits.h>, which is
   included by <termios.h> */
#define BAUDRATE B9600   // Change as needed, keep B

/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyO4" //Beaglebone Black serial port

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

main()
{
    int fd, c, res;
    struct termios oldtio, newtio;
    char buf[255];
    // Load the pin configuration
    int ret = system("echo uart4 > /sys/devices/bone_capemgr.9/slots");
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
    write(fd, "$PTNLSNM,273F,01*27\r\n", 21);
    /* terminal settings done, now handle input*/
    while (TRUE) {     /* loop continuously */
        /*  read blocks program execution until a line terminating character is
            input, even if more than 255 chars are input. If the number
            of characters read is smaller than the number of chars available,
            subsequent reads will return the remaining chars. res will be set
            to the actual number of characters actually read */
        res = read(fd, buf, 255);
        buf[res] = 0;             /* set end of string, so we can printf */
        printf("%s", buf, res);
       
         const char *buff[] = {
      "$GPRMC,173843,A,3349.896,N,11808.521,W,000.0,360.0,230108,013.4,E*69\r\n",
      "$GPGGA,111609.14,5001.27,N,3613.06,E,3,08,0.0,10.2,M,0.0,M,0.0,0000*70\r\n",
      "$GPGSV,2,1,08,01,05,005,80,02,05,050,80,03,05,095,80,04,05,140,80*7f\r\n",
      "$GPGSV,2,2,08,05,05,185,80,06,05,230,80,07,05,275,80,08,05,320,80*71\r\n",
      "$GPGSA,A,3,01,02,03,04,05,06,07,08,00,00,00,00,0.0,0.0,0.0*3a\r\n",
      "$GPRMC,111609.14,A,5001.27,N,3613.06,E,11.2,0.0,261206,0.0,E*50\r\n",
      "$GPVTG,217.5,T,208.8,M,000.00,N,000.01,K*4C\r\n" };

  int it;
  NmeaInfo info;
  NmeaParser parser;
  NmeaPosition dpos;

  nmeaInfoClear(&info);
  nmeaParserInit(&parser, 0);

  for (it = 0; it < 7; it++) {
    nmeaParserParse(&parser, buff[it], strlen(buff[it]), &info);

    nmeaMathInfoToPosition(&info, &dpos);
    printf("%03d, Lat: %f, Lon: %f, Sig: %d, Fix: %d\n", it, dpos.lat, dpos.lon, info.sig, info.fix);
  }

  nmeaParserDestroy(&parser);

return 0;
    }
    tcsetattr(fd, TCSANOW, &oldtio);
}

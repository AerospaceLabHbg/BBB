#include <iostream>
//#include <cmath>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <termios.h>    /*Enables us to set baud rate for RX/TX separately*/
#include <fcntl.h>      /*Enables use of flags to modify open(), read(), write() functions*/
#include <unistd.h>     /*Enables use of open(), read(), write()*/

using namespace std;

int main(){
   int file, count=80;

   if ((file = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return -1;
   }
   struct termios options;
   tcgetattr(file, &options);
   options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);
   
   unsigned char receive[1000];
   
   int bytes_read;
   char byte_in[5];
   int ARRAY_SIZE = 5;
   int i;
   bytes_read = read(file, byte_in, ARRAY_SIZE); /* Reads ttyO port, stores data into byte_in. */

    for (i = ARRAY_SIZE; i < ARRAY_SIZE; i++){
        cout << bytes_read[i] << endl;
    }
   
   count = read(file, (void*)receive, 10);
      printf("The following was read in [%d]: %s\n",count,receive);
 
   close(file);
   
 
     
   return 0;
}

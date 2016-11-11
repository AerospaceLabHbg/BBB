#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

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
   char bytes_in[5];
   byte ARRAY_SIZE = 5;
   bytes_read = read(file, byte_in, ARRAY_SIZE); /* Reads ttyO port, stores data into byte_in. */

    for (byte i = ARRAY_SIZE; i < ARRAY_SIZE; i++){
        cout << bytes_read[i] << endl;
    }
   
   count = read(file, (void*)receive, 10);
      printf("The following was read in [%d]: %s\n",count,receive);
 
   close(file);
   
 
     
   return 0;
}

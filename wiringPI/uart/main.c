// Compile : gcc -Wall uart-send.c -o uart-send -lwiringPi
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
 
int main() {
 
	int fd;
 
	printf("Raspberry's sending : \n");
  if((fd = serialOpen ("/dev/ttyS0", 115200)) < 0 ){
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
  }
	while(1) {
		serialPuts(fd, "hello");
		serialFlush(fd);
		printf("%s\n", "hello");
		fflush(stdout);
		delay(1000);
	}
	return 0;
}

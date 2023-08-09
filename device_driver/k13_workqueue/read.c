
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char read_buf[32];

int main(){
  int fd;
  char option;
  printf("\t ********************************** \n");
  printf("\t ******* knghia read a file ******* \n");
  
  fd = open("/dev/etx_device", O_RDWR);
  if(fd < 0) {
    printf("\t Cannot open device file...\n");
    return 0;
  }
  while(1) {
    printf("\t ****Please Enter the Option****** \n");
    printf("\t         j. Read                   \n");
    printf("\t         q. Exit                   \n");
    printf("\t ********************************* \n");
    scanf(" %c", &option);
    printf("\t Your Option = %c\n", option);
    switch(option) {
      case 'j':
        printf("\t Data Reading ...");
        read(fd, read_buf, 32);
        printf("\t Data = %s\n\n", read_buf);
        break;
      case 'q':
        close(fd);
        exit(1);
        return 1;
      default:
        printf("\t Enter Valid option = %c\n",option);
        break;
    }
  }
  close(fd);
  return 1;
}
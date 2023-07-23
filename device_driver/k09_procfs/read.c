
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char read_buff[256];

int main(){
    int fd;
    printf("\nOpening Driver\n");
    fd = open("/proc/my_value/value", O_RDWR);
    if(fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
    }
    read(fd, read_buff, 256); 
    printf("    Value %s %d\n", read_buff, sizeof(read_buff));
    close(fd);
}
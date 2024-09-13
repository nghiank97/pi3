
#include "spi.h"

#define spi_chanel 	  	0
#define spi_speed 	  	500000
#define	spi_cs_pin		17
int spi_fd;

u08 read_drdy(void){
	return digitalRead(drdy_pin);
}

int spi_init(void){
	/* setup gpio */
	wiringPiSetupGpio();
	pinMode(spi_cs_pin, OUTPUT);
	digitalWrite(spi_cs_pin, 1);

	/* setup spi */
	if( (wiringPiSPISetup(spi_chanel, spi_speed)) <0){
		printf("SPI Setup failed\n");
		return 0;
	}
 
	spi_fd = wiringPiSPIGetFd(0);
	return 1;
}

void spi_cs_l(void){
	digitalWrite(spi_cs_pin, 0);
}
void spi_cs_h(void){
	digitalWrite(spi_cs_pin, 1);
}
void spi_write(u08 data){
	write(spi_fd,&data, 1);
}
u08 spi_read(void){
	u08 c = 0;
	read(spi_fd,&c, 1);
	return c;
}
void spi_writes(u08* pBuf, u16 len){
	write(spi_fd,pBuf, len);
}
void spi_reads(u08* pBuf, u16 len){
	read(spi_fd,pBuf, len);
}

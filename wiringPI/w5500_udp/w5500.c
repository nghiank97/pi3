#include "wiringPi.h"
#include "wiringPiSPI.h"
#include "w5500.h"

#define spi_chanel 	  0
#define spi_speed 	  500000
#define	spi_cs_pin		17
#define	w5500_rst_pin	14
int spi_fd;

int w5500_spi_init(void){
  /* setup gpio */
  wiringPiSetupGpio();
	pinMode(spi_cs_pin, OUTPUT);
	digitalWrite(spi_cs_pin, 1);
	pinMode(w5500_rst_pin, OUTPUT);
	digitalWrite(w5500_rst_pin, 1);
  /* setup spi */
	if( (wiringPiSPISetup(spi_chanel, spi_speed)) <0){
		printf("SPI Setup failed\n");
		return 0;
	}
 
	spi_fd = wiringPiSPIGetFd(0);
  return 1;
}

void w5500_reset(void){
	digitalWrite(w5500_rst_pin, 0);
	delay(1);
	digitalWrite(w5500_rst_pin, 1);
	delay(1000);
}
void w5500_cs_l(void){
	digitalWrite(spi_cs_pin, 0);
}
void w5500_cs_h(void){
	digitalWrite(spi_cs_pin, 1);
}
void w5500_spi_write(u08 data){
	write(spi_fd,&data, 1);
}
u08 w5500_spi_read(void){
	u08 c = 0;
	read(spi_fd,&c, 1);
	return c;
}
u16 w5500_spi_write_page(u08* pBuf, u16 len){
	write(spi_fd,pBuf, len);
	return len;
}
u16 w5500_spi_read_page(u08* pBuf, u16 len){
	read(spi_fd,pBuf, len);
	return len;
}

NetInfo net_info;

void w5500_init(u08 _mac[6], u08 _ip[4], u08 _sub[4], u08 _gw[4]){
  w5500_spi_init();
	w5500_reset();
	memcpy(net_info.mac, _mac, 6);
	memcpy(net_info.ip, _ip, 4);
	memcpy(net_info.sn, _sub, 4);
	memcpy(net_info.gw, _gw, 4);
#ifdef W5500_DEBUG
  printf("Registering W5500 callbacks...\r\n");
#endif
	// write to common register
	// Gateway Address
	w5500_write_access(0x0001, COMMON_REGISTER, _gw, 4);delay(1);
	// Subnet Mask Address
	w5500_write_access(0x0005, COMMON_REGISTER, _sub, 4);delay(1);
	// Source Hardware Address : MAC
	w5500_write_access(0x0009, COMMON_REGISTER, _mac, 6);delay(1);
	// Source IP Address
	w5500_write_access(0x000F, COMMON_REGISTER, _ip, 4);delay(1);
	
#ifdef W5500_DEBUG
	u08 net_mac[6];
	u08 net_ip[4];
	u08 net_sub[4];
	u08 net_gw[4];
	
	// read to common register
	// Gateway Address
	w5500_read_access(0x0001, COMMON_REGISTER, net_gw, 4);delay(1);
	// Subnet Mask Address
	w5500_read_access(0x0005, COMMON_REGISTER, net_sub, 4);delay(1);
	// Source Hardware Address : MAC
	w5500_read_access(0x0009, COMMON_REGISTER, net_mac, 6);delay(1);
	// Source IP Address
	w5500_read_access(0x000F, COMMON_REGISTER, net_ip, 4);delay(1);
	
	printf("ip: %02x.%02x.%02x.%02x\r\n", net_ip[0], net_ip[1], net_ip[2], net_ip[3]);
	printf("sn: %02x.%02x.%02x.%02x\r\n", net_sub[0], net_sub[1], net_sub[2], net_sub[3]);
	printf("gw: %02x.%02x.%02x.%02x\r\n", net_gw[0], net_gw[1], net_gw[2], net_gw[3]);
	printf("mac: %02x.%02x.%02x.%02x.%02x.%02x\r\n", net_mac[0], net_mac[1], net_mac[2], net_mac[3], net_mac[4], net_mac[5]);
#endif
}

void w5500_set_size(u08* tx_size, u08* rx_size){
	for(u08 i=0;i<8;i++){
		// set size of rx buf
		w5500_write_access(0x001E, 4*i+3, &rx_size[i], 1);
		// set size of tx buf
		w5500_write_access(0x001F, 4*i+2, &rx_size[i], 1);
	}
}

void w5500_retry_count(u08 count){
	w5500_write_access(RCR, COMMON_REGISTER, &count, 1);
}

void w5500_retry_time_value(u16 time){
	u08 time_h = (u08)(time/256);
	u08 time_l = (u08)(time&256);
	w5500_write_access(RTR0, COMMON_REGISTER, &time_h, 1);
	w5500_write_access(RTR1, COMMON_REGISTER, &time_l, 1);
}
/**
@brief   The function read the Socket n status register
@return  data of status (u08) .
*/
SOCKET_STATUS w5500_sn_sr(u08 socket){
	u08 status;
	w5500_read_access(Sn_SR, 4*socket+1, &status, 1);
	return (SOCKET_STATUS)status;
}
#ifdef W5500_DEBUG
void w5500_get_version_chip(void){
	u08 version = 0;
	w5500_read_access(CHIP_VERSION, COMMON_REGISTER, &version, 1);
	printf("Chip version %d\n",version);
}
#endif

void w5500_write_access(u16 add, u08 bsb, u08* data, u16 len){
	u08 dummy = 0;
	w5500_cs_l();
	// write address
	dummy = (add>>8)&0xFF;
	w5500_spi_write(dummy);
	dummy = add&0xFF;
	w5500_spi_write(dummy);
	dummy = (bsb<<3)&B11111000;
	// command is set write
	dummy |= (1<<2);
	// variable length data mode (VDM)
	dummy &=~ (B11<<0);
	w5500_spi_write(dummy);
	// write data
	w5500_spi_write_page(data,len);
	w5500_cs_h();
}

void w5500_read_access(u16 add, u08 bsb, u08* data, u16 len){
	u08 dummy = 0;
	w5500_cs_l();
	// write address
	dummy = (add>>8)&0xFF;
	w5500_spi_write(dummy);
	dummy = add&0xFF;
	w5500_spi_write(dummy);
	dummy = (bsb<<3)&B11111000;
	// command is set read
	dummy &=~ (1<<2);
	// variable length data mode (VDM)
	dummy &=~ (B11<<0);
	w5500_spi_write(dummy);
	// write data
	w5500_spi_read_page(data,len);
	w5500_cs_h();
}
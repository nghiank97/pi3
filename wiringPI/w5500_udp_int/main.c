
#include "main.h"
#include "w5500.h"
#include "socket.h"
#include "wizchip_conf.h"

#define spi_chanel 	  	0
#define spi_speed 	  	32000000
#define	spi_cs_pin		17
#define	w5500_rst_pin	14
#define	w5500_int_pin	15
int spi_fd;

u08 check = 0;

void w5500_hw_int_cp(void) {
  if((getSn_IR(0) & Sn_IR_RECV) && check == 0){
    check = 1;
  }
}

int w5500_spi_init(void){
  /* setup gpio cs, rst */
  wiringPiSetupGpio();
  pinMode(spi_cs_pin, OUTPUT);
  digitalWrite(spi_cs_pin, 1);
  pinMode(w5500_rst_pin, OUTPUT);
  digitalWrite(w5500_rst_pin, 1);
   
  // set Pin 15 generate an interrupt on high-to-low transitions
  if ( wiringPiISR (w5500_int_pin, INT_EDGE_FALLING, &w5500_hw_int_cp) < 0 ) {
      printf ("Unable to setup ISR: %s\n");
      return 1;
  }

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
void w5500_spi_write_page(u08* pBuf, u16 len){
	write(spi_fd,pBuf, len);
}
void w5500_spi_read_page(u08* pBuf, u16 len){
	read(spi_fd,pBuf, len);
}

u08 txsize[8] = {4,2,2,2,2,2,2,2};
u08 rxsize[8] = {4,2,2,2,2,2,2,2};

wiz_NetInfo gWIZNETINFO = {
							.mac = {0x08,0x10,0x19,0x97,0x25,0x25},
							.ip = {192,168,7,47},
							.sn = {255, 255, 255, 0},
							.gw = {192, 168, 7, 1},
							.dns = {8, 8, 8, 8},
							.dhcp = NETINFO_STATIC
};
#define W5500_DEBUG
#define ETH_MAX_BUF_SIZE	2048
u08 buffer[ETH_MAX_BUF_SIZE];

/* udp info */
u08 remote_ip[4]={192,168,7,100};
u16 remote_port=5002;
u16 local_port=5004;
u16 len=0;

extern void setup(){
	w5500_spi_init();
	/* initialize w5500 */
	w5500_reset();
	reg_wizchip_cs_cbfunc(w5500_cs_l, w5500_cs_h);
	reg_wizchip_spi_cbfunc(w5500_spi_read, w5500_spi_write);
	reg_wizchip_spiburst_cbfunc(w5500_spi_read_page, w5500_spi_write_page);
	wizchip_init(txsize, rxsize);
	wizchip_setnetinfo(&gWIZNETINFO);
#ifdef W5500_DEBUG
	u08 net_ip[4] = {0};
	u08 net_sub[4] = {0};
	u08 net_gw[4] = {0};
	u08 net_mac[6] = {0};
	
	getSIPR(net_ip);
	getSHAR(net_mac);
	getSUBR(net_sub);
	getGAR(net_gw);
	
	printf("ip: %03d.%03d.%03d.%03d\r\n", net_ip[0], net_ip[1], net_ip[2], net_ip[3]);
	printf("sn: %03d.%03d.%03d.%03d\r\n", net_sub[0], net_sub[1], net_sub[2], net_sub[3]);
	printf("gw: %03d.%03d.%03d.%03d\r\n", net_gw[0], net_gw[1], net_gw[2], net_gw[3]);
	printf("mac: %02x.%02x.%02x.%02x.%02x.%02x\r\n", net_mac[0], net_mac[1], net_mac[2], net_mac[3], net_mac[4], net_mac[5]);
#endif
	/* initialize interrupt */
	// Sn_IMR - RECV interrupt
	setSn_IMR(0, Sn_IR_RECV);
	// enable socket 0 interrupt mark
	setSIMR(1<<0);
	setINTLEVEL(50);
}

extern void loop(){
	u08 s = getSn_SR(0);
	switch(s){
		case SOCK_UDP:
			len=getSn_RX_RSR(0);
			if(len>0){
				if (check == 1){ 
  					memset(buffer,0,len+1);
  					recvfrom(0,buffer, len, remote_ip,&remote_port);
  					printf("%s \r\n", buffer);
					sendto(0,buffer,len, remote_ip, remote_port);
					check = 0;
				}
			}
			if(getSn_IR(0) & Sn_IR_RECV){
				setSn_IR(0, Sn_IR_RECV);
			}

			break;
		case SOCK_CLOSED:
			socket(0,Sn_MR_UDP,local_port,0);
			break;
		default:
			break;
	}
  delay(1);
}

int main(){
  setup();
  while(1){
    loop();
  }
}

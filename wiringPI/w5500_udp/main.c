
#include "main.h"
#include "w5500.h"
#include "socket.h"
u08 mac[6]={0x08,0x10,0x19,0x97,0x25,0x25};   /* mac */
u08 ip[4]={192,168,7,100};                   	/* ip */
u08 sub[4]={255,255,255,0};                   /* subnet */
u08 gw[4]={192,168,7,1};                     	/* gateway */

u08 txsize[8] = {2,2,2,2,2,2,2,2};
u08 rxsize[8] = {2,2,2,2,2,2,2,2};

u08 _remote_ip[4] = {0};
u08 _port[2] = {0};
u08 _remote_port[2] = {0};
	

/* udp info */
SOCKET_STATUS status_socket = SOCK_CLOSED;
u08 dummy = 0;
u08 port[2] = {0};
u16 local_port = 5002;
u16 remote_port = 5004;
u08 remote_ip[4]={192,168,7,10};

extern void setup(){
	w5500_init(mac, ip, sub, gw);
	w5500_set_size(txsize, rxsize);
	
	w5500_retry_time_value(2000);
	w5500_retry_count(3);
	/* setup udp */
  socket_close(0);
	/* 1. configures the protocol mode of socket 0 */
	dummy = Sn_MR_PROTOCOL_UDP;
	w5500_write_access(Sn_MR, SOCKET_0_REGISTER, &dummy, 1);delay(1);
	// write local port
	port[0] = (u08)(local_port>> 8);
	port[1] = (u08)(local_port & 0x00ff);
	w5500_write_access(Sn_PORT ,SOCKET_0_REGISTER, port, 2);delay(1);
	// write destination ip address port
  w5500_write_access(Sn_DIPR ,SOCKET_0_REGISTER, remote_ip, 4);delay(1);
	// write destination port
	port[0] = (u08)(remote_port >> 8);
	port[1] = (u08)(remote_port & 0x00ff);
	w5500_write_access(Sn_DPORT ,SOCKET_0_REGISTER, port, 2);delay(1);
	
#ifdef W5500_DEBUG
	w5500_read_access(Sn_PORT ,SOCKET_0_REGISTER, _port, 2);delay(1);
	w5500_read_access(Sn_DIPR ,SOCKET_0_REGISTER, _remote_ip, 4);delay(1);
	w5500_read_access(Sn_DPORT ,SOCKET_0_REGISTER, _remote_port, 2);delay(1);
	
	
	printf("\r\nUDP INFO............\r\n");
	printf("port: %04d\r\n", _port[0]*256 + _port[1]);
	printf("remote ip: %03d.%03d.%03d.%03d\r\n", _remote_ip[0], _remote_ip[1], _remote_ip[2], _remote_ip[3]);
	printf("remote port: %04d\r\n", _remote_port[0]*256 + _remote_port[1]);
#endif
	
	// opened
	dummy = Sn_CR_OPEN;
	w5500_write_access(Sn_CR ,SOCKET_0_REGISTER, &dummy, 1);delay(1);
	// check Sn_CR
	while(1){
		dummy = 0;
		w5500_read_access(Sn_CR ,SOCKET_0_REGISTER, &dummy, 1);delay(1);
		if (dummy == 0x01){
			#ifdef W5500_DEBUG
				printf("Socket opened \r\n");
			#endif
			break;
		}
	}
}

extern void loop(){
	status_socket = w5500_sn_sr(0);
	switch (status_socket){
		case SOCK_CLOSED:{
		}
		case SOCK_UDP:{
			break;
		}
		default:
			break;
	}
#ifdef W5500_DEBUG
	printf("\r\nSOCKET STATUS : %02x\r\n",status_socket);
#endif
	delay(100);
}

int main(){
  setup();
  while(1){
    loop();
  }
}
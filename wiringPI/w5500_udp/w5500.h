#ifndef  _W5500_H_
#define  _W5500_H_
#include "main.h"

#define W5500_DEBUG

/* BLock select bits */
#define COMMON_REGISTER             B00000

#define SOCKET_0_REGISTER           B00001	// 1
#define SOCKET_0_TX             		B00010	// 2
#define SOCKET_0_RX             		B00011	// 3

#define SOCKET_1_REGISTER           B00101	// 5
#define SOCKET_1_TX             		B00110	// 6
#define SOCKET_1_RX             		B00111	// 7

#define SOCKET_2_REGISTER           B01001	// 9
#define SOCKET_2_TX             		B01010	// 10
#define SOCKET_2_RX             		B01011	// 11

#define SOCKET_3_REGISTER           B01101	// 13
#define SOCKET_3_TX             		B01110	// 14
#define SOCKET_3_RX             		B01111	// 15

#define SOCKET_4_REGISTER           B10001	// 17
#define SOCKET_4_TX             		B10010	// 18
#define SOCKET_4_RX             		B10011	// 19

#define SOCKET_5_REGISTER           B10101	// 21
#define SOCKET_5_TX             		B10110	// 22
#define SOCKET_5_RX             		B10111	// 23

#define SOCKET_6_REGISTER           B11001	// 25
#define SOCKET_6_TX             		B11010	// 26
#define SOCKET_6_RX             		B11011	// 27

#define SOCKET_7_REGISTER           B11101	// 29
#define SOCKET_7_TX             		B11110	// 30
#define SOCKET_7_RX             		B11111	// 31

/* 1-5-9-13-17-21-25-29  : 4*n+1 Register */
/* 2-6-10-14-18-22-26-30 : 4*n+2 TX */
/* 3-7-11-15-19-23-27-31 : 4*n+3 RX*/

#define RTR0												0x0019
#define RTR1												0x001A
#define RCR													0x001B

#define Sn_MR												0x0000
#define Sn_CR												0x0001
#define Sn_IR												0x0002
#define Sn_SR												0x0003

#define Sn_PORT											0x0004 // length 2
#define Sn_DHAR											0x0006 // length 6
#define Sn_DIPR											0x000C // length 4
#define Sn_DPORT										0x0010 // length 2

#define Sn_RXBUF_SIZE								0x001E
#define Sn_TXBUF_SIZE								0x001F

#define CHIP_VERSION								0x0039

#define Sn_MR_PROTOCOL_CLOSE				(0<<3)|(0<<2)|(0<<1)|(0<<0)
#define Sn_MR_PROTOCOL_TCP					(0<<3)|(0<<2)|(0<<1)|(1<<0)
#define Sn_MR_PROTOCOL_UDP					(0<<3)|(0<<2)|(1<<1)|(0<<0)
#define Sn_MR_PROTOCOL_MACRAW				(0<<3)|(1<<2)|(0<<1)|(0<<0)

#define Sn_CR_OPEN									0x01
#define Sn_CR_LISTEN								0x02
#define Sn_CR_CONNECT								0x04
#define Sn_CR_DISCON								0x08
#define Sn_CR_CLOSE									0x10
#define Sn_CR_SEND									0x20
#define Sn_CR_SEND_MAC							0x21
#define Sn_CR_SEND_KEED							0x22
#define Sn_CR_RECV									0x40

typedef enum{
	SOCK_CLOSED = 0x00,		
	SOCK_INIT = 0x13,		
	SOCK_LISTEN = 0x14,		
	SOCK_ESTABLISHED = 0x17,		
	SOCK_CLOSE_WAIT = 0x1C,		
	SOCK_UDP = 0x22,
	SOCK_MACRAW = 0x42
}SOCKET_STATUS;	

typedef struct{
   u08 mac[6];
   u08 ip[4];
   u08 sn[4];
   u08 gw[4];
}NetInfo;

void w5500_init(u08 _mac[6], u08 _ip[4], u08 _sub[4], u08 _gw[4]);
void w5500_set_size(u08* tx_size, u08* rx_size);
void w5500_retry_time_value(u16 time);
void w5500_retry_count(u08 count);
SOCKET_STATUS w5500_sn_sr(u08 socket);

void w5500_write_access(u16 add, u08 bsb, u08* data, u16 len);
void w5500_read_access(u16 add, u08 bsb, u08* data, u16 len);

#ifdef W5500_DEBUG
void w5500_get_version_chip(void);
#endif

#endif
#ifndef ETHERCAT_H
#define ETHERCAT_H

#include "main.h"
/*
Node addressing
*/

/*
EtherCAT command types
*/

#define NOP 	0 	// No Operation
#define APRD 	1 	// Auto Increment Read
#define APWR 	2 	// Auto Increment Write
#define APRW 	3 	// Auto Increment Read Write
#define FPRD 	4 	// Configured Address Read
#define FPWR 	5 	// Configured Address Write
#define FPRW 	6 	// Configured Address Read Write
#define BRD 	7 	// Broadcast Read
#define BWR 	8 	// Broadcast Write
#define BRW 	9 	// Broadcast Read Write
#define LRD 	10 	// Logical Memory Read
#define LWR 	11 	// Logical Memory Write
#define LRW 	12 	// Logical Memory Read Write
#define ARMW 	13 	// Auto Increment Read Multiple Write

extern u16 ethercat_udp_load_datagram(u08* dest,u08 cmd, u08 idx, u32 add, u16 RCM, u16 IRQ, u16 workingcount, u08* data, u16 len);
extern void ethercat_udp_send(void);
#endif
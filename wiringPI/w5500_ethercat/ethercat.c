#include "main.h"
#include "ethercat.h"

extern u16 ethercat_udp_load_datagram(u08* dest,u08 cmd, u08 idx, u32 add, u16 RCM, u16 IRQ, u16 workingcount, u08* data, u16 len){
	// Cmd
	dest[0] = cmd;
	// Idx
	dest[1] = idx;
	// Address : address for logic memory r/w
	dest[2]= (add&0x0000FFFF)%256;
	dest[3]= (add&0x0000FFFF)/256;
	dest[4]= ((add&0xFFFF0000)>>16)/256;
	dest[5]= ((add&0xFFFF0000)>>16)%256;
	// len-R-C-M
	//  Last indicator: More EtherCAT datagrams will follow
	RCM = RCM + (len&0x07FF);
	dest[6] = RCM&0xFF;
	dest[7] = (RCM>>8)&0xFF;
	// IRQ
	dest[8] = IRQ&0xFF;
	dest[9] = (IRQ>>8)&0xFF;
	copy_arr(&dest[10],  data, len);
	// Working Cnt
	dest[10 + len] = (workingcount>>8)&0xFF;
	dest[11 + len] = workingcount&0xFF;
	return 12 + len;
}

extern void ethercat_udp_send(void){
}

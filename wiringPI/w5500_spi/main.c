
#include "main.h"
#include "w5500.h"

u08 mac[6]={0x08,0x10,0x19,0x97,0x25,0x25};   /* mac */
u08 ip[4]={192,168,7,47};                   	/* ip */
u08 sub[4]={255,255,255,0};                   /* subnet */
u08 gw[4]={192,168,7,1};                     	/* gateway */

u08 txsize[8] = {2,2,2,2,2,2,2,2};
u08 rxsize[8] = {2,2,2,2,2,2,2,2};

extern void setup(){
	w5500_init(mac, ip, sub, gw);
	w5500_set_size(txsize, rxsize);
	
	w5500_retry_time_value(2000);
	w5500_retry_count(3);
}

extern void loop(){
}

int main(){
  setup();
  while(1){
    loop();
  }
}
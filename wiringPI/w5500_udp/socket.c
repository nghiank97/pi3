#include "socket.h"
#include "w5500.h"
void socket_init(u08 socket, u08 protocol, u16 port, u08 flag){
	
}

void socket_close(u08 socket){
  printf("Socket close \n");
  while(1){
    u08 dummy = Sn_CR_CLOSE;
    w5500_write_access(Sn_CR ,SOCKET_0_REGISTER, &dummy, 1);delay(100);
		dummy = 0;
		w5500_read_access(Sn_CR ,SOCKET_0_REGISTER, &dummy, 1);delay(1);
    printf("Socket status : %d\n",dummy);
		if (dummy == Sn_CR_CLOSE){
			#ifdef W5500_DEBUG
				printf("Socket finished : %d\n",dummy);
			#endif
			break;
 		}
	}
}
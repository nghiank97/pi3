#ifndef  _SOCKET_H_
#define  _SOCKET_H_
#include "main.h"

void socket_init(u08 socket, u08 protocol, u16 port, u08 flag);
void socket_close(u08 socket);

#endif
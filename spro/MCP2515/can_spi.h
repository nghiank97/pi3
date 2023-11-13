#ifndef CAN_SPI_H
#define	CAN_SPI_H

#include "binary.h"

typedef union {
  struct {
    u08 idType;
    uint32_t id;
    u08 dlc;
    u08 data0;
    u08 data1;
    u08 data2;
    u08 data3;
    u08 data4;
    u08 data5;
    u08 data6;
    u08 data7;
  } frame;
  u08 array[14];
} uCAN_MSG;

#define dSTANDARD_CAN_MSG_ID_2_0B 1
#define dEXTENDED_CAN_MSG_ID_2_0B 2

bool CANSPI_Initialize(void);
void CANSPI_Sleep(void);
u08 CANSPI_Transmit(uCAN_MSG *tempCanMsg);
u08 CANSPI_Receive(uCAN_MSG *tempCanMsg);
u08 CANSPI_messagesInBuffer(void);
u08 CANSPI_isBussOff(void);
u08 CANSPI_isRxErrorPassive(void);
u08 CANSPI_isTxErrorPassive(void);

#endif	/* CAN_SPI_H */
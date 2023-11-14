#ifndef MCP2515_H
#define MCP2515_H

#pragma anon_unions
#include "binary.h"

/* MCP2515 SPI Instruction Set */
#define MCP2515_RESET           0xC0

#define MCP2515_READ            0x03
#define MCP2515_READ_RXB0SIDH   0x90
#define MCP2515_READ_RXB0D0     0x92
#define MCP2515_READ_RXB1SIDH   0x94
#define MCP2515_READ_RXB1D0     0x96

#define MCP2515_WRITE           0x02
#define MCP2515_LOAD_TXB0SIDH   0x40    /* TX0 ID location */
#define MCP2515_LOAD_TXB0D0     0x41    /* TX0 Data location */
#define MCP2515_LOAD_TXB1SIDH   0x42    /* TX1 ID location */
#define MCP2515_LOAD_TXB1D0     0x43    /* TX1 Data location */
#define MCP2515_LOAD_TXB2SIDH   0x44    /* TX2 ID location */
#define MCP2515_LOAD_TXB2D0     0x45    /* TX2 Data location */

#define MCP2515_RTS_TX0         0x81
#define MCP2515_RTS_TX1         0x82
#define MCP2515_RTS_TX2         0x84
#define MCP2515_RTS_ALL         0x87
#define MCP2515_READ_STATUS     0xA0
#define MCP2515_RX_STATUS       0xB0
#define MCP2515_BIT_MOD         0x05

/* MCP25152515 Register Adresses */
#define MCP2515_RXF0SIDH	0x00
#define MCP2515_RXF0SIDL	0x01
#define MCP2515_RXF0EID8	0x02
#define MCP2515_RXF0EID0	0x03
#define MCP2515_RXF1SIDH	0x04
#define MCP2515_RXF1SIDL	0x05
#define MCP2515_RXF1EID8	0x06
#define MCP2515_RXF1EID0	0x07
#define MCP2515_RXF2SIDH	0x08
#define MCP2515_RXF2SIDL	0x09
#define MCP2515_RXF2EID8	0x0A
#define MCP2515_RXF2EID0	0x0B
#define MCP2515_CANSTAT		0x0E
#define MCP2515_CANCTRL		0x0F

#define MCP2515_RXF3SIDH	0x10
#define MCP2515_RXF3SIDL	0x11
#define MCP2515_RXF3EID8	0x12
#define MCP2515_RXF3EID0	0x13
#define MCP2515_RXF4SIDH	0x14
#define MCP2515_RXF4SIDL	0x15
#define MCP2515_RXF4EID8	0x16
#define MCP2515_RXF4EID0	0x17
#define MCP2515_RXF5SIDH	0x18
#define MCP2515_RXF5SIDL	0x19
#define MCP2515_RXF5EID8	0x1A
#define MCP2515_RXF5EID0	0x1B
#define MCP2515_TEC		0x1C
#define MCP2515_REC		0x1D

#define MCP2515_RXM0SIDH	0x20
#define MCP2515_RXM0SIDL	0x21
#define MCP2515_RXM0EID8	0x22
#define MCP2515_RXM0EID0	0x23
#define MCP2515_RXM1SIDH	0x24
#define MCP2515_RXM1SIDL	0x25
#define MCP2515_RXM1EID8	0x26
#define MCP2515_RXM1EID0	0x27
#define MCP2515_CNF3		0x28
#define MCP2515_CNF2		0x29
#define MCP2515_CNF1		0x2A
#define MCP2515_CANINTE		0x2B
#define MCP2515_CANINTF		0x2C
#define MCP2515_EFLG		0x2D

#define MCP2515_TXB0CTRL	0x30
#define MCP2515_TXB1CTRL	0x40
#define MCP2515_TXB2CTRL	0x50
#define MCP2515_RXB0CTRL	0x60
#define MCP2515_RXB0SIDH	0x61
#define MCP2515_RXB1CTRL	0x70
#define MCP2515_RXB1SIDH	0x71

/* Defines for Rx Status */
#define MSG_IN_RXB0             0x01
#define MSG_IN_RXB1             0x02
#define MSG_IN_BOTH_BUFFERS     0x03

typedef union{
  struct{
    unsigned RX0IF      : 1;
    unsigned RX1IF      : 1;
    unsigned TXB0REQ    : 1;
    unsigned TX0IF      : 1;
    unsigned TXB1REQ    : 1;
    unsigned TX1IF      : 1;
    unsigned TXB2REQ    : 1;
    unsigned TX2IF      : 1;
  };
  u08 ctrl_status;  
}ctrl_status_t;

typedef union{
  struct{
    unsigned filter     : 3;
    unsigned msgType    : 2;
    unsigned unusedBit  : 1;
    unsigned rxBuffer   : 2;
  };
  u08 ctrl_rx_status;
}ctrl_rx_status_t;

typedef union{
  struct{
    unsigned EWARN      :1;
    unsigned RXWAR      :1;
    unsigned TXWAR      :1;
    unsigned RXEP       :1;
    unsigned TXEP       :1;
    unsigned TXBO       :1;
    unsigned RX0OVR     :1;
    unsigned RX1OVR     :1;  
  };
  u08 error_flag_reg;
}ctrl_error_status_t;

typedef union{
  struct{
    u08 RXBnSIDH;
    u08 RXBnSIDL;
    u08 RXBnEID8;
    u08 RXBnEID0;
    u08 RXBnDLC;
    u08 RXBnD0;
    u08 RXBnD1;
    u08 RXBnD2;
    u08 RXBnD3;
    u08 RXBnD4;
    u08 RXBnD5;
    u08 RXBnD6;
    u08 RXBnD7;
  };
  u08 rx_reg_array[13];
}rx_reg_t;

/* MCP2515 Registers */
typedef struct{
  u08 RXF0SIDH;
  u08 RXF0SIDL;
  u08 RXF0EID8;
  u08 RXF0EID0;
}RXF0;

typedef struct{
  u08 RXF1SIDH;
  u08 RXF1SIDL;
  u08 RXF1EID8;
  u08 RXF1EID0;
}RXF1;

typedef struct{
  u08 RXF2SIDH;
  u08 RXF2SIDL;
  u08 RXF2EID8;
  u08 RXF2EID0;
}RXF2;

typedef struct{
  u08 RXF3SIDH;
  u08 RXF3SIDL;
  u08 RXF3EID8;
  u08 RXF3EID0;
}RXF3;

typedef struct{
  u08 RXF4SIDH;
  u08 RXF4SIDL;
  u08 RXF4EID8;
  u08 RXF4EID0;
}RXF4;

typedef struct{
  u08 RXF5SIDH;
  u08 RXF5SIDL;
  u08 RXF5EID8;
  u08 RXF5EID0;
}RXF5;

typedef struct{
  u08 RXM0SIDH;
  u08 RXM0SIDL;
  u08 RXM0EID8;
  u08 RXM0EID0;
}RXM0;

typedef struct{
  u08 RXM1SIDH;
  u08 RXM1SIDL;
  u08 RXM1EID8;
  u08 RXM1EID0;
}RXM1;

typedef struct{
  u08 tempSIDH;
  u08 tempSIDL;
  u08 tempEID8;
  u08 tempEID0;
}id_reg_t;

/* Functions */
bool MCP2515_Initialize(void);
bool MCP2515_SetConfigMode(void);
bool MCP2515_SetNormalMode(void);
bool MCP2515_SetSleepMode(void);
void MCP2515_Reset(void);
u08 MCP2515_ReadByte (u08 address);
void MCP2515_ReadRxSequence(u08 instruction, u08 *data, u08 length);
void MCP2515_WriteByte(u08 address, u08 data);
void MCP2515_WriteByteSequence(u08 startAddress, u08 endAddress, u08 *data);
void MCP2515_LoadTxSequence(u08 instruction, u08 *idReg, u08 dlc, u08 *data);
void MCP2515_LoadTxBuffer(u08 instruction, u08 data);
void MCP2515_RequestToSend(u08 instruction);
u08 MCP2515_ReadStatus(void);
u08 MCP2515_GetRxStatus(void);
void MCP2515_BitModify(u08 address, u08 mask, u08 data);

#endif
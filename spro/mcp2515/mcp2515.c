
#include "binary.h"
#include "mcp2515.h"

/* MCP2515 초기화 */
bool MCP2515_Initialize(void)
{
  MCP2515_CS_HIGH();    
  return false;
}

/* MCP2515 를 설정모드로 전환 */
bool MCP2515_SetConfigMode(void)
{
  /* CANCTRL Register Configuration 모드 설정 */  
  MCP2515_WriteByte(MCP2515_CANCTRL, 0x80);
  
  u08 loop = 10;
  
  do {    
    /* 모드전환 확인 */    
    if((MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0) == 0x80)
      return true;
    
    loop--;
  } while(loop > 0); 
  
  return false;
}

/* MCP2515 를 Normal모드로 전환 */
bool MCP2515_SetNormalMode(void)
{
  /* CANCTRL Register Normal 모드 설정 */  
  MCP2515_WriteByte(MCP2515_CANCTRL, 0x00);
  
  u08 loop = 10;
  
  do {    
    /* 모드전환 확인 */    
    if((MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0) == 0x00)
      return true;
    
    loop--;
  } while(loop > 0);
  
  return false;
}

/* MCP2515 를 Sleep 모드로 전환 */
bool MCP2515_SetSleepMode(void)
{
  /* CANCTRL Register Sleep 모드 설정 */  
  MCP2515_WriteByte(MCP2515_CANCTRL, 0x20);
  
  u08 loop = 10;
  
  do {    
    /* 모드전환 확인 */    
    if((MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0) == 0x20)
      return true;
    
    loop--;
  } while(loop > 0);
  
  return false;
}

/* MCP2515 SPI-Reset */
void MCP2515_Reset(void)
{    
  MCP2515_CS_LOW();
      
  SPI_Tx(MCP2515_RESET);
      
  MCP2515_CS_HIGH();
}

/* 1바이트 읽기 */
u08 MCP2515_ReadByte (u08 address)
{
  u08 retVal;
  
  MCP2515_CS_LOW();
  
  SPI_Tx(MCP2515_READ);
  SPI_Tx(address);
  retVal = SPI_Rx();
      
  MCP2515_CS_HIGH();
  
  return retVal;
}

/* Sequential Bytes 읽기 */
void MCP2515_ReadRxSequence(u08 instruction, u08 *data, u08 length)
{
  MCP2515_CS_LOW();
  
  SPI_Tx(instruction);        
  SPI_RxBuffer(data, length);
    
  MCP2515_CS_HIGH();
}

/* 1바이트 쓰기 */
void MCP2515_WriteByte(u08 address, u08 data)
{    
  MCP2515_CS_LOW();  
  
  SPI_Tx(MCP2515_WRITE);
  SPI_Tx(address);
  SPI_Tx(data);  
    
  MCP2515_CS_HIGH();
}

/* Sequential Bytes 쓰기 */
void MCP2515_WriteByteSequence(u08 startAddress, u08 endAddress, u08 *data)
{    
  MCP2515_CS_LOW();
  
  SPI_Tx(MCP2515_WRITE);
  SPI_Tx(startAddress);
  SPI_TxBuffer(data, (endAddress - startAddress + 1));
  
  MCP2515_CS_HIGH();
}

/* TxBuffer에 Sequential Bytes 쓰기 */
void MCP2515_LoadTxSequence(u08 instruction, u08 *idReg, u08 dlc, u08 *data)
{    
  MCP2515_CS_LOW();
  
  SPI_Tx(instruction);
  SPI_TxBuffer(idReg, 4);
  SPI_Tx(dlc);
  SPI_TxBuffer(data, dlc);
       
  MCP2515_CS_HIGH();
}

/* TxBuffer에 1 Bytes 쓰기 */
void MCP2515_LoadTxBuffer(u08 instruction, u08 data)
{
  MCP2515_CS_LOW();
  
  SPI_Tx(instruction);
  SPI_Tx(data);
        
  MCP2515_CS_HIGH();
}

/* RTS 명령을 통해서 TxBuffer 전송 */
void MCP2515_RequestToSend(u08 instruction)
{
  MCP2515_CS_LOW();
  
  SPI_Tx(instruction);
      
  MCP2515_CS_HIGH();
}

/* MCP2515 Status 확인 */
u08 MCP2515_ReadStatus(void)
{
  u08 retVal;
  
  MCP2515_CS_LOW();
  
  SPI_Tx(MCP2515_READ_STATUS);
  retVal = SPI_Rx();
        
  MCP2515_CS_HIGH();
  
  return retVal;
}

/* MCP2515 RxStatus 레지스터 확인 */
u08 MCP2515_GetRxStatus(void)
{
  u08 retVal;
  
  MCP2515_CS_LOW();
  
  SPI_Tx(MCP2515_RX_STATUS);
  retVal = SPI_Rx();
        
  MCP2515_CS_HIGH();
  
  return retVal;
}

/* 레지스터 값 변경 */
void MCP2515_BitModify(u08 address, u08 mask, u08 data)
{    
  MCP2515_CS_LOW();
  
  SPI_Tx(MCP2515_BIT_MOD);
  SPI_Tx(address);
  SPI_Tx(mask);
  SPI_Tx(data);
        
  MCP2515_CS_HIGH();
}

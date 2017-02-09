#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "stm32f10x.h"

void InitEepRom(void);
u8 ReadAByteFromEepRom(u16 ReadAddr);
void WriteAByteToEepRom(u16 WriteAddr,u8 DataToWrite);
void ReadDataFromEepRom(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);
void WriteDataToEepRom(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);








#endif




# EepRom例程（iic方式）

型号AT24C02

## 接口

`u8 ReadAByteFromEepRom(u16 ReadAddr)`：从eeprom读取一个字节数据

`void ReadDataFromEepRom(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)`：从eeprom循环读取数据

`void WriteAByteToEepRom(u16 WriteAddr,u8 DataToWrite)`：向eeprom写入一个字节数据

`void WriteDataToEepRom(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)`：向eeprom循环写入数据


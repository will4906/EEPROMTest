#include "eeprom.h"
#include "iic.h"
#include "delay.h"

/*******************************************************************
* 函数功能：初始化EEPROM
* 参数：		无
* 返回值：  无
*******************************************************************/
void InitEepRom(void)		//AT24C02
{
	InitIicCon();            //IIC初始化
}

/*******************************************************************
* 函数功能：从EEPROM读取一字节数据
* 参数：		无
* 返回值：  无
*******************************************************************/
u8 ReadAByteFromEepRom(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
   
	StartIic();                               //IIC开始信号
	SendIicAByte(0XA0 + ((ReadAddr / 256) << 1));   //发送器件地址0XA0,写数据 	   
	
	WaitForIicAck();                //等待应答
  SendIicAByte(ReadAddr % 256);   //发送低地址
	WaitForIicAck();	               //等待应答 
	StartIic();  	 	           //IIC开始信号
	SendIicAByte(0XA1);           //进入接收模式			   
	WaitForIicAck();	               //等待应答
  temp = ReadIicAByte(0);		   //读一字节数据     
  StopIic();                    //产生一个停止条件	    
	return temp;                   //返回读取的数据
}

/*******************************************************************
* 函数功能：向EEPROM写入一字节数据
* 参数：		无
* 返回值：  无
*******************************************************************/
void WriteAByteToEepRom(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
  StartIic();                                 //开始信号
	SendIicAByte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	WaitForIicAck();	                //等待应答
  SendIicAByte(WriteAddr%256);   //发送低地址
	WaitForIicAck(); 	 	        //等待应答									  		   
	SendIicAByte(DataToWrite);     //发送字节							   
	WaitForIicAck();  		        //等待应答	   
  StopIic();                     //产生一个停止条件 	  
	delay_ms(5);	                //延时
}

/*******************************************************************
* 函数功能：从EEPROM循环读取数据
* 参数：		WriteAddr :开始写入的地址,pBuffer  :数据数组首地址,NumToRead:要写入数据的个数
* 返回值：  无
*******************************************************************/
void ReadDataFromEepRom(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)   //循环的一个一个字节的独处，存放在pBuffer指向的空间内
	{
		*pBuffer++=ReadAByteFromEepRom(ReadAddr++);	
		NumToRead--;
	}
}

/*******************************************************************
* 函数功能：向EEPROM循环写入数据
* 参数：		WriteAddr :开始写入的地址,pBuffer  :数据数组首地址,NumToRead:要写入数据的个数
* 返回值：  无
*******************************************************************/
void WriteDataToEepRom(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--) //循环的一个一个的把存放在pBuffer指向的空间内的字节写入，
	{
		WriteAByteToEepRom(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}






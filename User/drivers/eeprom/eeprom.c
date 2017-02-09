#include "eeprom.h"
#include "iic.h"
#include "delay.h"

/*******************************************************************
* �������ܣ���ʼ��EEPROM
* ������		��
* ����ֵ��  ��
*******************************************************************/
void InitEepRom(void)		//AT24C02
{
	InitIicCon();            //IIC��ʼ��
}

/*******************************************************************
* �������ܣ���EEPROM��ȡһ�ֽ�����
* ������		��
* ����ֵ��  ��
*******************************************************************/
u8 ReadAByteFromEepRom(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
   
	StartIic();                               //IIC��ʼ�ź�
	SendIicAByte(0XA0 + ((ReadAddr / 256) << 1));   //����������ַ0XA0,д���� 	   
	
	WaitForIicAck();                //�ȴ�Ӧ��
  SendIicAByte(ReadAddr % 256);   //���͵͵�ַ
	WaitForIicAck();	               //�ȴ�Ӧ�� 
	StartIic();  	 	           //IIC��ʼ�ź�
	SendIicAByte(0XA1);           //�������ģʽ			   
	WaitForIicAck();	               //�ȴ�Ӧ��
  temp = ReadIicAByte(0);		   //��һ�ֽ�����     
  StopIic();                    //����һ��ֹͣ����	    
	return temp;                   //���ض�ȡ������
}

/*******************************************************************
* �������ܣ���EEPROMд��һ�ֽ�����
* ������		��
* ����ֵ��  ��
*******************************************************************/
void WriteAByteToEepRom(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
  StartIic();                                 //��ʼ�ź�
	SendIicAByte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	WaitForIicAck();	                //�ȴ�Ӧ��
  SendIicAByte(WriteAddr%256);   //���͵͵�ַ
	WaitForIicAck(); 	 	        //�ȴ�Ӧ��									  		   
	SendIicAByte(DataToWrite);     //�����ֽ�							   
	WaitForIicAck();  		        //�ȴ�Ӧ��	   
  StopIic();                     //����һ��ֹͣ���� 	  
	delay_ms(5);	                //��ʱ
}

/*******************************************************************
* �������ܣ���EEPROMѭ����ȡ����
* ������		WriteAddr :��ʼд��ĵ�ַ,pBuffer  :���������׵�ַ,NumToRead:Ҫд�����ݵĸ���
* ����ֵ��  ��
*******************************************************************/
void ReadDataFromEepRom(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)   //ѭ����һ��һ���ֽڵĶ����������pBufferָ��Ŀռ���
	{
		*pBuffer++=ReadAByteFromEepRom(ReadAddr++);	
		NumToRead--;
	}
}

/*******************************************************************
* �������ܣ���EEPROMѭ��д������
* ������		WriteAddr :��ʼд��ĵ�ַ,pBuffer  :���������׵�ַ,NumToRead:Ҫд�����ݵĸ���
* ����ֵ��  ��
*******************************************************************/
void WriteDataToEepRom(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--) //ѭ����һ��һ���İѴ����pBufferָ��Ŀռ��ڵ��ֽ�д�룬
	{
		WriteAByteToEepRom(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}






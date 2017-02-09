#include "stm32f10x.h"
#include "delay.h"
#include "TotalTimer.h"
#include "led.h"
#include "Uart.h"
#include "eeprom.h"
#include <string.h>
#include "UartCom2.h"

#define BUFF_SIZE sizeof(Buffer)	      //���ݳ���  

const u8 Buffer[]={"123456 TEST !\r\n"};   //д��24c02������

int main()
{ 	
	u8 readbuf[BUFF_SIZE];
	
	delay_init();         //��ʱ��ʼ��
	InitUartCon();  //����1��ʼ��	
	InitEepRom();	      //24c02��ʼ��

	while(1)
	{
		WriteDataToEepRom(0,(u8*)Buffer,BUFF_SIZE);  //д������
		delay_ms(10);                            //��ʱ10ms
		ReadDataFromEepRom(0,readbuf,BUFF_SIZE);       //��ȡ����
		delay_ms(10);                            //��ʱ10ms
		
		//SendUartData(0,readbuf,BUFF_SIZE);
		printf2("%s",readbuf);                    //�����������
		memset(readbuf,0,BUFF_SIZE);             //�����ȡ���������ݣ�ȫ��Ϊ0
		
		delay_ms(500);                           //��ʱ500ms
	}
}

#include "stm32f10x.h"
#include "delay.h"
#include "TotalTimer.h"
#include "led.h"
#include "Uart.h"
#include "eeprom.h"
#include <string.h>
#include "UartCom2.h"

#define BUFF_SIZE sizeof(Buffer)	      //数据长度  

const u8 Buffer[]={"123456 TEST !\r\n"};   //写入24c02的数据

int main()
{ 	
	u8 readbuf[BUFF_SIZE];
	
	delay_init();         //延时初始化
	InitUartCon();  //串口1初始化	
	InitEepRom();	      //24c02初始化

	while(1)
	{
		WriteDataToEepRom(0,(u8*)Buffer,BUFF_SIZE);  //写入数据
		delay_ms(10);                            //延时10ms
		ReadDataFromEepRom(0,readbuf,BUFF_SIZE);       //读取数据
		delay_ms(10);                            //延时10ms
		
		//SendUartData(0,readbuf,BUFF_SIZE);
		printf2("%s",readbuf);                    //串口输出数据
		memset(readbuf,0,BUFF_SIZE);             //清除读取缓冲区数据，全置为0
		
		delay_ms(500);                           //延时500ms
	}
}

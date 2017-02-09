#include "stm32f10x.h"
#include "iic.h"
#include "delay.h"

static void SetIicSda(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}
static void SetIicScl(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
}
static void ResetIicSda(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}
static void ResetIicScl(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}
static uint8_t ReadTicSda(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
}
/*******************************************************************
* 函数功能：初始化iic
* 参数：		无
* 返回值：  无
*******************************************************************/
void InitIicCon(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );    //使能GPIOB时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //初始化PB8.9
	
	SetIicSda();
	SetIicScl();
}
/*******************************************************************
* 函数功能：启动iic
* 参数：		无
* 返回值：  无
*******************************************************************/
void StartIic(void)
{
	SDA_OUT();     //SDA线输出模式
	SetIicSda();
	SetIicScl();
	delay_us(4);   //延时
 	ResetIicSda();     //SCL高电平的时候，SDA由高到低，发出一个起始信号
	delay_us(4);   //延时
	ResetIicScl();
}

/*******************************************************************
* 函数功能：关闭iic
* 参数：		无
* 返回值：  无
*******************************************************************/
void StopIic(void)
{
	SDA_OUT();     //SDA线输出模式
	ResetIicScl();
	ResetIicSda();     //SDA拉低//STOP:when CLK is high DATA change form low to high
 	delay_us(4);   //延时
	SetIicScl();
	SetIicSda();
	delay_us(4);   //延时 						   	
}

/*******************************************************************
* 函数功能：等待应答
* 参数：		无
* 返回值：  0：成功  1：失败
*******************************************************************/
u8 WaitForIicAck(void)
{
	u8 timeout=0;
	SDA_IN();                  //SDA线输出模式 
	SetIicSda();
	delay_us(1);	   //SDA拉高 延时  
	SetIicScl();
	delay_us(1);	   //SCL拉高 延时 
	
	while(ReadTicSda())            //等待SDA遍地，表示应答带来，不然一直while循环，直到超时
	{
		timeout ++;             //超时计数+1
		if(timeout > 250)        //如果大于250
		{
			StopIic();        //发送停止信号
			return 1;          //返回1，表示失败
		}
	}
	ResetIicScl();                //SCL拉低
	return 0;                  //返回0，表示成功
} 

/*******************************************************************
* 函数功能：应答Iic
* 参数：		无
* 返回值：  无
*******************************************************************/
void AckIic(void)
{
	ResetIicScl();
	SDA_OUT();   //SDA线输出模式 
	ResetIicSda();   //SDA拉低，表示应答
	delay_us(2); //延时
	SetIicScl();   //SCL拉高
	delay_us(2); //延时
	ResetIicScl();   //SCL拉低
}

/*******************************************************************
* 函数功能：不应答Iic
* 参数：		无
* 返回值：  无
*******************************************************************/
void DonotAckIic(void)
{
	ResetIicScl();   //SCL拉低
	SDA_OUT();   //SDA线输出模式 
	SetIicSda();   //SDA拉高，表示不应答
	delay_us(2); //延时
	SetIicScl();   //SCL拉高
	delay_us(2); //延时
	ResetIicScl();  //SCL拉低
}	

/*******************************************************************
* 函数功能：发送一字节数据
* 参数：		无
* 返回值：  无
*******************************************************************/
void SendIicAByte(u8 data)
{                        
	u8 t;   
	SDA_OUT(); 	                  //SDA线输出模式 
	ResetIicScl();                   //SCL拉低，开始数据传输
	for(t=0;t<8;t++)              //for循环，一位一位的发送，从最高位 位7开始
	{
		(data & 0x80) >> 7 == 1 ? SetIicSda() : ResetIicSda();
		data <<= 1; 	              //左移一位，准备下一次发送
		delay_us(2);              //延时
		SetIicScl();                //SCL拉高
		delay_us(2);              //延时
		ResetIicScl();	              //SCL拉低
		delay_us(2);              //延时
	}	 
} 	
/*******************************************************************
* 函数功能：读取一字节数据
* 参数：		无
* 返回值：  无
*******************************************************************/
u8 ReadIicAByte(unsigned char ack)
{
	u8 i,receive=0;
	
	SDA_IN();              //SDA设置为输入
  for(i=0;i<8;i++ )      //for循环，一位一位的读取，从最高位 位7开始
	{
    ResetIicScl();        //SCL拉低    
    delay_us(2);      //延时
		SetIicScl();        //SCL拉高 
    receive <<= 1;      //左移一位，准备下次的读取
    if(ReadTicSda())
		{
			receive++;     //如果读取的是高电平，也就是1，receive+1
			delay_us(1);  //延时
		}
	}		
	if (!ack)          //不需要发送
	{
		DonotAckIic();
	}
	else               //需要发送
	{
		AckIic();
	}
  return receive;
}





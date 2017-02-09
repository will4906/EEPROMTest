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
* �������ܣ���ʼ��iic
* ������		��
* ����ֵ��  ��
*******************************************************************/
void InitIicCon(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );    //ʹ��GPIOBʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //��ʼ��PB8.9
	
	SetIicSda();
	SetIicScl();
}
/*******************************************************************
* �������ܣ�����iic
* ������		��
* ����ֵ��  ��
*******************************************************************/
void StartIic(void)
{
	SDA_OUT();     //SDA�����ģʽ
	SetIicSda();
	SetIicScl();
	delay_us(4);   //��ʱ
 	ResetIicSda();     //SCL�ߵ�ƽ��ʱ��SDA�ɸߵ��ͣ�����һ����ʼ�ź�
	delay_us(4);   //��ʱ
	ResetIicScl();
}

/*******************************************************************
* �������ܣ��ر�iic
* ������		��
* ����ֵ��  ��
*******************************************************************/
void StopIic(void)
{
	SDA_OUT();     //SDA�����ģʽ
	ResetIicScl();
	ResetIicSda();     //SDA����//STOP:when CLK is high DATA change form low to high
 	delay_us(4);   //��ʱ
	SetIicScl();
	SetIicSda();
	delay_us(4);   //��ʱ 						   	
}

/*******************************************************************
* �������ܣ��ȴ�Ӧ��
* ������		��
* ����ֵ��  0���ɹ�  1��ʧ��
*******************************************************************/
u8 WaitForIicAck(void)
{
	u8 timeout=0;
	SDA_IN();                  //SDA�����ģʽ 
	SetIicSda();
	delay_us(1);	   //SDA���� ��ʱ  
	SetIicScl();
	delay_us(1);	   //SCL���� ��ʱ 
	
	while(ReadTicSda())            //�ȴ�SDA��أ���ʾӦ���������Ȼһֱwhileѭ����ֱ����ʱ
	{
		timeout ++;             //��ʱ����+1
		if(timeout > 250)        //�������250
		{
			StopIic();        //����ֹͣ�ź�
			return 1;          //����1����ʾʧ��
		}
	}
	ResetIicScl();                //SCL����
	return 0;                  //����0����ʾ�ɹ�
} 

/*******************************************************************
* �������ܣ�Ӧ��Iic
* ������		��
* ����ֵ��  ��
*******************************************************************/
void AckIic(void)
{
	ResetIicScl();
	SDA_OUT();   //SDA�����ģʽ 
	ResetIicSda();   //SDA���ͣ���ʾӦ��
	delay_us(2); //��ʱ
	SetIicScl();   //SCL����
	delay_us(2); //��ʱ
	ResetIicScl();   //SCL����
}

/*******************************************************************
* �������ܣ���Ӧ��Iic
* ������		��
* ����ֵ��  ��
*******************************************************************/
void DonotAckIic(void)
{
	ResetIicScl();   //SCL����
	SDA_OUT();   //SDA�����ģʽ 
	SetIicSda();   //SDA���ߣ���ʾ��Ӧ��
	delay_us(2); //��ʱ
	SetIicScl();   //SCL����
	delay_us(2); //��ʱ
	ResetIicScl();  //SCL����
}	

/*******************************************************************
* �������ܣ�����һ�ֽ�����
* ������		��
* ����ֵ��  ��
*******************************************************************/
void SendIicAByte(u8 data)
{                        
	u8 t;   
	SDA_OUT(); 	                  //SDA�����ģʽ 
	ResetIicScl();                   //SCL���ͣ���ʼ���ݴ���
	for(t=0;t<8;t++)              //forѭ����һλһλ�ķ��ͣ������λ λ7��ʼ
	{
		(data & 0x80) >> 7 == 1 ? SetIicSda() : ResetIicSda();
		data <<= 1; 	              //����һλ��׼����һ�η���
		delay_us(2);              //��ʱ
		SetIicScl();                //SCL����
		delay_us(2);              //��ʱ
		ResetIicScl();	              //SCL����
		delay_us(2);              //��ʱ
	}	 
} 	
/*******************************************************************
* �������ܣ���ȡһ�ֽ�����
* ������		��
* ����ֵ��  ��
*******************************************************************/
u8 ReadIicAByte(unsigned char ack)
{
	u8 i,receive=0;
	
	SDA_IN();              //SDA����Ϊ����
  for(i=0;i<8;i++ )      //forѭ����һλһλ�Ķ�ȡ�������λ λ7��ʼ
	{
    ResetIicScl();        //SCL����    
    delay_us(2);      //��ʱ
		SetIicScl();        //SCL���� 
    receive <<= 1;      //����һλ��׼���´εĶ�ȡ
    if(ReadTicSda())
		{
			receive++;     //�����ȡ���Ǹߵ�ƽ��Ҳ����1��receive+1
			delay_us(1);  //��ʱ
		}
	}		
	if (!ack)          //����Ҫ����
	{
		DonotAckIic();
	}
	else               //��Ҫ����
	{
		AckIic();
	}
  return receive;
}





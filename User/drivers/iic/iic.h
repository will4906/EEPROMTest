#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f10x.h"

#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}


void InitIicCon(void);
void StartIic(void);
void StopIic(void);

u8 WaitForIicAck(void);
void AckIic(void);
void DonotAckIic(void);
void SendIicAByte(u8 data);
u8 ReadIicAByte(unsigned char ack);









#endif




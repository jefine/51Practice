#ifndef _IIC_H
#define _IIC_H

#include "stc15f2k60s2.h"
#include "intrins.h"

sbit SDA = P2^1;
sbit SCL = P2^0;

void write_at2402(unsigned char addr,unsigned char da);
unsigned char read_at2402(unsigned char addr);
void IIC_Delay(unsigned char i);
void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 

#endif
#ifndef _IIC_H
#define _IIC_H

#include "stc15f2k60s2.h"
#include "intrins.h"

sbit SDA = P2^1;
sbit SCL = P2^0;

void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 

unsigned char read_adc(unsigned char add);
void write_adc(unsigned char add);
void write_dac(unsigned char data1);
#endif
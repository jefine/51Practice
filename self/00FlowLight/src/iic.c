#include "iic.h"

#define DELAY_TIME 50

unsigned char read_adc(unsigned char add)
{
    unsigned char t;
    EA = 0;
    IIC_Start();
    IIC_SendByte(0x90);
    IIC_WaitAck();
    IIC_SendByte(add);
    IIC_WaitAck();

    IIC_Start();
    IIC_SendByte(0x91);
    IIC_WaitAck();
    t = IIC_RecByte();
    IIC_WaitAck();
    IIC_Stop();
    EA = 1;
    return t;
}
void write_dac(unsigned char t)
{
    EA = 0;
    IIC_Start();
    IIC_SendByte(0x90);
    IIC_WaitAck();
    IIC_SendByte(0x40);
    IIC_WaitAck();
    IIC_SendByte(t);
    IIC_WaitAck();
    IIC_Stop();
    EA = 1;
}
//
void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}

//
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					
    IIC_Delay(DELAY_TIME);
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SCL = 0; 
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//
bit IIC_WaitAck(void)
{
    bit ackbit;
	
    SCL  = 1;
    IIC_Delay(DELAY_TIME);
    ackbit = SDA;
    SCL = 0;
    IIC_Delay(DELAY_TIME);
    return ackbit;
}

//
void IIC_SendByte(unsigned char byt)
{
    unsigned char i;

    for(i=0; i<8; i++)
    {
        SCL  = 0;
        IIC_Delay(DELAY_TIME);
        if(byt & 0x80) SDA  = 1;
        else SDA  = 0;
        IIC_Delay(DELAY_TIME);
        SCL = 1;
        byt <<= 1;
        IIC_Delay(DELAY_TIME);
    }
    SCL  = 0;  
}

//
unsigned char IIC_RecByte(void)
{
    unsigned char i, da;
    for(i=0; i<8; i++)
    {   
    	SCL = 1;
		IIC_Delay(DELAY_TIME);
		da <<= 1;
		if(SDA) da |= 1;
		SCL = 0;
		IIC_Delay(DELAY_TIME);
    }
    return da;    
}

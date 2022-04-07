#include "iic.h"

#define DELAY_TIME 50

void write_dac(unsigned char data1){
	IIC_Start();
	IIC_SendByte(0x90);//选定 PCF8591，并且准备写入
	IIC_WaitAck();
	IIC_SendByte(0x40);//0010 0000 意思是将da输出打开
	IIC_WaitAck();
	IIC_SendByte(data1);//写入电压值
	IIC_WaitAck();
	IIC_Stop();
}

void write_adc(unsigned char add){
	IIC_Start();		  //启动
	IIC_SendByte(0x90);   //写入0x90，选择PCF8591，准备写入通道
	IIC_WaitAck();		  //等待
	IIC_SendByte(add);    //写入1或3，1为光敏电阻通道，3为滑变通道
	IIC_WaitAck();		  //等待
	IIC_Stop();           //总线停止
}

unsigned char read_adc(unsigned char add){
	unsigned char temp;
	IIC_Start();		  //启动
	IIC_SendByte(0x90);   //写入0x90，选择PCF8591，准备写入通道
	IIC_WaitAck();		  //等待
	IIC_SendByte(add);    //写入1或3，1为光敏电阻通道，3为滑变通道
	IIC_WaitAck();		  //等待
	IIC_Start();		  //启动
	IIC_SendByte(0x91);   //写入0x91，准备读出数据
	IIC_WaitAck();		  //等待
	temp = IIC_RecByte(); //读出数据
	IIC_WaitAck();		  //等待
	IIC_Stop();           //总线停止
	return temp;
}


//I2C总线内部延时函数
void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}

//I2C总线启动信号
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//I2C总线停止信号
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//发送应答或非应答信号
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

//等待应答
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

//I2C总线发送一个字节数据
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

//I2C总线接收一个字节数据
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

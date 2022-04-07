#include "iic.h"

#define DELAY_TIME 50

void write_dac(unsigned char data1){
	IIC_Start();
	IIC_SendByte(0x90);//ѡ�� PCF8591������׼��д��
	IIC_WaitAck();
	IIC_SendByte(0x40);//0010 0000 ��˼�ǽ�da�����
	IIC_WaitAck();
	IIC_SendByte(data1);//д���ѹֵ
	IIC_WaitAck();
	IIC_Stop();
}

void write_adc(unsigned char add){
	IIC_Start();		  //����
	IIC_SendByte(0x90);   //д��0x90��ѡ��PCF8591��׼��д��ͨ��
	IIC_WaitAck();		  //�ȴ�
	IIC_SendByte(add);    //д��1��3��1Ϊ��������ͨ����3Ϊ����ͨ��
	IIC_WaitAck();		  //�ȴ�
	IIC_Stop();           //����ֹͣ
}

unsigned char read_adc(unsigned char add){
	unsigned char temp;
	IIC_Start();		  //����
	IIC_SendByte(0x90);   //д��0x90��ѡ��PCF8591��׼��д��ͨ��
	IIC_WaitAck();		  //�ȴ�
	IIC_SendByte(add);    //д��1��3��1Ϊ��������ͨ����3Ϊ����ͨ��
	IIC_WaitAck();		  //�ȴ�
	IIC_Start();		  //����
	IIC_SendByte(0x91);   //д��0x91��׼����������
	IIC_WaitAck();		  //�ȴ�
	temp = IIC_RecByte(); //��������
	IIC_WaitAck();		  //�ȴ�
	IIC_Stop();           //����ֹͣ
	return temp;
}


//I2C�����ڲ���ʱ����
void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}

//I2C���������ź�
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//I2C����ֹͣ�ź�
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//����Ӧ����Ӧ���ź�
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

//�ȴ�Ӧ��
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

//I2C���߷���һ���ֽ�����
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

//I2C���߽���һ���ֽ�����
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

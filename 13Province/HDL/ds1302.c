#include "ds1302.h"  									

unsigned char hour,min,sec;

void write_time(unsigned char hour,unsigned char min,unsigned char sec)
{
	EA = 0;
	Write_Ds1302_Byte(0x8e,0x00);
	Write_Ds1302_Byte(0x80,(sec/10)<<4 | sec%10);
	Write_Ds1302_Byte(0x82,(min/10)<<4 | min%10 );
	Write_Ds1302_Byte(0x84,(hour/10)<<4 | hour%10 );
	
//	Write_Ds1302_Byte(0x80,sec*16 );
//	Write_Ds1302_Byte(0x82,min*16  );
//	Write_Ds1302_Byte(0x84,hour*16 );
	
	Write_Ds1302_Byte(0x8e,0x80);
	EA = 1;
}
void read_time(){
	unsigned char t=0,tt =0;
	EA = 0;
//	t = Read_Ds1302_Byte(0x81);
//	sec = t>>4 * 10 + (t&0x0f);
//	t = Read_Ds1302_Byte(0x83);
//	min = t>>4 * 10 + (t&0x0f);
//	t = Read_Ds1302_Byte(0x85);
//	hour = t>>4 * 10 + (t&0x0f);
	
	t = Read_Ds1302_Byte(0x81);
	sec =  (t>>4)*10 +  (t&0x0f);
	
	t = Read_Ds1302_Byte(0x83);
	min =  (t>>4)*10 + (t&0x0f);
	t = Read_Ds1302_Byte(0x85);
	hour =  (t>>4)*10 + (t&0x0f);
	
//	sec = Read_Ds1302_Byte(0x81) %16;
//	min = Read_Ds1302_Byte(0x83) %16;
//	hour = Read_Ds1302_Byte(0x83) %16;
	EA = 1;
}

//д�ֽ�
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//��DS1302�Ĵ���д������
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//��DS1302�Ĵ�����������
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

#ifndef __DS1302_H
#define __DS1302_H

#include <STC15F2K60S2.h>
#include <intrins.h>


sbit SCK = P1^7;		
sbit SDA = P2^3;		
sbit RST = P1^3; 

void write_time(unsigned char hour,unsigned char min,unsigned char sec);
void read_time();
void Write_Ds1302(unsigned char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte( unsigned char address );

#endif

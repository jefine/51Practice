#include "STC15F2K60S2.H"
#include    "intrins.h"

typedef unsigned char u8;
typedef unsigned int u16;

#define LED_SET(x) {EA = 0;P0 = ~(0x01<<(x-1));P2 = (P2 & 0x1f)|0x80;P2 = P2 & 0x1f;EA = 1;}
void Delay100ms();


int main()
{
	u8 i=1;
	
	while(1)
	{
		LED_SET(i++);
		Delay100ms();
		if(i>8)i=1;
	}

	
}
void Delay100ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 144;
	k = 71;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

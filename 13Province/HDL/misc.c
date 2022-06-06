#include "misc.h"


u8 read_key()
{
	u16 t;
	u8 key;
	P44 = 0;P42 = 1;P35 = 1;P34 =1;
	t = P3;
	P44 = 1;P42 =0;
	t = t<<4 | (P3&0x0f);
	P42 = 1;P35 =0;
	t = t<<4 | (P3&0x0f);
	P35 = 1;P34 =0;
	t = t<<4 | (P3&0x0f);
	
	switch (~t)
	{
		case 0x8000: key = 4;break;
		case 0x4000: key = 5;break;
		case 0x2000: key = 6;break;
		case 0x1000: key = 7;break;
		case 0x0800: key = 8;break;
		case 0x0400: key = 9;break;
		case 0x0200: key = 10;break;
		case 0x0100: key = 11;break;
		case 0x0080: key = 12;break;
		case 0x0040: key = 13;break;
		case 0x0020: key = 14;break;
		case 0x0010: key = 15;break;
		case 0x0008: key = 16;break;
		case 0x0004: key = 17;break;
		case 0x0002: key = 18;break;
		case 0x0001: key = 19;break;
		default : key = 0;
	}
	return key;
}
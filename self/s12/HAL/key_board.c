#include "key_board.h"

extern u8 key_val,key_old;
u8 read_key()
{
	u16 t;
	P44 = 0;P42 = 1;P35 = 1;P34 =1;
	t = P3;
	P44 = 1;P42 = 0;
	t = t<<4 | P3&0x0f;
	P42 = 1;P35 =0;
	t = t<<4 | P3&0x0f;
	P35 = 1;P34 =0;
	t = t<<4 | P3&0x0f;
	
	switch(~t)
	{
		case 0x8000:key_val = 4;break;
		case 0x4000:key_val = 5;break;
		case 0x2000:key_val = 6;break;
		case 0x1000:key_val = 7;break;
		case 0x0800:key_val = 8;break;
		case 0x0400:key_val = 9;break;
		case 0x0200:key_val = 10;break;
		case 0x0100:key_val = 11;break;
		case 0x0080:key_val = 12;break;
		case 0x0040:key_val = 13;break;
		case 0x0020:key_val = 14;break;
		case 0x0010:key_val = 15;break;
		case 0x0008:key_val = 16;break;
		case 0x0004:key_val = 17;break;
		case 0x0002:key_val = 18;break;
		case 0x0001:key_val = 19;break;
		default :key_val = 0;
	}
	return key_val;
}


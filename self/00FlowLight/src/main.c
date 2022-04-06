#include "STC15F2K60S2.H"
typedef unsigned char u8;
typedef unsigned int u16;

u8 code t_display[]={                       //????
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

u8 code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //??
u16 Timer0_cnt = 0;
u8 smg[8]={0};
u8 flag_1s = 0;
u8 smg_i = 0;
void Timer0Init(void)		//1??@12.000MHz
{
	AUXR &= 0x7F;		//?????12T??
	TMOD &= 0xF0;		//???????
	TL0 = 0x18;		//???????
	TH0 = 0xFC;		//???????
	TF0 = 0;		//??TF0??
	TR0 = 1;		//???0????
	ET0 = 1;
	EA = 1;
}

void timer0Handle()interrupt 1
{
	if(Timer0_cnt++ == 1000)
	{
		flag_1s = 1;
		Timer0_cnt = 0;
	}
		P0 = T_COM[smg_i];
		P2 = 0xc0;
		P2 = 0;
		P0 = ~t_display[smg[smg_i]];
		P2 = 0xe0;
		P2 = 0;
	if(++smg_i==8)smg_i = 0;
}
void main()
{
	u8 i = 0;
	Timer0Init();
	while(1)
	{
		if(flag_1s)
		{
			flag_1s=0;
			smg[i] = i;
			smg[(i+7)%8]=0;
			P0 = 0x01<<i;
			P2 = 0x80;
			P2 = 0;
			if(++i ==8)i=0;
		}
	}
	
}
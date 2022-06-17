#include "STC15F2K60S2.H"
#include    "intrins.h"
typedef unsigned char u8;
typedef unsigned int u16;

sbit TX = P1^0;
sbit RX = P1^1;

u16 ms,distance;
u8 smg_cnt;
u8 smg_buf[8] = {0};
u8 code t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

u8 code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码

// 13 = 1 000 000 / 40 000 / 2
// 所需的是8个40Khz的发送波
void Delay13us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 36;
	while (--i);
}

void send_wave()
{
	u8 i=8;
	while(i--)
	{
		TX = 1;
		Delay13us();
		TX = 0;
		Delay13us();
	}
}
void get_wave()
{
	u16 time;
	EA = 0;
	send_wave();
	EA = 1;

	TH1 = 0;
	TL1 = 0;
	TR1 = 1;
	while((RX == 1) && (TF1 == 0));
	TR1 = 0;
	if(TF1 == 1)TF1 =0;
	else{
		time = (TH1 << 8) | TL1;
		distance = (u16)(time * 34000 /1000000 / 2);
		//distance = (u16)(time * 0.017);//34 000 / 1000 000 / 2
	}
}
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA = 1;
}
void Timer0Handle() interrupt 1
{
	P0 = T_COM[smg_cnt];
	P2 = 0xc0;
	P2 = 0;

	P0 = ~t_display[smg_buf[smg_cnt]];
	P2 = 0xe0;
	P2 = 0;
	if(smg_cnt++==7)smg_cnt=0;

	if(ms++ == 100000)ms=0;	
}
int main()
{

	Timer0Init();
	
	while(1)
	{
		if((ms % 200) ==0)
		{
			get_wave();
			smg_buf[0] = distance / 100;
			smg_buf[1] = distance / 10 % 10;
			smg_buf[2] = distance % 10;
		}
		
	}
}
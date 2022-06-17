#include "STC15F2K60S2.H"
#include  "intrins.h"
#include "iic.h"
typedef unsigned char u8;
typedef unsigned int u16;

u16 ms;
u8 smg_cnt;
u8 smg_buf[8] = {0};
u8 code t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

u8 code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码


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
	u16 Light_value=0;
	u16 v_value=0;
	Timer0Init();
	write_dac(125);
	
	while(1)
	{
		if(ms%500 == 0)
		{
			v_value = (u16)(read_adc(1)*500.0/255);
			
			smg_buf[0] = v_value /100 +32;
			smg_buf[1] = v_value /10 %10;
			smg_buf[2] = v_value %10;

			Light_value = read_adc(3);
			smg_buf[4] = Light_value / 1000;
			smg_buf[5] = Light_value % 1000 /100;
			smg_buf[6] = Light_value % 100 /10;
			smg_buf[7] = Light_value % 10;
		}
	}
}
#include "stc15f2k60s2.h" 
#include "iic.h"
int cnt = 0;
unsigned char smg_buf[8] = {0};

unsigned char  t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

unsigned char T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码

void Delay1000ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 46;
	j = 153;
	k = 245;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
  ET0 = 1;
  EA = 1;
}

void Timer0Handle() interrupt 1
{
  P0 = T_COM[cnt];
  P2 = 0xc0;
  P2 = 0;
  P0 = ~t_display[smg_buf[cnt]];
  P2 = 0xe0;
  P2 = 0;
  cnt++;
  if(cnt==8)cnt =0;
}

int main()
{
  int value;
  Timer0Init();
  while(1)
  {
    ET0 = 0;
    value = (int)(read_adc(1)*500.0/255);
    ET0 = 1;
    smg_buf[0] = value /100 +32;
    smg_buf[1] = value /10 %10;
    smg_buf[2] = value %10;
    write_dac((unsigned char)(2/5.0*255));
    Delay1000ms();
    
  }
}

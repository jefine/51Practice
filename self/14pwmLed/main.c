#include "stc15f2k60s2.h" 
int cnt = 0;
unsigned int timer1cnt=0,key_val=0;
unsigned char smg_cnt = 0;
unsigned char duty ,ucKey_Old=0,key_statue=0;

unsigned char  t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

unsigned char T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码

//1khz,占空比20%
void Timer0Init(void)		//10微秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x88;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
  ET0 = 1;
  EA = 1;
}
void Timer1Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x20;		//设置定时初值
	TH1 = 0xD1;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
  ET1 = 1;
  EA = 1;
}

void Timer1handle() interrupt 3
{
    P0 = T_COM[timer1cnt%8];
    P2 = 0xc0;
    P2 = 0;
    P0 = ~t_display[duty/10];
    P2 = 0xe0;
    P2 = 0;
    if(timer1cnt++ ==1000)timer1cnt =0;
}

void Timer0Handle() interrupt 1
{
  if(cnt==0)
  {
    P0 = 0xff;
    P2 = 0x80;
    P2 = 0;
  }
  else if (cnt == 100-duty)
  {
    P0 = 0x00;
    P2 = 0x80;
    P2 = 0;
  }
  cnt++;
  if(cnt == 100) cnt = 0;
}
unsigned char read_key()
{
  unsigned int t;
  unsigned char key_val;

  P44 = 0;P42 = 1;P35 = 1;P34 =1;
  t = P3;
  P44 = 1;P42 = 0;
  t = t<<4 | (P3&0x0f);
  P42 = 1;P35 = 0;
  t = t<<4 | (P3&0x0f);
  P35 = 1;P34 = 0;
  t = t<<4 | (P3&0x0f);
  switch (~t)
  {
  case 0x8000:key_val = 4; break;
  case 0x4000:key_val = 5; break;
  case 0x2000:key_val = 6; break;
  case 0x1000:key_val = 7; break;
  case 0x0800:key_val = 8; break;
  case 0x0400:key_val = 9; break;
  case 0x0200:key_val = 10; break;
  case 0x0100:key_val = 11; break;
  case 0x0080:key_val = 12; break;
  case 0x0040:key_val = 13; break;
  case 0x0020:key_val = 14; break;
  case 0x0010:key_val = 15; break;
  case 0x0008:key_val = 16; break;
  case 0x0004:key_val = 17; break;
  case 0x0002:key_val = 18; break;
  case 0x0001:key_val = 19; break;
  default:key_val = 0;break;
  }
  return key_val;
}
unsigned char key_proc()
{
  unsigned char ucKey_Val, ucKey_Down, ucKey_Up;
  ucKey_Val = read_key();
  ucKey_Down = ucKey_Val & (ucKey_Old ^ ucKey_Val);
  ucKey_Up = ~ucKey_Val & (ucKey_Old ^ ucKey_Val);
  ucKey_Old = ucKey_Val;
  return ucKey_Down;
}

void main()
{
  unsigned char key =0;
  Timer0Init();
  Timer1Init();
  duty = 0;
  while(1)
  {
    if(timer1cnt %10 == 0)
    {
      key = key_proc();
      if(key == 4)duty+=10;
      if(key == 5)duty-=10;
    }
  }
}

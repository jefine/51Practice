#include "STC15F2K60S2.h" 
#include "INTRINS.H"
sbit k6 = P4^2;
sbit k7 = P4^4;
unsigned char smg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char wei[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

unsigned char time_cnt = 0;
unsigned char key_statue = 0;

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
  ET0 = 1;
  EA = 1;
}
unsigned char read_keyboard(){
  unsigned char temp,row,col,key_value;
  // row -> 1;
  // col -> 0;
  P3 = 0x0f;
  k6 = 0;
  k7 = 0;
  switch (key_statue)
  {
  case 0:
    temp = P3 & 0x0f;
    if(temp != 0x0f)key_statue = 1;
    break;
  case 1:
    temp = P3 & 0x0f;
    if(temp != 0x0f)
    {
      row = temp;
      k6 = 1;
      k7 = 1;
      P3 = 0x30;
      temp = P3 & 0x30;
      if(temp != 0x30 || k6 !=1 || k7!=1)
      {
        if(temp!=0x30)col = temp | 0xc0;
        else if (k6!=1)
        {
          col = 0xb0;
        }
        else if (k7!=1)
        {
          col = 0x70;
        }
        temp = row | col;
        switch (temp)
        {
          //记忆 7bde 7bde

          case 0x77: key_value=4;break;
					case 0x7b: key_value=5;break;
					case 0x7d: key_value=6;break;
					case 0x7e: key_value=7;break;

					case 0xb7: key_value=8;break;
					case 0xbb: key_value=9;break;
					case 0xbd: key_value=10;break;
					case 0xbe: key_value=11;break;

					case 0xd7: key_value=12;break;
					case 0xdb: key_value=13;break;
					case 0xdd: key_value=14;break;
					case 0xde: key_value=15;break;

					case 0xe7: key_value=16;break;
					case 0xeb: key_value=17;break;
					case 0xed: key_value=18;break;
					case 0xee: key_value=19;break;	
        }
      }
      key_statue = 2;
    }
    else key_statue = 0;
    break;
  case 2:
    P3 = 0x0f;
    temp = P3 & 0x0f;
    if(temp == 0x0f)key_statue = 0;
    break;
  }
  return key_value;
}

void Timer0Handle() interrupt 1	{ 
  unsigned char key = 0;
  if(time_cnt++ <10)return;
  key = read_keyboard();
  // P0 = wei[0];
  // P2 = 0xc0;
  // P2 = 0;
  // P0 = smg[key];
  // P2 = 0xe0;
  // P2 = 0;
  
  if(key == 4){
    P0 = 0x0e;
  }else if(key == 5){
    P0 = 0x0d;
  }else if(key == 6){
    P0 = 0x0b;
  }else if(key == 7){
    P0 = 0x07;
  }
}

int main()
{
  Timer0Init();
  P2 = 0x80;
  while(1)
  {
    
  }
}

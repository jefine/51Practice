#include "STC15F2K60S2.h" 
//初始化函数
int cnt = 0;
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
  ET0 = 1;        //开定时器0中断
  EA = 1;         //开总中断
}

//中断函数
void Time0()interrupt 1{
    //中断内容
    if(cnt++ < 1000)return;
    cnt = 0;
    P0 = ~P0;
    P2 = P2 & 0x1f|0x80;
    P2 = P2 & 0x1f;
}


int main()
{
  Timer0Init();
  while(1)
  {
    
  }
}

